///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "utils/vector_utils.hpp"

#include "utils/state_management.hpp"

/// Originator
///*****************************************************************************
/// @test template<typename State> Originator<State>::Originator(Timepoint* init_timepoint, T state, Caretaker& caretaker) noexcept
/// @test template<typename State> Caretaker& Originator<State>::get_caretaker() const noexcept
/// @test template<typename State> Timepoint* Originator<State>::get_init_timepoint() const noexcept
/// @test template<typename State> Timepoint* Originator<State>::get_current_timepoint() const noexcept
/// @test template<typename State> void Originator<State>::go(Timepoint* t) noexcept
/// @test template<typename State> void Originator<State>::erase(std::set<Timepoint*> const& ts) noexcept
/// @test template<typename State> std::vector<std::tuple<Timepoint*, State>> Originator<State>::get_available_states() const noexcept
/// @test template<typename State> State const& Originator<State>::get_current_state() const noexcept
/// @test template<typename State> Timepoint* Originator<State>::next_timepoint() const noexcept
/// @test template<typename State> void Originator<State>::set_state(Timepoint* t, State const& state) noexcept
/// @test template<typename State> void Originator<State>::set_next_state(State const& state) noexcept
/// @test template<typename State> void Originator<State>::set_given_or_next_state(State const& state, Timepoint* t) noexcept
/// @test template<typename State> std::tuple<Timepoint*, State> Originator<State>::make_next_state() const noexcept
///*****************************************************************************

/// Caretaker
//******************************************************************************
/// @test static Caretaker& Caretaker::singleton() noexcept
/// @test Caretaker::Caretaker() noexcept
/// @test void Caretaker::garbage_collector() noexcept
/// @test void Caretaker::stop_browsing_user_history() noexcept
/// @test Timepoint* Caretaker::get_history_root() noexcept
/// @test Timepoint* Caretaker::get_current_timepoint() noexcept
/// @test vector<Timepoint*> const& Caretaker::get_pinned_timepoints() const noexcept
/// @test Timepoint* Caretaker::make_next_timepoint() noexcept
/// @test void Caretaker::take_care_of(shared_ptr<IOriginator> const& originator) noexcept
/// @test void Caretaker::undo(size_t remembered_timepoints) noexcept
/// @test void Caretaker::redo(size_t remembered_timepoints) noexcept
/// @test bool Caretaker::can_undo(size_t remembered_timepoints) const noexcept
/// @test bool Caretaker::can_redo(size_t remembered_timepoints) const noexcept
/// @test void Caretaker::unpin(Timepoint* t) noexcept
/// @test void Caretaker::pin_current_timepoint() noexcept
/// @test void Caretaker::remember_current_timepoint() noexcept
/// @test bool Caretaker::go_without_remembering(Timepoint* t) noexcept
/// @test bool Caretaker::go_and_remember(Timepoint* t) noexcept
/// @test bool Caretaker::get_auto_gc() const noexcept
/// @test void Caretaker::set_auto_gc(bool _auto_gc) noexcept
/// @test void Caretaker::annotate_current_timepoint(std::unique_ptr<IAnnotation> annotation) noexcept
/// @test IAnnotation* Caretaker::get_annotation(Timepoint* t) noexcept
/// @test Timepoint* Caretaker::find_first_ancestor_with_annotation_that(std::function<bool (IAnnotation const*)> const& predicate) noexcept
/// @test Timepoint* Caretaker::find_first_ancestor_with_annotation() noexcept
///*****************************************************************************

//******************************************************************************
struct StateA {
	std::string str;
	int num;
};

//******************************************************************************
class Annotation : public IAnnotation {
public:
	int const value;
	explicit Annotation(int value) : value(value) {}
};

//******************************************************************************
class A : public Originator<StateA> {
public:
	A(std::string const& str, int num, Timepoint* init_timepoint)
	: Originator(init_timepoint, { .str = str, .num = num })
	{}

	void set_str(std::string const& str, Timepoint* t = nullptr) {
		auto new_state = get_current_state();
		new_state.str = str;
		set_given_or_next_state(new_state, t);
	}

	void set_num(int num, Timepoint* t = nullptr) {
		auto new_state = get_current_state();
		new_state.num = num;
		set_given_or_next_state(new_state, t);
	}

	std::string const& get_str() const {
		return get_current_state().str;
	}

	int get_num() const {
		return get_current_state().num;
	}
};

//******************************************************************************
struct StateManager {
	int id;
	std::vector<std::shared_ptr<A>> all_a;
//	std::vector<std::shared_ptr<B>> all_b;
};

//******************************************************************************
class Manager : public Originator<StateManager> {
public:
	Manager(int id, Timepoint* init_timepoint)
	: Originator(init_timepoint, { .id = id })
	{}

	void set_id(int id) {
		auto new_state = get_current_state();
		new_state.id = id;
		set_next_state(new_state);
	}

	A* make_a(std::string const& str, int num) {
		auto [new_timepoint, new_state] = make_next_state();
		auto ret = new_state.all_a.emplace_back(std::make_shared<A>(str, num, new_timepoint));
		get_caretaker().take_care_of(ret);
		set_state(new_timepoint, new_state);
		return ret.get();
	}

	void remove_a(A* a) {
		auto new_state = get_current_state();
		std::erase_if(new_state.all_a, [&](auto const& item) {
			return item.get() == a;
		});
		set_next_state(new_state);
	}

	void increment_num_of_once_a_in_two() {
		auto* new_state = next_timepoint();
		bool alternate = true;
		for(auto a : get_current_state().all_a) {
			if(alternate) {
				a->set_num(a->get_num() + 1, new_state);
			}
			alternate = !alternate;
		}
	}

	// Make one B per A.
//	void sync_b() {
//		auto new_state = get_current_state();
//		for(auto a : new_state.all_a) {
//		}
//		set_next_state(new_state);
//	}
};

//******************************************************************************
constexpr Timepoint* at(std::list<Timepoint*> const& list, std::size_t i) {
	return *std::next(list.begin(), i);
}

//******************************************************************************
SCENARIO("template<typename State> Originator<State>::Originator(Timepoint* init_timepoint, T state, Caretaker& caretaker) noexcept", "[utils][state_management]") {
	GIVEN("An Originator") {
		Caretaker c;
		Timepoint t;
		Originator<StateA> a(&t, { .str = "ac", .num = 56 });
		Originator<StateA> b(&t, { .str = "ac", .num = 56 }, c);

		THEN("Init Timepoint and its associated state should be set as current Timepoint") {
			REQUIRE(a.init_timepoint == &t);
			REQUIRE(a.current_timepoint == &t);

			REQUIRE(a.ordered_timepoints.size() == 1);
			REQUIRE(a.ordered_timepoints[0] == &t);

			REQUIRE(a.states.contains(&t));
			REQUIRE(a.states.at(&t).str == "ac");
			REQUIRE(a.states.at(&t).num == 56);
		}

		WHEN("Specifying no Caretaker") {
			THEN("Should set the Caretaker singleton") {
				REQUIRE(&a.caretaker == &Caretaker::singleton());
			}
		}
	}
}

//******************************************************************************
SCENARIO("template<typename State> Caretaker& Originator<State>::get_caretaker() const noexcept", "[utils][state_management]") {
	GIVEN("An Originator") {
		Caretaker c;
		Timepoint t;
		Originator<StateA> a(&t, { .str = "ac", .num = 56 }, c);

		THEN("Should return the specified Caretaker") {
			REQUIRE(&a.get_caretaker() == &c);
		}
	}
}

//******************************************************************************
SCENARIO("template<typename State> Timepoint* Originator<State>::get_init_timepoint() const noexcept", "[utils][state_management]") {
	GIVEN("An Originator") {
		Timepoint t;
		Originator<StateA> a(&t, { .str = "ac", .num = 56 });

		THEN("Should return the specified Timepoint") {
			REQUIRE(a.get_init_timepoint() == &t);
		}

		WHEN("Going to the next state") {
			a.set_next_state({ .str = "lp", .num = 8 });

			THEN("Should still return the specified Timepoint") {
				REQUIRE(a.get_init_timepoint() == &t);
			}
		}
	}
}

//******************************************************************************
SCENARIO("template<typename State> Timepoint* Originator<State>::get_current_timepoint() const noexcept", "[utils][state_management]") {
	GIVEN("An Originator") {
		Timepoint t;
		Originator<StateA> a(&t, { .str = "ac", .num = 56 });

		THEN("Should return the specified Timepoint") {
			REQUIRE(a.get_current_timepoint() == &t);
		}

		WHEN("Going to the next state") {
			a.set_next_state({ .str = "lp", .num = 8 });

			THEN("Should return another Timepoint") {
				REQUIRE_FALSE(a.get_current_timepoint() == &t);
			}
		}
	}
}

