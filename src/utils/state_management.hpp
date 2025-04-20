///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <algorithm>
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <ranges>
#include <set>
#include <vector>

#include "tree_node.hpp"

using Timepoint = TreeNode;

class IOriginator;

#ifdef UNITTEST
#define private public
#define protected public
#endif // UNITTEST

//******************************************************************************
class Caretaker {
private:
	bool auto_gc;
	std::unique_ptr<Timepoint> history_root;
	Timepoint* current_timepoint;
	std::list<std::weak_ptr<IOriginator>> originators;
	std::vector<Timepoint*> pinned_timepoints; // TODO use std::set ?
	std::vector<Timepoint*> user_history;
	std::optional<decltype(user_history)::reverse_iterator> user_history_browser;

	bool go_without_remembering(Timepoint* t) noexcept;
	void stop_browsing_user_history() noexcept;

public:
	void garbage_collector() noexcept;
	static Caretaker& singleton() noexcept;

	Caretaker() noexcept;

	Timepoint* get_history_root() noexcept;
	Timepoint* get_current_timepoint() noexcept;
	Timepoint* make_next_timepoint() noexcept;
	std::vector<Timepoint*> const& get_pinned_timepoints() const noexcept;

	void take_care_of(std::shared_ptr<IOriginator> const& originator) noexcept;

	void undo(std::size_t remembered_timepoints = 1) noexcept;
	void redo(std::size_t remembered_timepoints = 1) noexcept;

//	bool go_and_remember(Timepoint* t, bool overwrite_redoable_history = false) noexcept; // TODO
	bool go_and_remember(Timepoint* t) noexcept;
	void remember_current_timepoint() noexcept;

	void pin_current_timepoint() noexcept;
	void unpin(Timepoint* t) noexcept;

	bool can_undo() const noexcept;
	bool can_redo() const noexcept;

	bool get_auto_gc() const noexcept;
	void set_auto_gc(bool auto_gc) noexcept;
};

//******************************************************************************
class IOriginator {
private:
protected:
	friend class Caretaker;
	virtual Caretaker& get_caretaker() const noexcept = 0;

public:
	virtual ~IOriginator() = default;
	virtual void go(Timepoint* t) noexcept = 0;
	virtual void erase(std::set<Timepoint*> const& ts) noexcept = 0;
	virtual Timepoint* get_init_timepoint() const noexcept = 0;
};

//******************************************************************************
template<typename State>
class Originator : public IOriginator {
private:
	Caretaker& caretaker;
	Timepoint* const init_timepoint;
	Timepoint* current_timepoint;
	// TODO An ordered map would fit better.
	// TODO https://github.com/Tessil/ordered-map
	std::map<Timepoint*, State> states; // Keep Timepoint -- State association.
	std::vector<Timepoint*> ordered_timepoints; // Keep Timepoint insertion order.

protected:
	Caretaker& get_caretaker() const noexcept final;

public:
	Originator(Timepoint* init_timepoint, State state, Caretaker& caretaker = Caretaker::singleton()) noexcept;
	Originator(Timepoint* init_timepoint, Caretaker& caretaker = Caretaker::singleton()) noexcept;

	Timepoint* get_init_timepoint() const noexcept final;
	Timepoint* get_current_timepoint() const noexcept;

	State const& get_current_state() const noexcept;

	void go(Timepoint* t) noexcept final;

	void erase(std::set<Timepoint*> const& ts) noexcept final;

	std::vector<std::pair<Timepoint*, State const&>> get_available_states() const noexcept;

	Timepoint* next_timepoint() const noexcept;
	void set_state(Timepoint* t, State const& state) noexcept;
	void set_next_state(State const& state) noexcept;
	void set_given_or_next_state(State const& state, Timepoint* t = nullptr) noexcept;
	std::tuple<Timepoint*, State> make_next_state() const noexcept;
};

#ifdef UNITTEST
#undef private
#undef protected
#endif // UNITTEST

