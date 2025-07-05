///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <cstdlib>
#include <execution>
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
void Caretaker::reset() noexcept {
	current_timepoint = history_root.get();
	pinned_timepoints.clear();
	user_history.erase(next(begin(user_history)), end(user_history));
	user_history_browser.reset();
	originators.clear();
	garbage_collector();
}

//******************************************************************************
void Caretaker::garbage_collector() noexcept {
	// Step 1: fill a set with all pinned/visited/current + ancestors : to keep.
	set<Timepoint*> to_keep;
	auto const keep = [&](auto const& list) {
		for(auto* t : list) {
			auto ancestors = t->ancestors(true);
			to_keep.insert(begin(ancestors), end(ancestors));
		}
	};
	keep(pinned_timepoints);
	keep(user_history);
	keep(array { current_timepoint });
//	// TODO C++26 std::ranges::concat_view & C++23 unsupported yet feature std::set::insert_range()
//	for(auto* t : ranges::concat_view(pinned_timepoints, user_history, { current_timepoint }))
//		to_keep.insert_range(t->ancestors(true));

	// Step 2: retrieve all nodes that are not to keep : to remove.
	set<Timepoint*> to_remove;
	ranges::copy_if(history_root->cluster(), inserter(to_remove, end(to_remove)), [&](auto* item) {
		return !to_keep.contains(item);
	});

	// Step 3: for each originator, erase blacklisted nodes.
	for_each(execution::unseq, begin(originators), end(originators), [&](auto const& ptr) {
		if(auto originator = ptr.lock(); originator)
			originator->erase(to_remove);
	});

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
	&& originator->get_init_timepoint()
	&& originator->get_init_timepoint()->root() == history_root.get())
		originators.emplace_back(originator);
}

//******************************************************************************
void Caretaker::undo(size_t remembered_timepoints) noexcept {
	if(!remembered_timepoints || !can_undo(remembered_timepoints))
		return;

	if(!user_history_browser.has_value()) {
		user_history_browser = user_history.rbegin();
		if(user_history.back() != current_timepoint) // Means current_timepoint -> user_history.back() is the first undo step.
			--remembered_timepoints;
	}

	ranges::advance(*user_history_browser, remembered_timepoints, prev(user_history.rend()));
	current_timepoint = **user_history_browser;

	if(*user_history_browser == user_history.rbegin())
		user_history_browser = nullopt;

	go_without_remembering(current_timepoint);
}

//******************************************************************************
void Caretaker::redo(size_t remembered_timepoints) noexcept {
	if(!remembered_timepoints || !can_redo(remembered_timepoints))
		return;

	ranges::advance(*user_history_browser, - make_signed_t<size_t>(remembered_timepoints), user_history.rbegin());
	current_timepoint = **user_history_browser;

	if(*user_history_browser == user_history.rbegin())
		user_history_browser = nullopt;

	go_without_remembering(current_timepoint);
}

//******************************************************************************
bool Caretaker::can_undo(size_t remembered_timepoints) const noexcept {
	using It = decltype(user_history)::const_reverse_iterator;

	if(!remembered_timepoints)
		return false;
	else if(user_history_browser.has_value())
		return remembered_timepoints
			< (size_t) abs(distance<It>(*user_history_browser, user_history.rend()));
	else
		return remembered_timepoints
			<= ((user_history.back() == current_timepoint)
				? user_history.size() - 1
				: user_history.size());
}

//******************************************************************************
bool Caretaker::can_redo(size_t remembered_timepoints) const noexcept {
	using It = decltype(user_history)::const_reverse_iterator;

	if(!remembered_timepoints)
		return false;
	else if(user_history_browser.has_value())
		return remembered_timepoints
			<= (size_t) abs(distance<It>(user_history.rbegin(), *user_history_browser));
	else
		return false;
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
		for_each(execution::unseq, begin(originators), end(originators), [&](auto const& ptr) {
			if(auto originator = ptr.lock(); originator)
				originator->go(t);
		});
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
void Caretaker::annotate_current_timepoint(unique_ptr<IAnnotation> annotation) noexcept {
	annotations.emplace(current_timepoint, std::move(annotation));
}

//******************************************************************************
IAnnotation* Caretaker::get_annotation(Timepoint* t) noexcept {
	if(annotations.contains(t))
		return annotations.at(t).get();
	else
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
void Caretaker::set_auto_gc(bool _auto_gc) noexcept {
	auto_gc = _auto_gc;
}
