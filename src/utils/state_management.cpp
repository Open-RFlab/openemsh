///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <cmath>
#include <iterator>

#include "state_management.hpp"

using namespace std;

//******************************************************************************
Caretaker& Caretaker::singleton() noexcept {
	static Caretaker c;
	return c;
}

//******************************************************************************
Caretaker::Caretaker() noexcept
: auto_gc(true)
, history_root(make_unique<Timepoint>())
, current_timepoint(history_root.get())
, user_history{ history_root.get() }
{}

//******************************************************************************
void Caretaker::garbage_collector() noexcept {
	// Step 1: fill a set with all pinned/visited/current + ancestors : to keep.
	set<Timepoint*> to_keep;
	for(auto const& list : { pinned_timepoints, user_history, { current_timepoint } })
		for(auto* t : list) {
//			to_keep.insert_range(t->ancestors()); // TODO C++23 unsupported yet feature
			auto ancestors = t->ancestors(true);
			to_keep.insert(begin(ancestors), end(ancestors));
		}

	// Step 2: retrieve all nodes that are not to keep : to remove.
	set<Timepoint*> to_remove;
	ranges::copy_if(history_root->cluster(), inserter(to_remove, end(to_remove)), [&](auto* item) {
		return !to_keep.contains(item);
	});

	// Step 3: for each originator, erase blacklisted nodes.
	for(auto& ptr : originators)
		if(auto originator = ptr.lock(); originator)
			originator->erase(to_remove);

	// Step 4: erase annotations related to blacklisted nodes.
	erase_if(annotations, [&](auto const& item) {
		return to_remove.contains(item.first);
	});

	// Step 5: actually remove nodes from history tree.
	history_root->erase_from_descendants(to_remove);

	// Step 6: gc expired originators.
	erase_if(originators, [](auto const& item) {
		return item.expired();
	});
}

//******************************************************************************
void Caretaker::stop_browsing_user_history() noexcept {
	if(user_history_browser.has_value()) {
		// Erase from browser iterator to end.
		user_history.erase(user_history_browser->base(), user_history.end());
		user_history_browser = nullopt;

		if(auto_gc)
			garbage_collector();
	}
}

//******************************************************************************
Timepoint* Caretaker::get_history_root() noexcept {
	return history_root.get();
}

//******************************************************************************
Timepoint* Caretaker::get_current_timepoint() noexcept {
	return current_timepoint;
}

//******************************************************************************
vector<Timepoint*> const& Caretaker::get_pinned_timepoints() const noexcept {
	return pinned_timepoints;
}

//******************************************************************************
Timepoint* Caretaker::make_next_timepoint() noexcept {
	stop_browsing_user_history();
	current_timepoint = &current_timepoint->add_child();
	return current_timepoint;
}

//******************************************************************************
void Caretaker::take_care_of(shared_ptr<IOriginator> const& originator) noexcept {
	// TODO Are all those checks really useful?
	if(originator
	&& &originator->get_caretaker() == this
	&& originator->get_init_timepoint()->root() == history_root.get())
		originators.emplace_back(originator);
}

//******************************************************************************
void Caretaker::undo(size_t remembered_timepoints) noexcept {
	if(!remembered_timepoints || !can_undo())
		return;

	if(!user_history_browser.has_value()) {
		user_history_browser = user_history.rbegin();
		if(user_history.back() != current_timepoint) // Means current_timepoint -> user_history.back() is the first undo step.
			--remembered_timepoints;
	}

	advance(*user_history_browser, min(remembered_timepoints, (size_t) abs(distance(*user_history_browser, user_history.rend()))));
	current_timepoint = **user_history_browser;

	if(user_history_browser == user_history.rbegin())
		user_history_browser = nullopt;

	go_without_remembering(current_timepoint);
}

//******************************************************************************
void Caretaker::redo(size_t remembered_timepoints) noexcept {
	if(!remembered_timepoints || !can_redo())
		return;

	advance(*user_history_browser, - min(remembered_timepoints, (size_t) abs(distance(user_history.rbegin(), *user_history_browser))));
	current_timepoint = **user_history_browser;

	if(user_history_browser == user_history.rbegin())
		user_history_browser = nullopt;

	go_without_remembering(current_timepoint);
}

//******************************************************************************
bool Caretaker::can_undo() const noexcept {
	if(user_history_browser.has_value()) {
		return *user_history_browser != prev(user_history.rend());
	} else {
		return user_history.size() > 1; // First is history_root
	}
}

//******************************************************************************
bool Caretaker::can_redo() const noexcept {
	return user_history_browser.has_value();
}

//******************************************************************************
void Caretaker::unpin(Timepoint* t) noexcept {
	erase(pinned_timepoints, t);
	if(auto_gc)
		garbage_collector();
}

//******************************************************************************
void Caretaker::pin_current_timepoint() noexcept {
	if(ranges::none_of(pinned_timepoints,
		[this](auto const* item) {
			return item == current_timepoint;
		}
	))
		pinned_timepoints.emplace_back(current_timepoint);
	remember_current_timepoint();
}

//******************************************************************************
void Caretaker::remember_current_timepoint() noexcept {
	if(user_history.back() != current_timepoint)
		user_history.emplace_back(current_timepoint);
}

//******************************************************************************
bool Caretaker::go_without_remembering(Timepoint* t) noexcept {
	if(t && t->root() == history_root.get()) {
		current_timepoint = t;
		for(auto& it : originators)
			if(auto ptr = it.lock(); ptr)
				ptr->go(t);
		return true;
	} else {
		return false;
	}
}

//******************************************************************************
bool Caretaker::go_and_remember(Timepoint* t) noexcept {
	bool does_succeed = go_without_remembering(t);
	if(does_succeed)
		remember_current_timepoint();
	return does_succeed;
}

//******************************************************************************
void Caretaker::annotate_current_timepoint(std::unique_ptr<IAnnotation> annotation) noexcept {
	annotations.emplace(current_timepoint, std::move(annotation));
}

//******************************************************************************
IAnnotation* Caretaker::get_annotation(Timepoint* t) noexcept {
	if(annotations.contains(t))
		return annotations.at(t).get();
	else
		return nullptr;
}

// In predicate, you will probably want to downcast to a well-known user-defined
// type.
//******************************************************************************
Timepoint* Caretaker::find_first_ancestor_with_annotation_that(std::function<bool (IAnnotation const*)> const& predicate, bool include_itself) noexcept{
	for(auto* t : current_timepoint->ancestors(include_itself))
		if(annotations.contains(t) && predicate(annotations.at(t).get()))
			return t;
	return nullptr;
}

//******************************************************************************
Timepoint* Caretaker::find_first_ancestor_with_annotation(bool include_itself) noexcept {
	return find_first_ancestor_with_annotation_that(
		[](auto) { return true; },
		include_itself);
}

//******************************************************************************
bool Caretaker::get_auto_gc() const noexcept {
	return auto_gc;
}

//******************************************************************************
void Caretaker::set_auto_gc(bool auto_gc) noexcept {
	this->auto_gc = auto_gc;
}
