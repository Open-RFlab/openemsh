///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <ranges>
#include <type_traits>
#include <vector>
#include <set>

#include "tree_node.hpp"

using Timepoint = TreeNode;

class IOriginator;

#ifdef UNITTEST
#define private public
#define protected public
#endif // UNITTEST

// TODO fix "state" vs "timepoint" vs "value" usage
// TODO Originator<State>
// TODO noexcept everything


// Remember / Pin
//******************************************************************************
class Caretaker {
private:
	bool auto_gc;
	std::unique_ptr<Timepoint> history_root;
	Timepoint* current_state;
	std::list<std::weak_ptr<IOriginator>> originators;
	std::vector<Timepoint*> pinned_states; // TODO use std::set ?
	std::vector<Timepoint*> user_history;
	std::optional<decltype(user_history)::reverse_iterator> user_history_browser;

//	void go_without_remembering(Timepoint* t); // TODO should this be public to user?
	bool go_without_remembering(Timepoint* t); // TODO should this be public to user?
	void stop_browsing_user_history();

public:
	void garbage_collector();
	static Caretaker& singleton() noexcept;

	Caretaker() noexcept;

	Timepoint* get_history_root();
	Timepoint* get_current_state();
	Timepoint* get_next_state();
	std::vector<Timepoint*> const& get_pinned_states() const;

	void add(std::shared_ptr<IOriginator> const& originator); // TODO rename take_responsability_of() ?

	void undo(std::size_t states = 1);
	void redo(std::size_t states = 1);

//	bool go_and_remember(Timepoint* t, bool overwrite_redoable_history = false); // TODO
	bool go_and_remember(Timepoint* t); // TODO rename go
	void remember_current_state();

	void pin_current_state();
	void unpin(Timepoint* state);

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
	virtual Timepoint* get_init_state() const = 0;
};

//******************************************************************************
template<typename T>
class Originator : public IOriginator {
private:
	Caretaker& caretaker;
	Timepoint* const init_state;
	Timepoint* current_state;
	// TODO An ordered map would fit better.
	// TODO https://github.com/Tessil/ordered-map
	std::map<Timepoint*, T> states; // Keep Timepoint -- T association.
	std::vector<Timepoint*> states_ordered; // Keep Timepoint insertion order.

protected:
	Caretaker& get_caretaker() const final;

public:
	Originator(Timepoint* init_state, T value, Caretaker& caretaker = Caretaker::singleton()) noexcept;

	Timepoint* get_init_state() const final;
	Timepoint* get_current_state() const;

	T const& value() const;

	void go(Timepoint* t) noexcept final;

	void erase(Timepoint* t) final;
	void erase(std::set<Timepoint*> const& ts) noexcept final;

	std::vector<std::pair<Timepoint*, T const&>> get_available_states() const noexcept;

	Timepoint* next_state() const;
	void set_state(Timepoint* k, T const& v);
	void set_next_state(T const& v);
	void set_given_or_next_state(T const& v, Timepoint* t);
//	void set_given_or_next_state(T const& v, Timepoint* t = nullptr);
	std::tuple<Timepoint*, T> make_next_state() const noexcept;
};

#ifdef UNITTEST
#undef private
#undef protected
#endif // UNITTEST

//******************************************************************************
template<typename T>
Originator<T>::Originator(Timepoint* init_state, T value, Caretaker& caretaker) noexcept
: caretaker(caretaker)
, init_state(init_state)
, current_state(init_state)
, states{{ init_state, value }}
, states_ordered{{ init_state }}
{}

//******************************************************************************
template<typename T>
Caretaker& Originator<T>::get_caretaker() const {
	return caretaker;
}

//******************************************************************************
template<typename T>
Timepoint* Originator<T>::get_init_state() const {
	return init_state;
}

//******************************************************************************
template<typename T>
Timepoint* Originator<T>::get_current_state() const {
	return current_state;
}

//******************************************************************************
template<typename T>
T const& Originator<T>::value() const {
	return states.at(current_state);
}

// Go to state or its last ancestor. if desired state is older than init_state, go nullptr
//******************************************************************************
template<typename T>
void Originator<T>::go(Timepoint* state) noexcept {
	if(state)
		for(auto* t : std::ranges::reverse_view(states_ordered)) {
			if(auto* common_ancestor = t->common_ancestor(*state, true)
			; common_ancestor == t) {
				current_state = t;
				return;
			}
		}
	current_state = init_state;
}

//******************************************************************************
template<typename T>
void Originator<T>::erase(Timepoint* t) {
	if(t == init_state || t == current_state) {
		// Nothing to do:
		// - The object must not exist with an invalid state
		// - The object will likely be garbage collected completely during the pass
	} else {
		states.erase(t);
		std::erase(states_ordered, t);
	}
}

//******************************************************************************
template<typename T>
void Originator<T>::erase(std::set<Timepoint*> const& ts) noexcept {
	auto const is_erasable = [&](Timepoint* t) {
		if(t == init_state || t == current_state)
			return false;
		else
			return ts.contains(t);
	};

	std::erase_if(states, [&](auto const& item) {
		return is_erasable(item.first);
	});
	std::erase_if(states_ordered, [&](auto* item) {
		return is_erasable(item);
	});
}

//******************************************************************************
template<typename T>
std::vector<std::pair<Timepoint*, T const&>> Originator<T>::get_available_states() const noexcept {
	std::vector<std::pair<Timepoint*, T const&>> ret;
	for(std::size_t i = 0; i < states_ordered.size(); i++) {
		ret.emplace_back(states_ordered[i], states.at(states_ordered[i]));
	}
	return ret;
}

// Asks the Caretaker to create the next state and return it.
//******************************************************************************
template<typename T>
Timepoint* Originator<T>::next_state() const {
	return caretaker.get_next_state();
}

// TODO Should it be allowed to update existing state?
//******************************************************************************
template<typename T>
void Originator<T>::set_state(Timepoint* k, T const& v) {
	states[k] = v;
	if(std::ranges::none_of(states_ordered, [&](auto const& it) { return it == k; })) {
		states_ordered.push_back(k);
		current_state = k;
	}
}

//******************************************************************************
template<typename T>
void Originator<T>::set_next_state(T const& v) {
	set_state(next_state(), v);
}

//******************************************************************************
template<typename T>
void Originator<T>::set_given_or_next_state(T const& v, Timepoint* t) {
	set_state(t ? t : next_state(), v);
}

// Facility to use when the next state timepoint is required before actually
// setting the new state, eg. to propagate it to new objects.
//******************************************************************************
template<typename T>
std::tuple<Timepoint*, T> Originator<T>::make_next_state() const noexcept {
	return { next_state(), value() };
}
