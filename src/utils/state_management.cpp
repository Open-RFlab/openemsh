///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <cmath>
#include <algorithm>
#include <iterator>
#include <iostream>

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
, current_state(history_root.get())
, user_history{{ history_root.get() }}
{}

//******************************************************************************
void Caretaker::garbage_collector() {
	// Step 1: fill a set with all pinned/visited/current + ancestors : to keep.
	set<Timepoint*> to_keep;
	for(auto const& list : { pinned_states, user_history, { current_state } })
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

	// Step 4: actually remove nodes from history tree.
	history_root->erase_from_descendants(to_remove);

	// Step 5: gc expired originators.
	erase_if(originators, [](auto const& item) {
		return item.expired();
	});
}

//******************************************************************************
void Caretaker::stop_browsing_user_history() {
	if(user_history_browser.has_value()) {
		// Erase from browser iterator to end.
		user_history.erase(user_history_browser->base(), user_history.end());
		user_history_browser = nullopt;

		if(auto_gc)
			garbage_collector();
	}
}

//******************************************************************************
Timepoint* Caretaker::get_history_root() {
	return history_root.get();
}

//******************************************************************************
Timepoint* Caretaker::get_current_state() {
	return current_state;
}

//******************************************************************************
vector<Timepoint*> const& Caretaker::get_pinned_states() const {
	return pinned_states;
}

//******************************************************************************
Timepoint* Caretaker::get_next_state() {
	stop_browsing_user_history();
	current_state = &current_state->add_child();
	return current_state;
}

//******************************************************************************
void Caretaker::add(shared_ptr<IOriginator> const& originator) {
	// TODO Are all those checks really useful?
	if(originator
	&& &originator->get_caretaker() == this
	&& originator->get_init_state()->root() == history_root.get())
		originators.emplace_back(originator);
}

//******************************************************************************
void Caretaker::undo(size_t states) {
	if(!states || !can_undo())
		return;

	if(!user_history_browser.has_value()) {
		user_history_browser = user_history.rbegin();
		if(user_history.back() != current_state) // Means current_state -> user_history.back() is the first undo step.
			--states;
	}

	advance(*user_history_browser, min(states, (size_t) abs(distance(*user_history_browser, user_history.rend()))));
	current_state = **user_history_browser;

	if(user_history_browser == user_history.rbegin())
		user_history_browser = nullopt;

	go_without_remembering(current_state);
}

//******************************************************************************
void Caretaker::redo(size_t states) {
	if(!states || !can_redo())
		return;

	advance(*user_history_browser, - min(states, (size_t) abs(distance(user_history.rbegin(), *user_history_browser))));
	current_state = **user_history_browser;

	if(user_history_browser == user_history.rbegin())
		user_history_browser = nullopt;

	go_without_remembering(current_state);
}

//******************************************************************************
bool Caretaker::can_undo() const {
	if(user_history_browser.has_value()) {
		return *user_history_browser != prev(user_history.rend());
	} else {
		return user_history.size() > 1; // First is history_root
	}
}

//******************************************************************************
bool Caretaker::can_redo() const {
	return user_history_browser.has_value();
}

//******************************************************************************
void Caretaker::unpin(Timepoint* state) {
	erase(pinned_states, state);
	if(auto_gc)
		garbage_collector();
}

//******************************************************************************
void Caretaker::pin_current_state() {
	if(ranges::none_of(pinned_states,
		[this](auto const* item) {
			return item == current_state;
		}
	))
		pinned_states.emplace_back(current_state);
	remember_current_state();
}

//******************************************************************************
void Caretaker::remember_current_state() {
	if(user_history.back() != current_state)
		user_history.emplace_back(current_state);
}

//******************************************************************************
bool Caretaker::go_without_remembering(Timepoint* t) {
	if(t && t->root() == history_root.get()) {
		current_state = t;
		for(auto& it : originators)
			if(auto ptr = it.lock(); ptr)
				ptr->go(t);
		return true;
	} else {
		return false;
	}
}

//******************************************************************************
bool Caretaker::go_and_remember(Timepoint* t) {
	bool does_succeed = go_without_remembering(t);
	if(does_succeed)
		remember_current_state();
	return does_succeed;
}

//******************************************************************************
bool Caretaker::get_auto_gc() const {
	return auto_gc;
}

//******************************************************************************
void Caretaker::set_auto_gc(bool auto_gc) {
	this->auto_gc = auto_gc;
}