//******************************************************************************
SCENARIO("template<typename State> void Originator<State>::go(Timepoint* t) noexcept", "[utils][state_management]") {
	GIVEN("An Originator having multiple states, corresponding to an history tree") {
		// + a
		//   + b <---------- 0 init
		//     + c1
		//     | + d1 <----- 2
		//     | + d2 <----- 1
		//     | | + e1 <--- 3 current
		//     | | + e2
		//     | + d3
		//     + c2

		[[maybe_unused]] Timepoint a;
		[[maybe_unused]] Timepoint& b = a.add_child();
		[[maybe_unused]] Timepoint& c1 = b.add_child();
		[[maybe_unused]] Timepoint& c2 = b.add_child();
		[[maybe_unused]] Timepoint& d1 = c1.add_child();
		[[maybe_unused]] Timepoint& d2 = c1.add_child();
		[[maybe_unused]] Timepoint& d3 = c1.add_child();
		[[maybe_unused]] Timepoint& e1 = d2.add_child();
		[[maybe_unused]] Timepoint& e2 = d2.add_child();

		Originator<StateA> x(&b, { .str = "ac", .num = 56 });
		x.set_state(&d2, { .str = "lp", .num = 8 });
		x.set_state(&d1, { .str = "gh", .num = 444 });
		x.set_state(&e1, { .str = "lo", .num = 69 });

		WHEN("Trying to go to an existing timepoint") {
			x.go(&d2);

			THEN("Current timepoint should be updated to this timepoint") {
				REQUIRE(x.get_current_timepoint() == &d2);
			}
		}

		WHEN("Trying to go to an unknown timepoint that is a descendant of an existing timepoint") {
			x.go(&d3);

			THEN("Current timepoint should be updated to the first known parent of this timepoint") {
				REQUIRE(x.get_current_timepoint() == &b);
			}
		}

		WHEN("Trying to go to an unknown timepoint that is both an ancestor and a descendant of some existing timepoints") {
			x.go(&c1);

			THEN("Current timepoint should be updated to the first known parent of this timepoint") {
				REQUIRE(x.get_current_timepoint() == &b);
			}
		}

		WHEN("Trying to go to an unrelated timepoint") {
			Timepoint t;
			x.go(&t);

			THEN("Current timepoint should be updated to the init timepoint") {
				REQUIRE(x.get_current_timepoint() == &b);
				REQUIRE(x.get_init_timepoint() == &b);
			}
		}

		WHEN("Trying to go to a null timepoint") {
			x.go(nullptr);

			THEN("Current timepoint should be updated to the init timepoint") {
				REQUIRE(x.get_current_timepoint() == &b);
				REQUIRE(x.get_init_timepoint() == &b);
			}
		}
	}
}

//******************************************************************************
SCENARIO("template<typename State> void Originator<State>::erase(std::set<Timepoint*> const& ts) noexcept", "[utils][state_management]") {
	GIVEN("An Originator having multiple states") {
		Timepoint t0;
		Timepoint& t1 = t0.add_child();
		Timepoint& t2 = t1.add_child();
		Timepoint& t3 = t2.add_child();
		Timepoint& t4 = t3.add_child();
		Timepoint& t5 = t4.add_child();
		Originator<StateA> a(&t0, { .str = "ac", .num = 56 });
		a.set_state(&t1, { .str = "lp", .num = 8 });
		a.set_state(&t2, { .str = "gh", .num = 444 });
		a.set_state(&t3, { .str = "lo", .num = 69 });
		a.set_state(&t4, { .str = "dk", .num = 12 });
		a.set_state(&t5, { .str = "ws", .num = 6 });

		WHEN("Trying to erase at once the init state, the current state, two other states and a state that does not exist") {
			Timepoint t;
			a.erase({ &t0, &t5, &t2, &t3, &t });

			THEN("Should only erase the two other states") {
				REQUIRE(a.get_init_timepoint() == &t0);
				REQUIRE(a.get_current_timepoint() == &t5);
				REQUIRE(a.ordered_timepoints.size() == 4);
				REQUIRE(a.ordered_timepoints[0] == &t0);
				REQUIRE(a.ordered_timepoints[1] == &t1);
				REQUIRE(a.ordered_timepoints[2] == &t4);
				REQUIRE(a.ordered_timepoints[3] == &t5);
				REQUIRE(a.states.contains(&t0));
				REQUIRE(a.states.contains(&t1));
				REQUIRE_FALSE(a.states.contains(&t2));
				REQUIRE_FALSE(a.states.contains(&t3));
				REQUIRE(a.states.contains(&t4));
				REQUIRE(a.states.contains(&t5));
				REQUIRE(a.states.at(&t0).str == "ac");
				REQUIRE(a.states.at(&t0).num == 56);
				REQUIRE(a.states.at(&t1).str == "lp");
				REQUIRE(a.states.at(&t1).num == 8);
				REQUIRE(a.states.at(&t4).str == "dk");
				REQUIRE(a.states.at(&t4).num == 12);
				REQUIRE(a.states.at(&t5).str == "ws");
				REQUIRE(a.states.at(&t5).num == 6);
			}
		}

		WHEN("Trying to erase nothing") {
			a.erase({});

			THEN("Should not do anything") {
				REQUIRE(a.get_init_timepoint() == &t0);
				REQUIRE(a.get_current_timepoint() == &t5);
				REQUIRE(a.ordered_timepoints.size() == 6);
				REQUIRE(a.ordered_timepoints[0] == &t0);
				REQUIRE(a.ordered_timepoints[1] == &t1);
				REQUIRE(a.ordered_timepoints[2] == &t2);
				REQUIRE(a.ordered_timepoints[3] == &t3);
				REQUIRE(a.ordered_timepoints[4] == &t4);
				REQUIRE(a.ordered_timepoints[5] == &t5);
				REQUIRE(a.states.contains(&t0));
				REQUIRE(a.states.contains(&t1));
				REQUIRE(a.states.contains(&t2));
				REQUIRE(a.states.contains(&t3));
				REQUIRE(a.states.contains(&t4));
				REQUIRE(a.states.contains(&t5));
				REQUIRE(a.states.at(&t0).str == "ac");
				REQUIRE(a.states.at(&t0).num == 56);
				REQUIRE(a.states.at(&t1).str == "lp");
				REQUIRE(a.states.at(&t1).num == 8);
				REQUIRE(a.states.at(&t2).str == "gh");
				REQUIRE(a.states.at(&t2).num == 444);
				REQUIRE(a.states.at(&t3).str == "lo");
				REQUIRE(a.states.at(&t3).num == 69);
				REQUIRE(a.states.at(&t4).str == "dk");
				REQUIRE(a.states.at(&t4).num == 12);
				REQUIRE(a.states.at(&t5).str == "ws");
				REQUIRE(a.states.at(&t5).num == 6);
			}
		}
	}
}

//******************************************************************************
SCENARIO("template<typename State> std::vector<std::tuple<Timepoint*, State const&>> Originator<State>::get_available_states() const noexcept", "[utils][state_management]") {
	GIVEN("An Originator having multiple states, corresponding to an history tree") {
		// + a
		//   + b <---------- 0 init
		//     + c1
		//     | + d1 <----- 2
		//     | + d2 <----- 1
		//     | | + e1 <--- 3
		//     | | + e2 <--- 5 current
		//     | + d3
		//     + c2 <------- 4


		[[maybe_unused]] Timepoint a;
		[[maybe_unused]] Timepoint& b = a.add_child();
		[[maybe_unused]] Timepoint& c1 = b.add_child();
		[[maybe_unused]] Timepoint& c2 = b.add_child();
		[[maybe_unused]] Timepoint& d1 = c1.add_child();
		[[maybe_unused]] Timepoint& d2 = c1.add_child();
		[[maybe_unused]] Timepoint& d3 = c1.add_child();
		[[maybe_unused]] Timepoint& e1 = d2.add_child();
		[[maybe_unused]] Timepoint& e2 = d2.add_child();

		Originator<StateA> x(&b, { .str = "ac", .num = 56 });
		x.set_state(&d2, { .str = "lp", .num = 8 });
		x.set_state(&d1, { .str = "gh", .num = 444 });
		x.set_state(&e1, { .str = "lo", .num = 69 });
		x.set_state(&c2, { .str = "dk", .num = 12 });
		x.set_state(&e2, { .str = "ws", .num = 6 });

		THEN("Should return all the states in insertion order") {
			auto y = x.get_available_states();

			REQUIRE(y.size() == 6);
			REQUIRE(y[0].first == &b);
			REQUIRE(y[1].first == &d2);
			REQUIRE(y[2].first == &d1);
			REQUIRE(y[3].first == &e1);
			REQUIRE(y[4].first == &c2);
			REQUIRE(y[5].first == &e2);
			REQUIRE(y[0].second.str == "ac");
			REQUIRE(y[0].second.num == 56);
			REQUIRE(y[1].second.str == "lp");
			REQUIRE(y[1].second.num == 8);
			REQUIRE(y[2].second.str == "gh");
			REQUIRE(y[2].second.num == 444);
			REQUIRE(y[3].second.str == "lo");
			REQUIRE(y[3].second.num == 69);
			REQUIRE(y[4].second.str == "dk");
			REQUIRE(y[4].second.num == 12);
			REQUIRE(y[5].second.str == "ws");
			REQUIRE(y[5].second.num == 6);
		}
	}
}

//******************************************************************************
SCENARIO("template<typename State> State const& Originator<State>::get_current_state() const noexcept", "[utils][state_management]") {
	GIVEN("An Originator") {
		Timepoint t;
		Originator<StateA> a(&t, { .str = "ac", .num = 56 });

		THEN("Should return the init state object") {
			REQUIRE(a.get_current_state().str == "ac");
			REQUIRE(a.get_current_state().num == 56);
		}

		WHEN("Going to the next state") {
			a.set_next_state({ .str = "lp", .num = 8 });

			THEN("Should return the next state object") {
				REQUIRE(a.get_current_state().str == "lp");
				REQUIRE(a.get_current_state().num == 8);
			}
		}
	}
}

