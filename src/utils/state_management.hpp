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

// TODO noexcept everything

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

	bool go_without_remembering(Timepoint* t);
	void stop_browsing_user_history();

public:
	void garbage_collector();
	static Caretaker& singleton() noexcept;

	Caretaker() noexcept;

	Timepoint* get_history_root();
	Timepoint* get_current_timepoint();
	Timepoint* make_next_timepoint();
	std::vector<Timepoint*> const& get_pinned_timepoints() const;

	void take_care_of(std::shared_ptr<IOriginator> const& originator);

	void undo(std::size_t remembered_timepoints = 1);
	void redo(std::size_t remembered_timepoints = 1);

//	bool go_and_remember(Timepoint* t, bool overwrite_redoable_history = false); // TODO
	bool go_and_remember(Timepoint* t);
	void remember_current_timepoint();

	void pin_current_timepoint();
	void unpin(Timepoint* t);

	bool can_undo() const;
	bool can_redo() const;

	bool get_auto_gc() const;
	void set_auto_gc(bool auto_gc);
};

//******************************************************************************
class IOriginator {
private:
protected:
	friend class Caretaker;
	virtual Caretaker& get_caretaker() const = 0;

public:
	virtual ~IOriginator() = default;
	virtual void go(Timepoint* t) = 0;
	virtual void erase(Timepoint* t) = 0;
	virtual void erase(std::set<Timepoint*> const& ts) = 0;
	virtual Timepoint* get_init_timepoint() const = 0;
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
	Caretaker& get_caretaker() const final;

public:
	Originator(Timepoint* init_timepoint, State state, Caretaker& caretaker = Caretaker::singleton()) noexcept;

	Timepoint* get_init_timepoint() const final;
	Timepoint* get_current_timepoint() const;

	State const& get_current_state() const;

	void go(Timepoint* t) noexcept final;

	void erase(Timepoint* t) final;
	void erase(std::set<Timepoint*> const& ts) noexcept final;

	std::vector<std::pair<Timepoint*, State const&>> get_available_states() const noexcept;

	Timepoint* next_timepoint() const;
	void set_state(Timepoint* t, State const& state);
	void set_next_state(State const& state);
	void set_given_or_next_state(State const& state, Timepoint* t = nullptr);
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
Caretaker& Originator<State>::get_caretaker() const {
	return caretaker;
}

//******************************************************************************
template<typename State>
Timepoint* Originator<State>::get_init_timepoint() const {
	return init_timepoint;
}

//******************************************************************************
template<typename State>
Timepoint* Originator<State>::get_current_timepoint() const {
	return current_timepoint;
}

//******************************************************************************
template<typename State>
State const& Originator<State>::get_current_state() const {
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
void Originator<State>::erase(Timepoint* t) {
	if(t == init_timepoint || t == current_timepoint) {
		// Nothing to do:
		// - The object must not exist with an invalid state
		// - The object will likely be garbage collected completely during the pass
	} else {
		states.erase(t);
		std::erase(ordered_timepoints, t);
	}
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
Timepoint* Originator<State>::next_timepoint() const {
	return caretaker.make_next_timepoint();
}

// TODO Should it be allowed to update existing state?
//******************************************************************************
template<typename State>
void Originator<State>::set_state(Timepoint* t, State const& state) {
	states[t] = state;
	if(std::ranges::none_of(ordered_timepoints, [&](auto const& it) { return it == t; })) {
		ordered_timepoints.push_back(t);
		current_timepoint = t;
	}
}

//******************************************************************************
template<typename State>
void Originator<State>::set_next_state(State const& state) {
	set_state(next_timepoint(), state);
}

//******************************************************************************
template<typename State>
void Originator<State>::set_given_or_next_state(State const& state, Timepoint* t) {
	set_state(t ? t : next_timepoint(), state);
}

// Facility to use when the next state timepoint is required before actually
// setting the new state, eg. to propagate it to new objects.
//******************************************************************************
template<typename State>
std::tuple<Timepoint*, State> Originator<State>::make_next_state() const noexcept {
	return { next_timepoint(), get_current_state() };
}