//******************************************************************************
template<typename State>
Originator<State>::Originator(Timepoint* init_timepoint, State state, Caretaker& caretaker) noexcept
: caretaker(caretaker)
, init_timepoint(init_timepoint)
, current_timepoint(init_timepoint)
, states{{ init_timepoint, state }}
, ordered_timepoints{{ init_timepoint }}
{}

//******************************************************************************
template<typename State>
Originator<State>::Originator(Timepoint* init_timepoint, Caretaker& caretaker) noexcept
: caretaker(caretaker)
, init_timepoint(init_timepoint)
, current_timepoint(init_timepoint)
, states{{ init_timepoint, State() }}
, ordered_timepoints{{ init_timepoint }}
{}

//******************************************************************************
template<typename State>
Caretaker& Originator<State>::get_caretaker() const noexcept {
	return caretaker;
}

//******************************************************************************
template<typename State>
Timepoint* Originator<State>::get_init_timepoint() const noexcept {
	return init_timepoint;
}

//******************************************************************************
template<typename State>
Timepoint* Originator<State>::get_current_timepoint() const noexcept {
	return current_timepoint;
}

//******************************************************************************
template<typename State>
State const& Originator<State>::get_current_state() const noexcept {
	return states.at(current_timepoint);
}

// Go to t or its last ancestor. If desired timepoint is older than init_timepoint, go nullptr
//******************************************************************************
template<typename State>
void Originator<State>::go(Timepoint* t) noexcept {
	if(t)
		for(auto* it : std::ranges::reverse_view(ordered_timepoints)) {
			if(auto* common_ancestor = it->common_ancestor(*t, true)
			; common_ancestor == it) {
				current_timepoint = it;
				return;
			}
		}
	current_timepoint = init_timepoint;
}

//******************************************************************************
template<typename State>
void Originator<State>::erase(std::set<Timepoint*> const& ts) noexcept {
	auto const is_erasable = [&](Timepoint* t) {
		if(t == init_timepoint || t == current_timepoint)
			return false;
		else
			return ts.contains(t);
	};

	std::erase_if(states, [&](auto const& item) {
		return is_erasable(item.first);
	});
	std::erase_if(ordered_timepoints, [&](auto* item) {
		return is_erasable(item);
	});
}

//******************************************************************************
template<typename State>
std::vector<std::pair<Timepoint*, State const&>> Originator<State>::get_available_states() const noexcept {
	std::vector<std::pair<Timepoint*, State const&>> ret;
	for(std::size_t i = 0; i < ordered_timepoints.size(); i++) {
		ret.emplace_back(ordered_timepoints[i], states.at(ordered_timepoints[i]));
	}
	return ret;
}

// Asks the Caretaker to create the next state and return it.
//******************************************************************************
template<typename State>
Timepoint* Originator<State>::next_timepoint() const noexcept {
	return caretaker.make_next_timepoint();
}

// TODO Should it be allowed to update existing state?
//******************************************************************************
template<typename State>
void Originator<State>::set_state(Timepoint* t, State const& state) noexcept {
	states[t] = state;
	if(std::ranges::none_of(ordered_timepoints, [&](auto const& it) { return it == t; })) {
		ordered_timepoints.push_back(t);
		current_timepoint = t;
	}
}

//******************************************************************************
template<typename State>
void Originator<State>::set_next_state(State const& state) noexcept {
	set_state(next_timepoint(), state);
}

//******************************************************************************
template<typename State>
void Originator<State>::set_given_or_next_state(State const& state, Timepoint* t) noexcept {
	set_state(t ? t : next_timepoint(), state);
}

// Facility to use when the next state timepoint is required before actually
// setting the new state, eg. to propagate it to new objects.
//******************************************************************************
template<typename State>
std::tuple<Timepoint*, State> Originator<State>::make_next_state() const noexcept {
	return { next_timepoint(), get_current_state() };
}