//******************************************************************************
SCENARIO("template<typename State> Timepoint* Originator<State>::next_timepoint() const noexcept", "[utils][state_management]") {
	GIVEN("An Originator which init timepoint is part of the history tree of its Caretaker") {
		Caretaker c;
		Timepoint* t = c.get_history_root();
		Originator<StateA> a(t, { .str = "ac", .num = 56 }, c);

		THEN("Its current timepoint must be the same as the Caretaker current timepoint") {
			REQUIRE(a.get_current_timepoint() == t);
			REQUIRE(a.get_current_timepoint() == c.get_current_timepoint());
		}

		WHEN("Running next_timepoint") {
			Timepoint* n = a.next_timepoint();

			THEN("Should not update its current timepoint") {
				REQUIRE(a.get_current_timepoint() == t);
			}

			THEN("Should return a new Timepoint that is a child of the init Timepoint") {
				REQUIRE(n != t);
				REQUIRE(n->parent() == t);

				AND_THEN("Caretaker current timepoint should be updated to this new Timepoint") {
					REQUIRE(c.get_current_timepoint() == n);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("template<typename State> void Originator<State>::set_state(Timepoint* t, State const& state) noexcept", "[utils][state_management]") {
	GIVEN("An Originator") {
		Timepoint t0;
		Originator<StateA> a(&t0, { .str = "ac", .num = 56 });
		Originator<StateA const> b(&t0, { .str = "ac", .num = 56 });

		WHEN("Setting a new state") {
			Timepoint t1;
			a.set_state(&t1, { .str = "lp", .num = 8 });
			b.set_state(&t1, { .str = "lp", .num = 8 });

			THEN("The new state should be registered to the given Timepoint") {
				REQUIRE(a.states.contains(&t1));
				REQUIRE(a.states.at(&t1).str == "lp");
				REQUIRE(a.states.at(&t1).num == 8);
				REQUIRE(b.states.contains(&t1));
				REQUIRE(b.states.at(&t1).str == "lp");
				REQUIRE(b.states.at(&t1).num == 8);
			}

			THEN("The given Timepoint should be appended to timepoints order tracking") {
				REQUIRE(a.ordered_timepoints.size() == 2);
				REQUIRE(a.ordered_timepoints[0] == &t0);
				REQUIRE(a.ordered_timepoints[1] == &t1);
				REQUIRE(b.ordered_timepoints.size() == 2);
				REQUIRE(b.ordered_timepoints[0] == &t0);
				REQUIRE(b.ordered_timepoints[1] == &t1);
			}

			THEN("The current timepoint should be updated to the given Timepoint") {
				REQUIRE(a.get_current_timepoint() == &t1);
				REQUIRE(b.get_current_timepoint() == &t1);
			}

			THEN("The init timepoint should not change") {
				REQUIRE(a.get_init_timepoint() == &t0);
				REQUIRE(b.get_init_timepoint() == &t0);
			}

			AND_WHEN("Setting an existing state") {
				a.set_state(&t0, { .str = "gh", .num = 444 });
				b.set_state(&t0, { .str = "gh", .num = 444 });

				AND_WHEN("State type is not const") {
					THEN("The new state should be registered to the given Timepoint") {
						REQUIRE(a.states.contains(&t0));
						REQUIRE(a.states.at(&t0).str == "gh");
						REQUIRE(a.states.at(&t0).num == 444);
					}
				}

				AND_WHEN("State type is const") {
					THEN("The new state should not be registered to the given Timepoint") {
						REQUIRE(b.states.contains(&t0));
						REQUIRE(b.states.at(&t0).str == "ac");
						REQUIRE(b.states.at(&t0).num == 56);
					}
				}

				THEN("The given Timepoint should not be appended to timepoints order tracking") {
					REQUIRE(a.ordered_timepoints.size() == 2);
					REQUIRE(a.ordered_timepoints[0] == &t0);
					REQUIRE(a.ordered_timepoints[1] == &t1);
					REQUIRE(b.ordered_timepoints.size() == 2);
					REQUIRE(b.ordered_timepoints[0] == &t0);
					REQUIRE(b.ordered_timepoints[1] == &t1);
				}

				THEN("The current timepoint should not be updated to the given Timepoint") {
					REQUIRE(a.get_current_timepoint() == &t1);
					REQUIRE(b.get_current_timepoint() == &t1);
				}

				THEN("The init timepoint should not change") {
					REQUIRE(a.get_init_timepoint() == &t0);
					REQUIRE(b.get_init_timepoint() == &t0);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("template<typename State> void Originator<State>::set_next_state(State const& state) noexcept", "[utils][state_management]") {
	GIVEN("An Originator which init state is part of the history tree of its Caretaker") {
		Caretaker c;
		Timepoint* t0 = c.get_history_root();
		Originator<StateA> a(t0, { .str = "ac", .num = 56 }, c);

		WHEN("Setting the next state") {
			a.set_next_state({ .str = "lp", .num = 8 });

			THEN("Current timepoint should be updated to a new Timepoint that is a child of the init Timepoint") {
				Timepoint* t1 = a.get_current_timepoint();
				REQUIRE(t1->parent() == t0);

				AND_THEN("The new state should be registered to this new Timepoint") {
					REQUIRE(a.states.contains(t1));
					REQUIRE(a.states.at(t1).str == "lp");
					REQUIRE(a.states.at(t1).num == 8);
				}

				AND_THEN("Caretaker current timepoint should be updated to this new Timepoint") {
					REQUIRE(c.get_current_timepoint() == t1);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("template<typename State> void Originator<State>::set_given_or_next_state(State const& state, Timepoint* t) noexcept", "[utils][state_management]") {
	GIVEN("An Originator which init state is part of the history tree of its Caretaker") {
		Caretaker c;
		Timepoint* t0 = c.get_history_root();
		Originator<StateA> a(t0, { .str = "ac", .num = 56 }, c);

		WHEN("Setting the next state without specifying a Timepoint") {
			a.set_given_or_next_state({ .str = "lp", .num = 8 }, nullptr);

			THEN("Current timepoint should be updated to a new Timepoint that is a child of the init Timepoint") {
				Timepoint* t1 = a.get_current_timepoint();
				REQUIRE(t1->parent() == t0);

				AND_THEN("The new state should be registered to this new Timepoint") {
					REQUIRE(a.states.contains(t1));
					REQUIRE(a.states.at(t1).str == "lp");
					REQUIRE(a.states.at(t1).num == 8);
				}

				AND_THEN("Caretaker current timepoint should be updated to this new Timepoint") {
					REQUIRE(c.get_current_timepoint() == t1);
				}
			}
		}

		WHEN("Setting the next state specifying a Timepoint") {
			Timepoint n;
			a.set_given_or_next_state({ .str = "lp", .num = 8 }, &n);

			THEN("Current timepoint should be updated to the given Timepoint") {
				Timepoint* t1 = a.get_current_timepoint();
				REQUIRE(t1 == &n);

				AND_THEN("The new state should be registered to this given Timepoint") {
					REQUIRE(a.states.contains(t1));
					REQUIRE(a.states.at(t1).str == "lp");
					REQUIRE(a.states.at(t1).num == 8);
				}

				AND_THEN("Caretaker current timepoint should not be updated to this given Timepoint") {
					REQUIRE(c.get_current_timepoint() == t0);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("template<typename State> std::tuple<Timepoint*, State> Originator<State>::make_next_state() const noexcept", "[utils][state_management]") {
	GIVEN("An Originator which init state is part of the history tree of its Caretaker") {
		Caretaker c;
		Timepoint* t0 = c.get_history_root();
		Originator<StateA> a(t0, { .str = "ac", .num = 56 }, c);

		WHEN("Making the next state") {
			auto [t1, s] = a.make_next_state();

			THEN("Should return a new Timepoint that is a child of the init Timepoint and a copy of the current state") {
				REQUIRE(t1->parent() == t0);
				REQUIRE(s.str == "ac");
				REQUIRE(s.num == 56);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

//******************************************************************************
SCENARIO("static Caretaker& Caretaker::singleton() noexcept", "[utils][state_management]") {
	WHEN("Running a first time") {
		Caretaker& c = Caretaker::singleton();

		THEN("Should create a Caretaker and return its address") {
			REQUIRE(&c != nullptr);

			AND_WHEN("Running other times") {
				THEN("Should still return the same address") {
					REQUIRE(&Caretaker::singleton() == &c);
					REQUIRE(&Caretaker::singleton() == &c);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("Caretaker::Caretaker() noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker") {
		Caretaker c;

		THEN("Current timepoint should be set to the history tree root") {
			REQUIRE(c.current_timepoint == c.history_root.get());
		}

		THEN("User history should contain to the history tree root") {
			REQUIRE(c.user_history.size() == 1);
			REQUIRE(at(c.user_history, 0) == c.history_root.get());
		}
	}
}

//******************************************************************************
SCENARIO("void Caretaker::garbage_collector() noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker with an history tree and managing some Originators") {
		// + a <---------- root, init
		//   + b
		//     + c1 <------- y1
		//     | + d1
		//     | + d2 <----- x1
		//     | | + e1 <--- current
		//     | | + e2 <--- x2, annotated
		//     | + d3 <----- y2
		//     | | + f1 <--- y3, pinned, annotated
		//     | + d4 <----- y4
		//     |   + g1 <--- y5, remembered
		//     + c2 <------- z1

		Caretaker c;
		[[maybe_unused]] Timepoint* a = c.get_history_root();
		[[maybe_unused]] Timepoint* b = &a->add_child();
		[[maybe_unused]] Timepoint* c1 = &b->add_child();
		[[maybe_unused]] Timepoint* c2 = &b->add_child();
		[[maybe_unused]] Timepoint* d1 = &c1->add_child();
		[[maybe_unused]] Timepoint* d2 = &c1->add_child();
		[[maybe_unused]] Timepoint* d3 = &c1->add_child();
		[[maybe_unused]] Timepoint* d4 = &c1->add_child();
		[[maybe_unused]] Timepoint* e1 = &d2->add_child();
		[[maybe_unused]] Timepoint* e2 = &d2->add_child();
		[[maybe_unused]] Timepoint* f1 = &d3->add_child();
		[[maybe_unused]] Timepoint* g1 = &d4->add_child();

		auto x = std::make_shared<Originator<StateA>>(a, StateA { .str = "ac", .num = 56 }, c);
		auto y = std::make_shared<Originator<StateA>>(a, StateA { .str = "lp", .num = 8 }, c);
		auto z = std::make_shared<Originator<StateA>>(a, StateA { .str = "gh", .num = 444 }, c);
		x->set_state(d2, { .str = "lo", .num = 69 });
		x->set_state(e2, { .str = "lo", .num = 68 });
		y->set_state(c1, { .str = "dk", .num = 12 });
		y->set_state(d3, { .str = "dk", .num = 13 });
		y->set_state(f1, { .str = "dk", .num = 14 });
		y->set_state(d4, { .str = "dk", .num = 15 });
		y->set_state(g1, { .str = "dk", .num = 16 });
		z->set_state(c2, { .str = "ws", .num = 6 });
		c.take_care_of(x);
		c.take_care_of(y);
		c.take_care_of(z);
		z.reset();

		c.set_auto_gc(false);
		c.go_without_remembering(e2);
		c.annotate_current_timepoint(std::make_unique<Annotation>(5));
		c.go_without_remembering(f1);
		c.pin_current_timepoint();
		c.annotate_current_timepoint(std::make_unique<Annotation>(6));
		c.undo();
		c.stop_browsing_user_history();
		c.go_and_remember(g1);
		c.go_without_remembering(e1);

		auto const contains_originator = [](std::vector<std::weak_ptr<IOriginator>> const& list, IOriginator* originator) {
			return std::ranges::any_of(list, [&](auto const& wptr) {
				return wptr.expired() ? false : (wptr.lock().get() == originator);
			});
		};
		auto const contains_expired = [](std::vector<std::weak_ptr<IOriginator>> const& list) {
			return std::ranges::any_of(list, [](auto const& wptr) {
				return wptr.expired();
			});
		};

		REQUIRE(c.history_root.get() == a);
		REQUIRE(c.current_timepoint == e1);
		REQUIRE(c.pinned_timepoints.size() == 1);
		REQUIRE(contains(c.pinned_timepoints, f1));
		REQUIRE(c.user_history.size() == 2);
		REQUIRE(at(c.user_history, 0) == a);
		REQUIRE(at(c.user_history, 1) == g1);
		REQUIRE(c.originators.size() == 3);
		REQUIRE(contains_originator(c.originators, x.get()));
		REQUIRE(contains_originator(c.originators, y.get()));
		REQUIRE(contains_expired(c.originators));

		REQUIRE(x->states.size() == 3);
		REQUIRE(x->states[a].str == "ac");
		REQUIRE(x->states[a].num == 56);
		REQUIRE(x->states[d2].str == "lo");
		REQUIRE(x->states[d2].num == 69);
		REQUIRE(x->states[e2].str == "lo");
		REQUIRE(x->states[e2].num == 68);

		REQUIRE(y->states.size() == 6);
		REQUIRE(y->states[a].str == "lp");
		REQUIRE(y->states[a].num == 8);
		REQUIRE(y->states[c1].str == "dk");
		REQUIRE(y->states[c1].num == 12);
		REQUIRE(y->states[d3].str == "dk");
		REQUIRE(y->states[d3].num == 13);
		REQUIRE(y->states[f1].str == "dk");
		REQUIRE(y->states[f1].num == 14);
		REQUIRE(y->states[d4].str == "dk");
		REQUIRE(y->states[d4].num == 15);
		REQUIRE(y->states[g1].str == "dk");
		REQUIRE(y->states[g1].num == 16);

		WHEN("Running") {
			c.garbage_collector();

			THEN("Should erase Originator states that does not correspond to Timepoints located between the history root and either the current timepoint, any pinned or remembered timepoint") {
				REQUIRE(x->states.size() == 2);
				REQUIRE(x->states[a].str == "ac");
				REQUIRE(x->states[a].num == 56);
				REQUIRE(x->states[d2].str == "lo");
				REQUIRE(x->states[d2].num == 69);

				REQUIRE(y->states.size() == 6);
				REQUIRE(y->states[a].str == "lp");
				REQUIRE(y->states[a].num == 8);
				REQUIRE(y->states[c1].str == "dk");
				REQUIRE(y->states[c1].num == 12);
				REQUIRE(y->states[d3].str == "dk");
				REQUIRE(y->states[d3].num == 13);
				REQUIRE(y->states[f1].str == "dk");
				REQUIRE(y->states[f1].num == 14);
				REQUIRE(y->states[d4].str == "dk");
				REQUIRE(y->states[d4].num == 15);
				REQUIRE(y->states[g1].str == "dk");
				REQUIRE(y->states[g1].num == 16);
			}

			THEN("Should erase Timepoints that are not located between the history root and either the current timepoint, any pinned or remembered timepoint; even if annotated") {
				auto h = a->cluster(true);
				REQUIRE(h.size() == 9);
				REQUIRE(contains(h, a));
				REQUIRE(contains(h, b));
				REQUIRE(contains(h, c1));
				REQUIRE_FALSE(contains(h, c2));
				REQUIRE_FALSE(contains(h, d1));
				REQUIRE(contains(h, d2));
				REQUIRE(contains(h, d3));
				REQUIRE(contains(h, d4));
				REQUIRE(contains(h, e1));
				REQUIRE_FALSE(contains(h, e2));
				REQUIRE(contains(h, f1));
				REQUIRE(contains(h, g1));
			}

			THEN("Should erase Annotations of erased Timepoints") {
				REQUIRE(c.annotations.size() == 1);
				REQUIRE_FALSE(c.annotations.contains(e2));
				REQUIRE(static_cast<Annotation*>(c.annotations[f1].get())->value == 6);
			}

			THEN("Should unregister expired Originators") {
				REQUIRE(c.originators.size() == 2);
				REQUIRE(contains_originator(c.originators, x.get()));
				REQUIRE(contains_originator(c.originators, y.get()));
				REQUIRE_FALSE(contains_expired(c.originators));
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Caretaker::stop_browsing_user_history() noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker with some remembered timepoints") {
		Caretaker c;
		[[maybe_unused]] Timepoint* t0 = c.get_current_timepoint();
		[[maybe_unused]] Timepoint* t1 = c.make_next_timepoint();
		c.remember_current_timepoint();
		[[maybe_unused]] Timepoint* t2 = c.make_next_timepoint();
		c.remember_current_timepoint();
		[[maybe_unused]] Timepoint* t3 = c.make_next_timepoint();
		c.remember_current_timepoint();

		WHEN("Not browsing user history") {
			REQUIRE(c.user_history.size() == 4);
			REQUIRE(at(c.user_history, 0) == t0);
			REQUIRE(at(c.user_history, 1) == t1);
			REQUIRE(at(c.user_history, 2) == t2);
			REQUIRE(at(c.user_history, 3) == t3);
			REQUIRE(c.get_current_timepoint() == t3);

			THEN("Should not do anything") {
				c.stop_browsing_user_history();
				REQUIRE(c.user_history.size() == 4);
				REQUIRE(at(c.user_history, 0) == t0);
				REQUIRE(at(c.user_history, 1) == t1);
				REQUIRE(at(c.user_history, 2) == t2);
				REQUIRE(at(c.user_history, 3) == t3);
				REQUIRE(c.get_current_timepoint() == t3);
			}
		}

		WHEN("Browsing user history by undoing") {
			c.undo(2);
			REQUIRE(c.user_history.size() == 4);
			REQUIRE(at(c.user_history, 0) == t0);
			REQUIRE(at(c.user_history, 1) == t1);
			REQUIRE(at(c.user_history, 2) == t2);
			REQUIRE(at(c.user_history, 3) == t3);
			REQUIRE(c.get_current_timepoint() == t1);

			WHEN("Auto garbage collector is enabled") {
				c.set_auto_gc(true);
				c.stop_browsing_user_history();

				THEN("Should remove undoed timepoints from user history") {
					REQUIRE(c.user_history.size() == 2);
					REQUIRE(at(c.user_history, 0) == t0);
					REQUIRE(at(c.user_history, 1) == t1);
					REQUIRE(c.get_current_timepoint() == t1);
				}

				THEN("Should delete unreferenced Timepoints from history tree") {
					auto h = t0->cluster(true);
					REQUIRE(h.size() == 2);
					REQUIRE(contains(h, t0));
					REQUIRE(contains(h, t1));
					REQUIRE_FALSE(contains(h, t2));
					REQUIRE_FALSE(contains(h, t3));
				}
			}

			WHEN("Auto garbage collector is disabled") {
				c.set_auto_gc(false);
				c.stop_browsing_user_history();

				THEN("Should remove undoed timepoints from user history") {
					REQUIRE(c.user_history.size() == 2);
					REQUIRE(at(c.user_history, 0) == t0);
					REQUIRE(at(c.user_history, 1) == t1);
					REQUIRE(c.get_current_timepoint() == t1);
				}

				THEN("Should keep unreferenced Timepoints in history tree") {
					auto h = t0->cluster(true);
					REQUIRE(h.size() == 4);
					REQUIRE(contains(h, t0));
					REQUIRE(contains(h, t1));
					REQUIRE(contains(h, t2));
					REQUIRE(contains(h, t3));
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("Timepoint* Caretaker::get_history_root() noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker") {
		Caretaker c;

		THEN("Should return its history tree root") {
			Timepoint* t0 = c.get_history_root();
			REQUIRE(t0 != nullptr);

			AND_WHEN("Going to the next timepoint") {
				Timepoint* t1 = c.make_next_timepoint();

				THEN("Should still return the history tree root") {
					REQUIRE(c.get_history_root() == t0);
					REQUIRE(c.get_history_root() != t1);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("Timepoint* Caretaker::get_current_timepoint() noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker") {
		Caretaker c;

		THEN("Should return its history tree root") {
			Timepoint* t0 = c.get_history_root();
			REQUIRE(c.get_current_timepoint() == t0);

			AND_WHEN("Going to the next timepoint") {
				Timepoint* t1 = c.make_next_timepoint();

				THEN("Should return the next timepoint") {
					REQUIRE(c.get_current_timepoint() != t0);
					REQUIRE(c.get_current_timepoint() == t1);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("vector<Timepoint*> const& Caretaker::get_pinned_timepoints() const noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker with some pinned timepoints") {
		Caretaker c;
		[[maybe_unused]] Timepoint* t0 = c.get_current_timepoint();
		c.pin_current_timepoint();
		[[maybe_unused]] Timepoint* t1 = c.make_next_timepoint();
		[[maybe_unused]] Timepoint* t2 = c.make_next_timepoint();
		c.pin_current_timepoint();
		[[maybe_unused]] Timepoint* t3 = c.make_next_timepoint();
		c.pin_current_timepoint();
		[[maybe_unused]] Timepoint* t4 = c.make_next_timepoint();

		THEN("Should return pinned timepoints") {
			auto a = c.get_pinned_timepoints();
			REQUIRE(a.size() == 3);
			REQUIRE(contains(a, t0));
			REQUIRE(contains(a, t2));
			REQUIRE(contains(a, t3));
		}
	}
}

//******************************************************************************
SCENARIO("Timepoint* Caretaker::make_next_timepoint() noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker with some remembered timepoints") {
		Caretaker c;
		[[maybe_unused]] Timepoint* t0 = c.get_current_timepoint();
		[[maybe_unused]] Timepoint* t1 = c.make_next_timepoint();
		c.remember_current_timepoint();
		[[maybe_unused]] Timepoint* t2 = c.make_next_timepoint();
		c.remember_current_timepoint();
		[[maybe_unused]] Timepoint* t3 = c.make_next_timepoint();
		c.remember_current_timepoint();

		WHEN("Not browsing user history") {
			REQUIRE(c.user_history.size() == 4);
			REQUIRE(at(c.user_history, 0) == t0);
			REQUIRE(at(c.user_history, 1) == t1);
			REQUIRE(at(c.user_history, 2) == t2);
			REQUIRE(at(c.user_history, 3) == t3);
			REQUIRE(c.get_current_timepoint() == t3);
			REQUIRE_FALSE(c.can_redo());

			WHEN("Running") {
				Timepoint* t4 = c.make_next_timepoint();

				THEN("Should return a new Timepoint that is a child of the current Timepoint") {
					REQUIRE(t4->parent() == t3);

					AND_THEN("Current timepoint should be updated to this new Timepoint") {
						REQUIRE(c.get_current_timepoint() == t4);
					}
				}
			}
		}

		WHEN("Browsing user history") {
			c.undo(2);
			REQUIRE(c.user_history.size() == 4);
			REQUIRE(at(c.user_history, 0) == t0);
			REQUIRE(at(c.user_history, 1) == t1);
			REQUIRE(at(c.user_history, 2) == t2);
			REQUIRE(at(c.user_history, 3) == t3);
			REQUIRE(c.get_current_timepoint() == t1);
			REQUIRE(c.can_redo());

			WHEN("Running") {
				Timepoint* t4 = c.make_next_timepoint();

				THEN("Should return a new Timepoint that is a child of the current Timepoint") {
					REQUIRE(t4->parent() == t1);

					AND_THEN("Current timepoint should be updated to this new Timepoint") {
						REQUIRE(c.get_current_timepoint() == t4);
					}
				}

				THEN("Should remove undoed timepoints from user history") {
					REQUIRE_FALSE(c.can_redo());
					REQUIRE(c.user_history.size() == 2);
					REQUIRE(at(c.user_history, 0) == t0);
					REQUIRE(at(c.user_history, 1) == t1);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Caretaker::take_care_of(shared_ptr<IOriginator> const& originator) noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker") {
		Caretaker c;

		WHEN("Trying to take_care_of a null Originator") {
			c.take_care_of(nullptr);

			THEN("Should not do anything") {
				REQUIRE(c.originators.empty());
			}
		}

		WHEN("Trying to take_care_of an Originator affiliated with another Caretaker") {
			auto a = std::make_shared<Originator<StateA>>(c.get_history_root(), StateA { .str = "ac", .num = 56 });
			c.take_care_of(a);

			THEN("Should not do anything") {
				REQUIRE(c.originators.empty());
			}
		}

		WHEN("Trying to take_care_of an Originator which init timepoint is null") {
			auto a = std::make_shared<Originator<StateA>>(nullptr, StateA { .str = "ac", .num = 56 }, c);
			c.take_care_of(a);

			THEN("Should not do anything") {
				REQUIRE(c.originators.empty());
			}
		}

		WHEN("Trying to take_care_of an Originator which init timepoint is unrelated to the history tree") {
			Timepoint t;
			auto a = std::make_shared<Originator<StateA>>(&t, StateA { .str = "ac", .num = 56 }, c);
			c.take_care_of(a);

			THEN("Should not do anything") {
				REQUIRE(c.originators.empty());
			}
		}

		WHEN("Trying to take_care_of an Originator affiliated with this Caretaker and which init timepoint is part of the history tree") {
			auto a = std::make_shared<Originator<StateA>>(c.get_history_root(), StateA { .str = "ac", .num = 56 }, c);
			c.take_care_of(a);

			THEN("Should register it") {
				REQUIRE(c.originators.size() == 1);
				REQUIRE(c.originators.back().lock().get() == a.get());
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Caretaker::undo(size_t remembered_timepoints) noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker") {
		Caretaker c;

		WHEN("Can undo") {
			[[maybe_unused]] Timepoint* t0 = c.get_current_timepoint();
			[[maybe_unused]] Timepoint* t1 = c.make_next_timepoint();
			c.remember_current_timepoint();
			[[maybe_unused]] Timepoint* t2 = c.make_next_timepoint();
			c.remember_current_timepoint();
			[[maybe_unused]] Timepoint* t3 = c.make_next_timepoint();

			REQUIRE(c.can_undo());

			WHEN("Undoing 0 timepoints") {
				c.undo(0);

				THEN("Should not do anything") {
					REQUIRE(c.get_current_timepoint() == t3);
				}
			}

			WHEN("Current timepoint is a remembered timepoint") {
				c.remember_current_timepoint();
				REQUIRE(c.user_history.size() == 4);
				REQUIRE(at(c.user_history, 0) == t0);
				REQUIRE(at(c.user_history, 1) == t1);
				REQUIRE(at(c.user_history, 2) == t2);
				REQUIRE(at(c.user_history, 3) == t3);
				REQUIRE(c.get_current_timepoint() == t3);

				WHEN("Undoing one timepoint") {
					c.undo();

					THEN("Current timepoint should be the second last remembered timepoint") {
						REQUIRE(c.get_current_timepoint() == t2);
					}

					THEN("Redoing should be possible") {
						REQUIRE(c.can_redo());
					}

					AND_WHEN("Undoing some more timepoints") {
						c.undo(2);

						THEN("Current timepoint should be updated") {
							REQUIRE(c.get_current_timepoint() == t0);
						}

						THEN("Redoing should be possible") {
							REQUIRE(c.can_redo());
						}
					}
				}
			}

			WHEN("Current timepoint is not a remembered timepoint") {
				REQUIRE(c.user_history.size() == 3);
				REQUIRE(at(c.user_history, 0) == t0);
				REQUIRE(at(c.user_history, 1) == t1);
				REQUIRE(at(c.user_history, 2) == t2);
				REQUIRE(c.get_current_timepoint() == t3);

				WHEN("Undoing one timepoint") {
					c.undo();

					THEN("Current timepoint should be the last remembered timepoint") {
						REQUIRE(c.get_current_timepoint() == t2);
					}

					THEN("Redoing should not be possible") {
						REQUIRE_FALSE(c.can_redo());
					}

					AND_WHEN("Undoing some more timepoints") {
						c.undo(2);

						THEN("Current timepoint should be updated") {
							REQUIRE(c.get_current_timepoint() == t0);
						}

						THEN("Redoing should be possible") {
							REQUIRE(c.can_redo());
						}
					}
				}
			}
		}

		WHEN("Cannot undo") {
			REQUIRE(c.user_history.size() == 1);
			REQUIRE(at(c.user_history, 0) == c.get_history_root());
			REQUIRE(c.get_current_timepoint() == c.get_history_root());
			REQUIRE_FALSE(c.can_undo());

			THEN("Should not do anything") {
				c.undo();
				REQUIRE(c.get_current_timepoint() == c.get_history_root());
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Caretaker::redo(size_t remembered_timepoints) noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker") {
		Caretaker c;

		WHEN("Can redo") {
			[[maybe_unused]] Timepoint* t0 = c.get_current_timepoint();
			[[maybe_unused]] Timepoint* t1 = c.make_next_timepoint();
			c.remember_current_timepoint();
			[[maybe_unused]] Timepoint* t2 = c.make_next_timepoint();
			c.remember_current_timepoint();
			[[maybe_unused]] Timepoint* t3 = c.make_next_timepoint();
			c.remember_current_timepoint();
			c.undo(3);

			REQUIRE(c.can_redo());

			WHEN("Redoing 0 timepoints") {
				c.redo(0);

				THEN("Should not do anything") {
					REQUIRE(c.get_current_timepoint() == t0);
				}
			}

			WHEN("Redoing some but not all the redoable timepoints") {
				c.redo(2);

				THEN("Current timepoint should be updated") {
					REQUIRE(c.get_current_timepoint() == t2);
				}

				THEN("Redoing should be possible") {
					REQUIRE(c.can_redo());
				}

				AND_WHEN("Redoing the last redoable timepoint") {
					c.redo();

					THEN("Current timepoint should be the last remembered timepoint") {
						REQUIRE(c.get_current_timepoint() == c.user_history.back());
						REQUIRE(c.get_current_timepoint() == t3);
					}

					THEN("Redoing should not be possible") {
						REQUIRE_FALSE(c.can_redo());
					}
				}
			}
		}

		WHEN("Cannot redo") {
			REQUIRE(c.get_current_timepoint() == c.get_history_root());
			REQUIRE(c.get_current_timepoint() == c.user_history.back());
			REQUIRE_FALSE(c.can_redo());

			THEN("Should not do anything") {
				c.redo();
				REQUIRE(c.get_current_timepoint() == c.get_history_root());
			}
		}
	}
}

//******************************************************************************
SCENARIO("bool Caretaker::can_undo(size_t remembered_timepoints) const noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker with history root as current timepoint and only remembered timepoint") {
		Caretaker c;

		THEN("Should return false") {
			REQUIRE(at(c.user_history, 0) == c.get_current_timepoint());
			REQUIRE(at(c.user_history, 0) == c.get_history_root());
			REQUIRE_FALSE(c.can_undo());
		}
	}

	GIVEN("A Caretaker with some remembered timepoints") {
		Caretaker c;
		[[maybe_unused]] Timepoint* t0 = c.get_current_timepoint();
		[[maybe_unused]] Timepoint* t1 = c.make_next_timepoint();
		c.remember_current_timepoint();
		[[maybe_unused]] Timepoint* t2 = c.make_next_timepoint();
		c.remember_current_timepoint();
		[[maybe_unused]] Timepoint* t3 = c.make_next_timepoint();
		c.remember_current_timepoint();

		WHEN("Not browsing user history") {
			REQUIRE(c.user_history.size() == 4);
			REQUIRE(at(c.user_history, 0) == t0);
			REQUIRE(at(c.user_history, 1) == t1);
			REQUIRE(at(c.user_history, 2) == t2);
			REQUIRE(at(c.user_history, 3) == t3);
			REQUIRE(c.get_current_timepoint() == t3);

			WHEN("Running about 0 timepoints") {
				THEN("Should return false") {
					REQUIRE_FALSE(c.can_undo(0));
				}
			}

			WHEN("Running about less or the exact number of timepoints available") {
				THEN("Should return true") {
					REQUIRE(c.can_undo());
					REQUIRE(c.can_undo(1));
					REQUIRE(c.can_undo(2));
					REQUIRE(c.can_undo(3));
				}
			}

			WHEN("Running about more timepoints than available") {
				THEN("Should return false") {
					REQUIRE_FALSE(c.can_undo(4));
				}
			}
		}

		WHEN("Browsing user history by undoing to the history root") {
			c.undo(3);
			REQUIRE(c.user_history.size() == 4);
			REQUIRE(at(c.user_history, 0) == t0);
			REQUIRE(at(c.user_history, 1) == t1);
			REQUIRE(at(c.user_history, 2) == t2);
			REQUIRE(at(c.user_history, 3) == t3);
			REQUIRE(c.get_current_timepoint() == t0);

			THEN("Should return false") {
				REQUIRE_FALSE(c.can_undo());
				REQUIRE_FALSE(c.can_undo(0));
				REQUIRE_FALSE(c.can_undo(1));
				REQUIRE_FALSE(c.can_undo(2));
			}

			AND_WHEN("Redoing some but not all of the undoed timepoints") {
				c.redo(2);
				REQUIRE(c.user_history.size() == 4);
				REQUIRE(at(c.user_history, 0) == t0);
				REQUIRE(at(c.user_history, 1) == t1);
				REQUIRE(at(c.user_history, 2) == t2);
				REQUIRE(at(c.user_history, 3) == t3);
				REQUIRE(c.get_current_timepoint() == t2);

				WHEN("Running about 0 timepoints") {
					THEN("Should return false") {
						REQUIRE_FALSE(c.can_undo(0));
					}
				}

				WHEN("Running about less or the exact number of timepoints available") {
					THEN("Should return true") {
						REQUIRE(c.can_undo());
						REQUIRE(c.can_undo(1));
						REQUIRE(c.can_undo(2));
					}
				}

				WHEN("Running about more timepoints than available") {
					THEN("Should return false") {
						REQUIRE_FALSE(c.can_undo(3));
					}
				}
			}
		}

		WHEN("Browsing user history by undoing to an intermediate timepoint") {
			c.undo(2);
			REQUIRE(c.user_history.size() == 4);
			REQUIRE(at(c.user_history, 0) == t0);
			REQUIRE(at(c.user_history, 1) == t1);
			REQUIRE(at(c.user_history, 2) == t2);
			REQUIRE(at(c.user_history, 3) == t3);
			REQUIRE(c.get_current_timepoint() == t1);

			WHEN("Running about 0 timepoints") {
				THEN("Should return false") {
					REQUIRE_FALSE(c.can_undo(0));
				}
			}

			WHEN("Running about less or the exact number of timepoints available") {
				THEN("Should return true") {
					REQUIRE(c.can_undo());
					REQUIRE(c.can_undo(1));
				}
			}

			WHEN("Running about more timepoints than available") {
				THEN("Should return false") {
					REQUIRE_FALSE(c.can_undo(2));
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("bool Caretaker::can_redo(size_t remembered_timepoints) const noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker with some remembered timepoints") {
		Caretaker c;
		[[maybe_unused]] Timepoint* t0 = c.get_current_timepoint();
		[[maybe_unused]] Timepoint* t1 = c.make_next_timepoint();
		c.remember_current_timepoint();
		[[maybe_unused]] Timepoint* t2 = c.make_next_timepoint();
		c.remember_current_timepoint();
		[[maybe_unused]] Timepoint* t3 = c.make_next_timepoint();
		c.remember_current_timepoint();

		WHEN("Not browsing user history") {
			REQUIRE(c.user_history.size() == 4);
			REQUIRE(at(c.user_history, 0) == t0);
			REQUIRE(at(c.user_history, 1) == t1);
			REQUIRE(at(c.user_history, 2) == t2);
			REQUIRE(at(c.user_history, 3) == t3);
			REQUIRE(c.get_current_timepoint() == t3);

			THEN("Should return false") {
				REQUIRE_FALSE(c.can_redo());
				REQUIRE_FALSE(c.can_redo(0));
				REQUIRE_FALSE(c.can_redo(1));
				REQUIRE_FALSE(c.can_redo(2));
			}
		}

		WHEN("Browsing user history by undoing") {
			c.undo(2);
			REQUIRE(c.user_history.size() == 4);
			REQUIRE(at(c.user_history, 0) == t0);
			REQUIRE(at(c.user_history, 1) == t1);
			REQUIRE(at(c.user_history, 2) == t2);
			REQUIRE(at(c.user_history, 3) == t3);
			REQUIRE(c.get_current_timepoint() == t1);

			WHEN("Running about 0 timepoints") {
				THEN("Should return false") {
					REQUIRE_FALSE(c.can_redo(0));
				}
			}

			WHEN("Running about less or the exact number of timepoints available") {
				THEN("Should return true") {
					REQUIRE(c.can_redo());
					REQUIRE(c.can_redo(1));
					REQUIRE(c.can_redo(2));
				}
			}

			WHEN("Running about more timepoints than available") {
				THEN("Should return false") {
					REQUIRE_FALSE(c.can_redo(3));
				}
			}

			AND_WHEN("Stopping browsing user history by creating a next timepoint") {
				Timepoint* t4 = c.make_next_timepoint();
				REQUIRE(c.user_history.size() == 2);
				REQUIRE(at(c.user_history, 0) == t0);
				REQUIRE(at(c.user_history, 1) == t1);
				REQUIRE(c.get_current_timepoint() == t4);

				THEN("Should return false") {
					REQUIRE_FALSE(c.can_redo());
					REQUIRE_FALSE(c.can_redo(0));
					REQUIRE_FALSE(c.can_redo(1));
					REQUIRE_FALSE(c.can_redo(2));
				}
			}

			AND_WHEN("Stopping browsing user history by redoing all undoed timepoints") {
				c.redo();
				REQUIRE(c.can_redo());
				c.redo();
				REQUIRE(c.user_history.size() == 4);
				REQUIRE(at(c.user_history, 0) == t0);
				REQUIRE(at(c.user_history, 1) == t1);
				REQUIRE(at(c.user_history, 2) == t2);
				REQUIRE(at(c.user_history, 3) == t3);
				REQUIRE(c.get_current_timepoint() == t3);

				THEN("Should return false") {
					REQUIRE_FALSE(c.can_redo());
					REQUIRE_FALSE(c.can_redo(0));
					REQUIRE_FALSE(c.can_redo(1));
					REQUIRE_FALSE(c.can_redo(2));
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Caretaker::unpin(Timepoint* t) noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker with some pinned timepoints") {
		Caretaker c;
		[[maybe_unused]] Timepoint* t0 = c.get_current_timepoint();
		[[maybe_unused]] Timepoint* t1 = c.make_next_timepoint();
		c.pin_current_timepoint();
		[[maybe_unused]] Timepoint* t2 = c.make_next_timepoint();
		c.pin_current_timepoint();
		[[maybe_unused]] Timepoint* t3 = c.make_next_timepoint();
		c.go_without_remembering(t0);

		REQUIRE(c.user_history.size() == 1);
		REQUIRE(at(c.user_history, 0) == t0);
		REQUIRE(c.pinned_timepoints.size() == 2);
		REQUIRE(contains(c.pinned_timepoints, t1));
		REQUIRE(contains(c.pinned_timepoints, t2));
		REQUIRE(c.get_current_timepoint() == t0);

		WHEN("Auto garbage collector is enabled") {
			c.set_auto_gc(true);
			c.unpin(t1);

			THEN("Should remove unpined timepoint from pinned timepoints") {
				REQUIRE(c.pinned_timepoints.size() == 1);
				REQUIRE_FALSE(contains(c.pinned_timepoints, t1));
				REQUIRE(contains(c.pinned_timepoints, t2));
				REQUIRE(c.get_current_timepoint() == t0);
			}

			THEN("Should delete unreferenced Timepoints from history tree") {
				auto h = t0->cluster(true);
				REQUIRE(h.size() == 3);
				REQUIRE(contains(h, t0));
				REQUIRE(contains(h, t1)); // Ancestor of pinned timepoint
				REQUIRE(contains(h, t2));
				REQUIRE_FALSE(contains(h, t3));
				REQUIRE(c.user_history.size() == 1);
				REQUIRE(at(c.user_history, 0) == t0);
			}
		}

		WHEN("Auto garbage collector is disabled") {
			c.set_auto_gc(false);
			c.unpin(t1);

			THEN("Should remove unpined timepoint from pinned timepoints") {
				REQUIRE(c.pinned_timepoints.size() == 1);
				REQUIRE_FALSE(contains(c.pinned_timepoints, t1));
				REQUIRE(contains(c.pinned_timepoints, t2));
				REQUIRE(c.get_current_timepoint() == t0);
			}

			THEN("Should keep unreferenced Timepoints in history tree") {
				auto h = t0->cluster(true);
				REQUIRE(h.size() == 4);
				REQUIRE(contains(h, t0));
				REQUIRE(contains(h, t1));
				REQUIRE(contains(h, t2));
				REQUIRE(contains(h, t3));
				REQUIRE(c.user_history.size() == 1);
				REQUIRE(at(c.user_history, 0) == t0);
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Caretaker::pin_current_timepoint() noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker") {
		Caretaker c;
		Timepoint* t0 = c.get_current_timepoint();

		THEN("No timepoint should be pinned but root history should be in user history") {
			REQUIRE(c.pinned_timepoints.empty());
			REQUIRE(c.user_history.size() == 1);
			REQUIRE(at(c.user_history, 0) == t0);
		}

		WHEN("Trying to pin current timepoint") {
			c.pin_current_timepoint();

			THEN("Current timepoint should be pinned and not appended again to user history") {
				REQUIRE(c.pinned_timepoints.size() == 1);
				REQUIRE(contains(c.pinned_timepoints, t0));
				REQUIRE(c.user_history.size() == 1);
				REQUIRE(at(c.user_history, 0) == t0);
			}

			AND_WHEN("Trying to pin current timepoint again") {
				c.pin_current_timepoint();

				THEN("Should not do anything") {
					REQUIRE(c.pinned_timepoints.size() == 1);
					REQUIRE(contains(c.pinned_timepoints, t0));
					REQUIRE(c.user_history.size() == 1);
					REQUIRE(at(c.user_history, 0) == t0);
				}
			}

			AND_WHEN("Trying to pin current timepoint after going to the next timepoint") {
				Timepoint* t1 = c.make_next_timepoint();
				c.pin_current_timepoint();

				THEN("Next timepoint should be pinned nor appended to user history") {
					REQUIRE(c.pinned_timepoints.size() == 2);
					REQUIRE(contains(c.pinned_timepoints, t0));
					REQUIRE(contains(c.pinned_timepoints, t1));
					REQUIRE(c.user_history.size() == 1);
					REQUIRE(at(c.user_history, 0) == t0);
				}

				AND_WHEN("Trying to pin current timepoint after going_without_remembering to the previous timepoint") {
					c.go_without_remembering(t0);
					c.pin_current_timepoint();

					THEN("Previous timepoint should not be added again to pinned timepoints nor appended to user history") {
						REQUIRE(c.pinned_timepoints.size() == 2);
						REQUIRE(contains(c.pinned_timepoints, t0));
						REQUIRE(contains(c.pinned_timepoints, t1));
						REQUIRE(at(c.user_history, 0) == t0);
					}
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Caretaker::remember_current_timepoint() noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker") {
		Caretaker c;
		Timepoint* t0 = c.get_current_timepoint();

		THEN("Root history should be in user history") {
			REQUIRE(c.user_history.size() == 1);
			REQUIRE(at(c.user_history, 0) == t0);
		}

		WHEN("Trying to remember current timepoint") {
			c.remember_current_timepoint();

			THEN("Current timepoint should not be appended again to user history") {
				REQUIRE(c.user_history.size() == 1);
				REQUIRE(at(c.user_history, 0) == t0);
			}

			AND_WHEN("Trying to remember current timepoint after going to the next timepoint") {
				Timepoint* t1 = c.make_next_timepoint();
				c.remember_current_timepoint();

				THEN("Next timepoint should be appended to user history") {
					REQUIRE(c.user_history.size() == 2);
					REQUIRE(at(c.user_history, 0) == t0);
					REQUIRE(at(c.user_history, 1) == t1);
				}

				AND_WHEN("Trying to remember current timepoint after going_without_remembering to the previous timepoint") {
					c.go_without_remembering(t0);
					c.remember_current_timepoint();

					THEN("Previous timepoint should be appended to user history") {
						REQUIRE(c.user_history.size() == 3);
						REQUIRE(at(c.user_history, 0) == t0);
						REQUIRE(at(c.user_history, 1) == t1);
						REQUIRE(at(c.user_history, 2) == t0);
					}
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("bool Caretaker::go_without_remembering(Timepoint* t) noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker with an history tree and managing some Originators") {
		// + a <---------- root, init
		//   + b
		//     + c1 <----- y1
		//     | + d1 <--- go
		//     | + d2 <--- x1
		//     | | + e1
		//     | | + e2
		//     | + d3
		//     + c2 <----- z1

		Caretaker c;
		[[maybe_unused]] Timepoint* a = c.get_history_root();
		[[maybe_unused]] Timepoint* b = &a->add_child();
		[[maybe_unused]] Timepoint* c1 = &b->add_child();
		[[maybe_unused]] Timepoint* c2 = &b->add_child();
		[[maybe_unused]] Timepoint* d1 = &c1->add_child();
		[[maybe_unused]] Timepoint* d2 = &c1->add_child();
		[[maybe_unused]] Timepoint* d3 = &c1->add_child();
		[[maybe_unused]] Timepoint* e1 = &d2->add_child();
		[[maybe_unused]] Timepoint* e2 = &d2->add_child();

		auto x = std::make_shared<Originator<StateA>>(a, StateA { .str = "ac", .num = 56 }, c);
		auto y = std::make_shared<Originator<StateA>>(a, StateA { .str = "lp", .num = 8 }, c);
		auto z = std::make_shared<Originator<StateA>>(a, StateA { .str = "gh", .num = 444 }, c);
		x->set_state(d2, { .str = "lo", .num = 69 });
		y->set_state(c1, { .str = "dk", .num = 12 });
		z->set_state(c2, { .str = "ws", .num = 6 });
		c.take_care_of(x);
		c.take_care_of(y);
		c.take_care_of(z);
		z.reset();

		WHEN("Trying to go to a Timepoint that is part of the history tree") {
			bool r = c.go_without_remembering(d1);

			THEN("Should update current timepoint and return true") {
				REQUIRE(r);
				REQUIRE(c.get_current_timepoint() == d1);
			}

			THEN("Current timepoint should be propagated to valid Originators") {
				AND_THEN("Originators current timepoint should be the given Timepoint or its first known parent or Originator init timepoint") {
					REQUIRE(x->get_current_timepoint() == a);
					REQUIRE(y->get_current_timepoint() == c1);
				}
			}
		}

		WHEN("Trying to go to a Timepoint that is not part of the history tree") {
			Timepoint f;
			Timepoint* t = c.get_current_timepoint();
			bool r = c.go_without_remembering(&f);

			THEN("Should not do anything and return false") {
				REQUIRE_FALSE(r);
				REQUIRE(c.get_current_timepoint() == t);
				REQUIRE(x->get_current_timepoint() == d2);
				REQUIRE(y->get_current_timepoint() == c1);

			}
		}

		WHEN("Trying to go to a null Timepoint") {
			Timepoint* t = c.get_current_timepoint();
			bool r = c.go_without_remembering(nullptr);

			THEN("Should not do anything and return false") {
				REQUIRE_FALSE(r);
				REQUIRE(c.get_current_timepoint() == t);
				REQUIRE(x->get_current_timepoint() == d2);
				REQUIRE(y->get_current_timepoint() == c1);
			}
		}
	}
}

//******************************************************************************
SCENARIO("bool Caretaker::go_and_remember(Timepoint* t) noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker with an history tree and managing some Originators") {
		// + a <---------- root, init
		//   + b
		//     + c1 <----- y1
		//     | + d1 <--- go
		//     | + d2 <--- x1
		//     | | + e1
		//     | | + e2
		//     | + d3
		//     + c2 <----- z1

		Caretaker c;
		[[maybe_unused]] Timepoint* a = c.get_history_root();
		[[maybe_unused]] Timepoint* b = &a->add_child();
		[[maybe_unused]] Timepoint* c1 = &b->add_child();
		[[maybe_unused]] Timepoint* c2 = &b->add_child();
		[[maybe_unused]] Timepoint* d1 = &c1->add_child();
		[[maybe_unused]] Timepoint* d2 = &c1->add_child();
		[[maybe_unused]] Timepoint* d3 = &c1->add_child();
		[[maybe_unused]] Timepoint* e1 = &d2->add_child();
		[[maybe_unused]] Timepoint* e2 = &d2->add_child();

		auto x = std::make_shared<Originator<StateA>>(a, StateA { .str = "ac", .num = 56 }, c);
		auto y = std::make_shared<Originator<StateA>>(a, StateA { .str = "lp", .num = 8 }, c);
		auto z = std::make_shared<Originator<StateA>>(a, StateA { .str = "gh", .num = 444 }, c);
		x->set_state(d2, { .str = "lo", .num = 69 });
		y->set_state(c1, { .str = "dk", .num = 12 });
		z->set_state(c2, { .str = "ws", .num = 6 });
		c.take_care_of(x);
		c.take_care_of(y);
		c.take_care_of(z);
		z.reset();

		THEN("Root history should be in user history") {
			REQUIRE(c.user_history.size() == 1);
			REQUIRE(at(c.user_history, 0) == a);
		}

		WHEN("Trying to go to a Timepoint that is part of the history tree") {
			bool r = c.go_and_remember(d1);

			THEN("Should update current timepoint, user history and return true") {
				REQUIRE(r);
				REQUIRE(c.get_current_timepoint() == d1);
				REQUIRE(c.user_history.size() == 2);
				REQUIRE(at(c.user_history, 0) == a);
				REQUIRE(at(c.user_history, 1) == d1);
			}

			THEN("Current timepoint should be propagated to valid Originators") {
				AND_THEN("Originators current timepoint should be the given Timepoint or its first known parent or Originator init timepoint") {
					REQUIRE(x->get_current_timepoint() == a);
					REQUIRE(y->get_current_timepoint() == c1);
				}
			}
		}

		WHEN("Trying to go to a Timepoint that is not part of the history tree") {
			Timepoint f;
			Timepoint* t = c.get_current_timepoint();
			bool r = c.go_and_remember(&f);

			THEN("Should not do anything and return false") {
				REQUIRE_FALSE(r);
				REQUIRE(c.get_current_timepoint() == t);
				REQUIRE(x->get_current_timepoint() == d2);
				REQUIRE(y->get_current_timepoint() == c1);
				REQUIRE(c.user_history.size() == 1);
				REQUIRE(at(c.user_history, 0) == a);
			}
		}

		WHEN("Trying to go to a null Timepoint") {
			Timepoint* t = c.get_current_timepoint();
			bool r = c.go_and_remember(nullptr);

			THEN("Should not do anything and return false") {
				REQUIRE_FALSE(r);
				REQUIRE(c.get_current_timepoint() == t);
				REQUIRE(x->get_current_timepoint() == d2);
				REQUIRE(y->get_current_timepoint() == c1);
				REQUIRE(c.user_history.size() == 1);
				REQUIRE(at(c.user_history, 0) == a);
			}
		}
	}
}

//******************************************************************************
SCENARIO("bool Caretaker::get_auto_gc() const noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker") {
		Caretaker c;

		WHEN("Auto garbage collector is enabled") {
			c.auto_gc = true;

			THEN("Should return true") {
				REQUIRE(c.get_auto_gc());
			}
		}

		WHEN("Auto garbage collector is disabled") {
			c.auto_gc = false;

			THEN("Should return false") {
				REQUIRE_FALSE(c.get_auto_gc());
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Caretaker::set_auto_gc(bool _auto_gc) noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker") {
		Caretaker c;

		WHEN("Setting true") {
			c.set_auto_gc(true);

			THEN("Auto garbage collector should be enabled") {
				REQUIRE(c.auto_gc);
			}
		}

		WHEN("Setting false") {
			c.set_auto_gc(false);

			THEN("Auto garbage collector should be disabled") {
				REQUIRE_FALSE(c.auto_gc);
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Caretaker::annotate_current_timepoint(std::unique_ptr<IAnnotation> annotation) noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker") {
		Caretaker c;
		Timepoint* t0 = c.get_history_root();
		Timepoint* t1 = &t0->add_child();
		c.go_without_remembering(t1);

		THEN("No timepoint should be annotated") {
			REQUIRE(c.annotations.empty());
		}

		WHEN("Trying to annotate current timepoint") {
			c.annotate_current_timepoint(std::make_unique<Annotation>(5));

			THEN("Current timepoint should be annotated") {
				REQUIRE(c.annotations.size() == 1);
				REQUIRE(c.annotations.contains(t1));
				REQUIRE(static_cast<Annotation*>(c.annotations.at(t1).get())->value == 5);
			}

			THEN("Current timepoint should not be pinned nor appended to user history") {
				REQUIRE(c.pinned_timepoints.empty());
				REQUIRE(c.user_history.size() == 1);
				REQUIRE(at(c.user_history, 0) == c.history_root.get());
			}

			AND_WHEN("Trying to annotate current timepoint again") {
				c.annotate_current_timepoint(std::make_unique<Annotation>(7));

				THEN("Annotation should be updated") {
					REQUIRE(c.annotations.size() == 1);
					REQUIRE(c.annotations.contains(t1));
					REQUIRE(static_cast<Annotation*>(c.annotations.at(t1).get())->value == 5);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("IAnnotation* Caretaker::get_annotation(Timepoint* t) noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker with some annotated timepoints") {
		Caretaker c;
		[[maybe_unused]] Timepoint* t0 = c.get_current_timepoint();
		[[maybe_unused]] Timepoint* t1 = c.make_next_timepoint();
		c.annotate_current_timepoint(std::make_unique<Annotation>(5));
		[[maybe_unused]] Timepoint* t2 = c.make_next_timepoint();
		c.annotate_current_timepoint(std::make_unique<Annotation>(6));
		[[maybe_unused]] Timepoint* t3 = c.make_next_timepoint();
		[[maybe_unused]] Timepoint t4;

		WHEN("Running for an annotated timepoint") {
			THEN("Should return its annotation") {
				REQUIRE(static_cast<Annotation*>(c.get_annotation(t1))->value == 5);
				REQUIRE(static_cast<Annotation*>(c.get_annotation(t2))->value == 6);
			}
		}

		WHEN("Running for an unannotated timepoint") {
			THEN("Should return nullptr") {
				REQUIRE(c.get_annotation(t0) == nullptr);
				REQUIRE(c.get_annotation(t3) == nullptr);
			}
		}

		WHEN("Running for an unrelated timepoint") {
			THEN("Should return nullptr") {
				REQUIRE(c.get_annotation(&t4) == nullptr);
			}
		}

		WHEN("Running for nullptr") {
			THEN("Should return nullptr") {
				REQUIRE(c.get_annotation(nullptr) == nullptr);
			}
		}
	}
}

//******************************************************************************
SCENARIO("Timepoint* Caretaker::find_first_ancestor_with_annotation_that(std::function<bool (IAnnotation const*)> const& predicate) noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker") {
		// + a <------------ root
		//   + b <---------- 0
		//     + c1
		//     | + d1 <----- 2
		//     | + d2 <----- 1
		//     | | + e1 <--- 3
		//     | | + e2 <--- 5 current
		//     | + d3
		//     + c2 <------- 4

		Caretaker c;
		[[maybe_unused]] Timepoint* a = c.get_history_root();
		[[maybe_unused]] Timepoint* b = &a->add_child();
		[[maybe_unused]] Timepoint* c1 = &b->add_child();
		[[maybe_unused]] Timepoint* c2 = &b->add_child();
		[[maybe_unused]] Timepoint* d1 = &c1->add_child();
		[[maybe_unused]] Timepoint* d2 = &c1->add_child();
		[[maybe_unused]] Timepoint* d3 = &c1->add_child();
		[[maybe_unused]] Timepoint* e1 = &d2->add_child();
		[[maybe_unused]] Timepoint* e2 = &d2->add_child();

		WHEN("There is some annotated timepoints, including current timepoint, in the history") {
			c.go_without_remembering(b); c.annotate_current_timepoint(std::make_unique<Annotation>(0));
			c.go_without_remembering(d2); c.annotate_current_timepoint(std::make_unique<Annotation>(1));
			c.go_without_remembering(d1); c.annotate_current_timepoint(std::make_unique<Annotation>(2));
			c.go_without_remembering(e1); c.annotate_current_timepoint(std::make_unique<Annotation>(3));
			c.go_without_remembering(c2); c.annotate_current_timepoint(std::make_unique<Annotation>(4));
			c.go_without_remembering(e2); c.annotate_current_timepoint(std::make_unique<Annotation>(5));

			WHEN("Looking for a predicate that match the current timepoint and an older one") {
				auto const pred = [](IAnnotation const* annotation) {
					auto* a = static_cast<Annotation const*>(annotation);
					return a->value == 0
					    || a->value == 5;
				};

				AND_WHEN("including_itself") {
					THEN("Should return current timepoint") {
						REQUIRE(c.current_timepoint == e2);
						REQUIRE(c.find_first_ancestor_with_annotation_that(pred, true) == e2);
					}
				}

				AND_WHEN("not including_itself") {
					THEN("Should return the first annotated ancestor timepoint of the current state") {
						REQUIRE(c.current_timepoint == e2);
						REQUIRE(c.find_first_ancestor_with_annotation_that(pred, false) == b);
					}
				}
			}

			WHEN("Looking for a predicate that match no timepoint in the history") {
				THEN("Should return nullptr") {
					REQUIRE(c.find_first_ancestor_with_annotation_that([](auto) { return false; }) == nullptr);
				}
			}
		}

		WHEN("No ancestor is annotated") {
			REQUIRE(c.annotations.empty());

			THEN("Should return nullptr") {
				REQUIRE(c.find_first_ancestor_with_annotation_that([](auto) { return true; }) == nullptr);
			}
		}
	}
}

//******************************************************************************
SCENARIO("Timepoint* Caretaker::find_first_ancestor_with_annotation() noexcept", "[utils][state_management]") {
	GIVEN("A Caretaker") {
		// + a <------------ root
		//   + b <---------- 0
		//     + c1
		//     | + d1 <----- 2
		//     | + d2 <----- 1
		//     | | + e1 <--- 3
		//     | | + e2 <--- 5 current
		//     | + d3
		//     + c2 <------- 4

		Caretaker c;
		[[maybe_unused]] Timepoint* a = c.get_history_root();
		[[maybe_unused]] Timepoint* b = &a->add_child();
		[[maybe_unused]] Timepoint* c1 = &b->add_child();
		[[maybe_unused]] Timepoint* c2 = &b->add_child();
		[[maybe_unused]] Timepoint* d1 = &c1->add_child();
		[[maybe_unused]] Timepoint* d2 = &c1->add_child();
		[[maybe_unused]] Timepoint* d3 = &c1->add_child();
		[[maybe_unused]] Timepoint* e1 = &d2->add_child();
		[[maybe_unused]] Timepoint* e2 = &d2->add_child();

		WHEN("There is some annotated timepoints, including current timepoint, in the history") {
			c.go_without_remembering(b); c.annotate_current_timepoint(std::make_unique<Annotation>(0));
			c.go_without_remembering(d2); c.annotate_current_timepoint(std::make_unique<Annotation>(1));
			c.go_without_remembering(d1); c.annotate_current_timepoint(std::make_unique<Annotation>(2));
			c.go_without_remembering(e1); c.annotate_current_timepoint(std::make_unique<Annotation>(3));
			c.go_without_remembering(c2); c.annotate_current_timepoint(std::make_unique<Annotation>(4));
			c.go_without_remembering(e2); c.annotate_current_timepoint(std::make_unique<Annotation>(5));

			AND_WHEN("including_itself") {
				THEN("Should return current timepoint") {
					REQUIRE(c.current_timepoint == e2);
					REQUIRE(c.find_first_ancestor_with_annotation(true) == e2);
				}
			}

			AND_WHEN("not including_itself") {
				THEN("Should return the first annotated ancestor timepoint of the current state") {
					REQUIRE(c.current_timepoint == e2);
					REQUIRE(c.find_first_ancestor_with_annotation(false) == d2);
				}
			}
		}

		WHEN("No ancestor is annotated") {
			REQUIRE(c.annotations.empty());

			THEN("Should return nullptr") {
				REQUIRE(c.find_first_ancestor_with_annotation() == nullptr);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////

//******************************************************************************
SCENARIO("Real case Originator + Caretaker usage", "[utils][state_management]") {
}
