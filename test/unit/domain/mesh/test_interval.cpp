///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include <vector>

#include "domain/mesh/meshline.hpp"
#include "domain/mesh/meshline_policy.hpp"

#include "domain/mesh/interval.hpp"

/// @test Interval::Side::Side(MeshlinePolicy* meshline_policy, size_t lmin, double smoothness, Coord h, function<double (double)> d_init)
/// @test double Interval::Side::d_init_(double d)
/// @test Coord Interval::s(Interval::Side const& side) const @todo
/// @test Coord Interval::s(Interval::Side const& side, double d) const @todo
/// @test std::vector<Coord> find_ls(double d, double smoothness, double dmax, Coord s)
/// @test bool is_ls_valid_for_dmax_lmin_smoothness(std::vector<Coord> const& ls, double d, double smoothness, double dmax, size_t lmin)
/// @test void Interval::update_ls() @todo
/// @test void Interval::update_ls(Interval::Side& side) @todo
/// @test double find_dmax(Interval::Side const& side, double dmax)
/// @test double find_dmax(Interval::Side const& side, Interval::Side const& b, double dmax)
/// @test std::tuple<double, bool> Interval::adjust_d_for_dmax_lmin(Interval::Side const& side, size_t iter_limit) const
/// @test std::tuple<double, bool> Interval::adjust_smoothness_for_s(Interval::Side const& side, size_t iter_limit) const
/// @test void Interval::auto_solve_d() @todo
/// @test void Interval::auto_solve_smoothness() @todo
/// @test std::vector<std::unique_ptr<Meshline>> Interval::mesh() const
///*****************************************************************************

using namespace domain;

//******************************************************************************
SCENARIO("Interval::Side::Side(MeshlinePolicy* meshline_policy, size_t lmin, double smoothness, Coord h, function<double (double)> d_init)", "[interval]") {
	Timepoint* t = Caretaker::singleton().get_history_root();
	GIVEN("Two meshline policies") {
		GlobalParams p(t);
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::ONELINE,
			MeshlinePolicy::Normal::NONE,
			&p,
			10,
			t);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::ONELINE,
			MeshlinePolicy::Normal::NONE,
			&p,
			20,
			t);
		auto state_a = a.get_current_state();
		auto state_b = b.get_current_state();
		state_a.d = 2;
		state_b.d = 15;
		a.set_next_state(state_a);
		b.set_next_state(state_b);
		WHEN("Creating an Interval's sides") {
			Interval i(&a, &b, Y, &p, t);
			THEN("d should be limited to the half of the distance between both policies") {
				REQUIRE(a.get_current_state().d == 2);
				REQUIRE(b.get_current_state().d == 5);
			}
		}
	}
}

//******************************************************************************
SCENARIO("double Interval::Side::d_init_(double d)", "[interval]") {
	Timepoint* t = Caretaker::singleton().get_history_root();
	GlobalParams p(t);
	double d = 1;

	GIVEN("An Interval between two MeshlinePolicy") {
		WHEN("Meshline policies are ONELINE") {
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::ONELINE,
				MeshlinePolicy::Normal::NONE,
				&p,
				10,
				t);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::ONELINE,
				MeshlinePolicy::Normal::NONE,
				&p,
				20,
				t);
			Interval i(&a, &b, Y, &p, t);
			THEN("d_init should be 0") {
				REQUIRE(i.get_current_state().before.d_init_(d) == 0);
				REQUIRE(i.get_current_state().after.d_init_(d) == 0);
			}
		}

		WHEN("Meshline policies are HALFS") {
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				&p,
				10,
				t);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				&p,
				20,
				t);
			Interval i(&a, &b, Y, &p, t);
			THEN("d_init should be d/2") {
				REQUIRE(i.get_current_state().before.d_init_(d) == d/2);
				REQUIRE(i.get_current_state().after.d_init_(d) == d/2);
			}
		}

		WHEN("Meshline policies are THIRDS") {
			AND_WHEN("Meshline Normal are MIN") {
				MeshlinePolicy a(
					Y,
					MeshlinePolicy::Policy::THIRDS,
					MeshlinePolicy::Normal::MIN,
					&p,
					10,
					t);
				MeshlinePolicy b(
					Y,
					MeshlinePolicy::Policy::THIRDS,
					MeshlinePolicy::Normal::MIN,
					&p,
					20,
					t);
				Interval i(&a, &b, Y, &p, t);
				THEN("d_init should be d/3 inside and 2d/3 outside") {
					REQUIRE(i.get_current_state().before.d_init_(d) == 1.0/3.0 * d);
					REQUIRE(i.get_current_state().after.d_init_(d) == 2.0/3.0 * d);
				}
			}

			AND_WHEN("Meshline Normal are MAX") {
				MeshlinePolicy a(
					Y,
					MeshlinePolicy::Policy::THIRDS,
					MeshlinePolicy::Normal::MAX,
					&p,
					10,
					t);
				MeshlinePolicy b(
					Y,
					MeshlinePolicy::Policy::THIRDS,
					MeshlinePolicy::Normal::MAX,
					&p,
					20,
					t);
				Interval i(&a, &b, Y, &p, t);
				THEN("d_init should be d/3 inside and 2d/3 outside") {
					REQUIRE(i.get_current_state().before.d_init_(d) == 2.0/3.0 * d);
					REQUIRE(i.get_current_state().after.d_init_(d) == 1.0/3.0 * d);
				}
			}

			AND_WHEN("Meshline Normal are NONE") {
				MeshlinePolicy a(
					Y,
					MeshlinePolicy::Policy::THIRDS,
					MeshlinePolicy::Normal::NONE,
					&p,
					10,
					t);
				MeshlinePolicy b(
					Y,
					MeshlinePolicy::Policy::THIRDS,
					MeshlinePolicy::Normal::NONE,
					&p,
					20,
					t);
				Interval i(&a, &b, Y, &p, t);
				THEN("d_init should be d") {
					REQUIRE(i.get_current_state().before.d_init_(d) == d);
					REQUIRE(i.get_current_state().after.d_init_(d) == d);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("std::vector<Coord> find_ls(double d, double smoothness, double dmax, Coord s)", "[interval]") {
	GIVEN("d > dmax") {
		std::vector<Coord> ls = find_ls(4.0, 2.0, 3.0, 10.0);
		THEN("Coordinates should grow according 'ls[n] = ls[n-1] + dmax' algorithm") {
			REQUIRE(ls.size() == 4);
			REQUIRE(ls[0] == Coord(3.0));
			REQUIRE(ls[1] == Coord(6.0));
			REQUIRE(ls[2] == Coord(9.0));
			AND_THEN("Should stop once s is passed") {
				REQUIRE(ls[3] == Coord(12.0));
			}
		}
	}

	GIVEN("dmax will be reached to fill s") {
		std::vector<Coord> ls = find_ls(0.1, 2.0, 3.0, 10.0);
		THEN("Coordinates should grow according 'ls[n] = ls[n-1] + ls[n-1] * smoothness' algorithm until ls[n] reaches dmax") {
			REQUIRE(ls.size() == 7);
			REQUIRE(ls[0] == Coord(0.2));
			REQUIRE(ls[1] == Coord(0.6));
			REQUIRE(ls[2] == Coord(1.4));
			REQUIRE(ls[3] == Coord(3.0));
			AND_THEN("Coordinates should grow according 'ls[n] = ls[n-1] + dmax' algorithm") {
				REQUIRE(ls[4] == Coord(6.0));
				REQUIRE(ls[5].value() == Coord(9.0));
				AND_THEN("Should stop once s is passed") {
					REQUIRE(ls[6] == Coord(12.0));
				}
			}
		}
	}

	GIVEN("dmax will not be reached to fill s") {
		std::vector<Coord> ls = find_ls(0.1, 2.0, 30.0, 10.0);
		THEN("Coordinates should grow according 'ls[n] = ls[n-1] + ls[n-1] * smoothness' algorithm") {
			REQUIRE(ls.size() == 6);
			REQUIRE(ls[0] == Coord(0.2));
			REQUIRE(ls[1] == Coord(0.6));
			REQUIRE(ls[2] == Coord(1.4));
			REQUIRE(ls[3] == Coord(3.0));
			REQUIRE(ls[4] == Coord(6.2));
			AND_THEN("Should stop once s is passed") {
				REQUIRE(ls[5] == Coord(12.6));
			}
		}
	}

	GIVEN("d > s") {
		std::vector<Coord> ls = find_ls(3.0, 2.0, 7.0, 2.0);
		THEN("Should return only one coordinate, after s") {
			REQUIRE(ls.size() == 1);
			REQUIRE(ls[0] == Coord(6.0));
		}
	}
}

//******************************************************************************
SCENARIO("bool is_ls_valid_for_dmax_lmin_smoothness(std::vector<Coord> const& ls, double d, double smoothness, double dmax, size_t lmin)", "[interval]") {
	GIVEN("An empty ls vector") {
		THEN("Should not be valid, disregarding lmin") {
			REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness({}, 1.0, 2.0, 3.0, 2));
			REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness({}, 1.0, 2.0, 3.0, 0));
		}
	}

	GIVEN("An ls vector containing one line") {
		std::vector<Coord> ls { 0.2 };
		WHEN("There is less than lmin lines") {
			THEN("Should not be valid") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(ls, 0.1, 2.0, 30.0, 10));
			}
		}

		WHEN("A space between two lines, including d exceeds dmax") {
			THEN("Should not be valid") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(ls, 0.1, 2.0, 0.05, 5));
			}
		}

		WHEN("A space between two lines is not between smoothness times and 1/smoothness times its adjacent spaces, d being the space previous the first ls space") {
			THEN("Should not be valid") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(ls, 0.05, 2.0, 30.0, 5));
			}
		}

		WHEN("There is lmin lines or more") {
			AND_WHEN("No space between two lines exceeds dmax") {
				AND_WHEN("Every space between two lines is between smoothness times and 1/smoothness times its adjacent spaces") {
					THEN("Should be valid") {
						REQUIRE(is_ls_valid_for_dmax_lmin_smoothness(ls, 0.1, 2.0, 30.0, 1));
					}
				}
			}
		}
	}

	GIVEN("An arbitrary sized ls vector") {
		std::vector<Coord> ls { 0.2, 0.6, 1.4, 3.0, 6.2, 12.6 };
		WHEN("There is less than lmin lines") {
			THEN("Should not be valid") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(ls, 0.1, 2.0, 30.0, 10));
			}
		}

		WHEN("A space between two lines, including d exceeds dmax") {
			THEN("Should not be valid") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(ls, 0.1, 2.0, 0.05, 5));
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(ls, 0.1, 2.0, 0.15, 5));
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(ls, 0.1, 2.0, 0.25, 5));
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(ls, 0.1, 2.0, 5.0, 5));
			}
		}

		WHEN("A space between two lines is not between smoothness times and 1/smoothness times its adjacent spaces, d being the space previous the first ls space") {
			THEN("Should not be valid") {
				std::vector<Coord> ls1 { 0.3, 0.6, 1.4, 3.0, 6.2, 12.6 };
				std::vector<Coord> ls2 { 0.2, 1.0, 1.4, 3.0, 6.2, 12.6 };
				std::vector<Coord> ls3 { 0.2, 0.6, 1.4, 3.0, 6.2, 13.6 };
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(ls, 0.05, 2.0, 30.0, 5));
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(ls1, 0.1, 2.0, 30.0, 5));
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(ls2, 0.1, 2.0, 30.0, 5));
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(ls3, 0.1, 2.0, 30.0, 5));

			}
		}

		WHEN("There is lmin lines or more") {
			AND_WHEN("No space between two lines exceeds dmax") {
				AND_WHEN("Every space between two lines is between smoothness times and 1/smoothness times its adjacent spaces") {
					THEN("Should be valid") {
						REQUIRE(is_ls_valid_for_dmax_lmin_smoothness(ls, 0.1, 2.0, 30.0, 5));
					}
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("double find_dmax(Interval::Side const& side, double dmax)", "[interval]") {
	Timepoint* t = Caretaker::singleton().get_history_root();
	GIVEN("A Side of an Interval with previously computed ls") {
		GlobalParams p(t);
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::NONE,
			&p,
			10,
			t);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::NONE,
			&p,
			20,
			t);
		auto state_p = p.get_current_state();
		auto state_a = a.get_current_state();
		state_p.smoothness = 2;
		state_a.d = 1;
		p.set_next_state(state_p);
		a.set_next_state(state_a);
		Interval i(&a, &b, Y, &p, t);
		WHEN("ls contains no lines") {
			THEN("Should return dmax") {
				REQUIRE_FALSE(i.get_current_state().before.ls.size());
				REQUIRE(find_dmax(i.get_current_state().before, 2.0) == 2.0);
			}
		}

		WHEN("ls contains one line") {
			auto state_i = i.get_current_state();
			state_i.before.ls = { 3.0 };
			i.set_next_state(state_i);
			AND_WHEN("The line coord does not exceed dmax") {
				THEN("Should return smoothness times the line coord") {
					REQUIRE(i.get_current_state().before.ls.size() == 1);
					REQUIRE(find_dmax(i.get_current_state().before, 8.0) == 6.0);
				}
			}

			AND_WHEN("The line coord exceed dmax") {
				THEN("Should return dmax") {
					REQUIRE(i.get_current_state().before.ls.size() == 1);
					REQUIRE(find_dmax(i.get_current_state().before, 2.0) == 2.0);
				}
			}
		}

		WHEN("ls contains some lines") {
			auto state_i = i.get_current_state();
			state_i.before.ls = { 1.0, 3.0, 6.0 };
			i.set_next_state(state_i);
			AND_WHEN("The last space does not exceed dmax") {
				THEN("Should return smoothness times the last space") {
					REQUIRE(i.get_current_state().before.ls.size() == 3);
					REQUIRE(find_dmax(i.get_current_state().before, 8.0) == 6.0);
				}
			}

			AND_WHEN("The last space exceed dmax") {
				THEN("Should return dmax") {
					REQUIRE(i.get_current_state().before.ls.size() == 3);
					REQUIRE(find_dmax(i.get_current_state().before, 2.0) == 2.0);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("double find_dmax(Interval::Side const& side, Interval::Side const& b, double dmax)", "[interval]") {
	Timepoint* t = Caretaker::singleton().get_history_root();
	GIVEN("Two Sides of an Interval with previously computed ls") {
		GlobalParams p(t);
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::NONE,
			&p,
			10,
			t);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::NONE,
			&p,
			20,
			t);
		auto state_p = p.get_current_state();
		auto state_a = a.get_current_state();
		state_p.smoothness = 2;
		state_a.d = 1;
		p.set_next_state(state_p);
		a.set_next_state(state_a);
		Interval i(&a, &b, Y, &p, t);
		auto state_i = i.get_current_state();
		state_i.before.ls = { 2.0 };
		state_i.after.ls = { 1.0, 3.0, 6.0 };
		i.set_next_state(state_i);
		THEN("Should return the minimal dmax of both Sides'") {
			REQUIRE(find_dmax(i.get_current_state().before, 5.0) == 4.0);
			REQUIRE(find_dmax(i.get_current_state().after, 5.0) == 5.0);
			REQUIRE(find_dmax(i.get_current_state().before, i.get_current_state().after, 5.0) == 4.0);
			REQUIRE(find_dmax(i.get_current_state().after, i.get_current_state().before, 5.0) == 4.0);
		}
	}
}

//******************************************************************************
SCENARIO("std::tuple<double, bool> Interval::adjust_d_for_dmax_lmin(Interval::Side const& side, size_t iter_limit) const", "[interval]") {
	Timepoint* t = Caretaker::singleton().get_history_root();
	GIVEN("A Side of an Interval with previously computed ls") {
		WHEN("dmax and lmin end criteras are already respected") {
			GlobalParams p(t);
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				&p,
				10,
				t);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				&p,
				20,
				t);
			auto state_p = p.get_current_state();
			auto state_a = a.get_current_state();
			state_p.dmax = 1.2;
			state_p.lmin = 2;
			state_a.d = 1.0;
			p.set_next_state(state_p);
			a.set_next_state(state_a);
			Interval i(&a, &b, Y, &p, t);
			auto state_i = i.get_current_state();
			i.update_ls(state_i);
			i.set_next_state(state_i);
			THEN("Initial ls should be conform") {
				REQUIRE(is_ls_valid_for_dmax_lmin_smoothness(
					i.get_current_state().before.ls,
					a.get_current_state().d,
					i.get_current_state().before.smoothness,
					p.get_current_state().dmax,
					p.get_current_state().lmin));
			}

			THEN("Side's d should be unchanged") {
				auto new_d = std::get<0>(i.adjust_d_for_dmax_lmin(i.get_current_state().before));
				REQUIRE(new_d == 1.0);
			}
		}

		WHEN("dmax end critera is already respected but not lmin one") {
			GlobalParams p(t);
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				&p,
				10,
				t);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				&p,
				20,
				t);
			auto state_p = p.get_current_state();
			auto state_a = a.get_current_state();
			state_p.dmax = 2.0;
			state_p.lmin = 10;
			state_a.d = 1.0;
			p.set_next_state(state_p);
			a.set_next_state(state_a);
			Interval i(&a, &b, Y, &p, t);
			auto state_i = i.get_current_state();
			i.update_ls(state_i);
			i.set_next_state(state_i);
			THEN("Initial ls should not be conform") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(
					i.get_current_state().before.ls,
					a.get_current_state().d,
					i.get_current_state().before.smoothness,
					p.get_current_state().dmax,
					p.get_current_state().lmin));
			}

			THEN("Side's d should be reduced") {
				auto state_a = a.get_current_state();
				state_a.d = std::get<0>(i.adjust_d_for_dmax_lmin(i.get_current_state().before));
				a.set_next_state(state_a);
				REQUIRE(a.get_current_state().d < 1.0);
				AND_THEN("New ls should be conform") {
					auto state_i = i.get_current_state();
					i.update_ls(state_i);
					i.set_next_state(state_i);
					REQUIRE(is_ls_valid_for_dmax_lmin_smoothness(
						i.get_current_state().before.ls,
						a.get_current_state().d,
						i.get_current_state().before.smoothness,
						p.get_current_state().dmax,
						p.get_current_state().lmin));
				}
			}
		}

		WHEN("lmin end critera is already respected but not dmax one") {
			GlobalParams p(t);
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				&p,
				10,
				t);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				&p,
				20,
				t);
			auto state_p = p.get_current_state();
			auto state_a = a.get_current_state();
			state_p.dmax = 0.8;
			state_p.lmin = 2;
			state_a.d = 1.0;
			p.set_next_state(state_p);
			a.set_next_state(state_a);
			Interval i(&a, &b, Y, &p, t);
			auto state_i = i.get_current_state();
			i.update_ls(state_i);
			i.set_next_state(state_i);
			THEN("Initial ls should not be conform") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(
					i.get_current_state().before.ls,
					a.get_current_state().d,
					i.get_current_state().before.smoothness,
					p.get_current_state().dmax,
					p.get_current_state().lmin));
			}

			THEN("Side's d should be reduced") {
				auto state_a = a.get_current_state();
				state_a.d = std::get<0>(i.adjust_d_for_dmax_lmin(i.get_current_state().before));
				a.set_next_state(state_a);
				REQUIRE(a.get_current_state().d <= 0.8);
				AND_THEN("New ls should be conform") {
					auto state_i = i.get_current_state();
					i.update_ls(state_i);
					i.set_next_state(state_i);
					REQUIRE(is_ls_valid_for_dmax_lmin_smoothness(
						i.get_current_state().before.ls,
						a.get_current_state().d,
						i.get_current_state().before.smoothness,
						p.get_current_state().dmax,
						p.get_current_state().lmin));
				}
			}
		}

		WHEN("dmax and lmin end criteras are not respected") {
			GlobalParams p(t);
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				&p,
				10,
				t);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				&p,
				20,
				t);
			auto state_p = p.get_current_state();
			auto state_a = a.get_current_state();
			state_p.dmax = 0.8;
			state_p.lmin = 10;
			state_a.d = 1.0;
			p.set_next_state(state_p);
			a.set_next_state(state_a);
			Interval i(&a, &b, Y, &p, t);
			auto state_i = i.get_current_state();
			i.update_ls(state_i);
			i.set_next_state(state_i);
			THEN("Initial ls should not be conform") {
				REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(
					i.get_current_state().before.ls,
					a.get_current_state().d,
					i.get_current_state().before.smoothness,
					p.get_current_state().dmax,
					p.get_current_state().lmin));
			}

			AND_WHEN("Process iterations are unlimited") {
				THEN("Side's d should be reduced") {
					auto [new_d, is_limit_reached] = i.adjust_d_for_dmax_lmin(i.get_current_state().before);
					auto state_a = a.get_current_state();
					state_a.d = new_d;
					a.set_next_state(state_a);
					REQUIRE_FALSE(is_limit_reached);
					REQUIRE(a.get_current_state().d <= 0.8);
					AND_THEN("New ls should be conform") {
						auto state_i = i.get_current_state();
						i.update_ls(state_i);
						i.set_next_state(state_i);
						REQUIRE(is_ls_valid_for_dmax_lmin_smoothness(
							i.get_current_state().before.ls,
							a.get_current_state().d,
							i.get_current_state().before.smoothness,
							p.get_current_state().dmax,
							p.get_current_state().lmin));
					}
				}
			}

			AND_WHEN("Process iterations are limited") {
				THEN("Side's d should be reduced") {
					auto [new_d, is_limit_reached] = i.adjust_d_for_dmax_lmin(i.get_current_state().before, 20);
					auto state_a = a.get_current_state();
					state_a.d = new_d;
					a.set_next_state(state_a);
					REQUIRE(is_limit_reached);
					REQUIRE(a.get_current_state().d <= 0.8);
					AND_THEN("New ls should not be conform") {
						auto state_i = i.get_current_state();
						i.update_ls(state_i);
						i.set_next_state(state_i);
						REQUIRE_FALSE(is_ls_valid_for_dmax_lmin_smoothness(
							i.get_current_state().before.ls,
							a.get_current_state().d,
							i.get_current_state().before.smoothness,
							p.get_current_state().dmax,
							p.get_current_state().lmin));
					}
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("std::tuple<double, bool> Interval::adjust_smoothness_for_s(Interval::Side const& side, size_t iter_limit) const", "[interval]") {
	Timepoint* t = Caretaker::singleton().get_history_root();
	GIVEN("A Side of an Interval with previously computed valid ls with smoothness superior to 2") {
		GlobalParams p(t);
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			&p,
			0,
			t);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			&p,
			20,
			t);
		auto state_p = p.get_current_state();
		auto state_a = a.get_current_state();
		state_p.dmax = 30.0;
		state_p.lmin = 5;
		state_p.smoothness = 2;
		state_a.d = 0.1;
		p.set_next_state(state_p);
		a.set_next_state(state_a);
		Interval i(&a, &b, Y, &p, t);
		Interval j(&a, &b, Y, &p, t);
		auto state_i = i.get_current_state();
		auto state_j = j.get_current_state();
		i.update_ls(state_i);
		j.update_ls(state_j);
		i.set_next_state(state_i);
		j.set_next_state(state_j);
		Coord old_last_space = i.get_current_state().before.ls.back() - i.s(i.get_current_state().before);
		REQUIRE(is_ls_valid_for_dmax_lmin_smoothness(
			i.get_current_state().before.ls,
			a.get_current_state().d,
			i.get_current_state().before.smoothness,
			p.get_current_state().dmax,
			p.get_current_state().lmin));
		REQUIRE(is_ls_valid_for_dmax_lmin_smoothness(
			j.get_current_state().before.ls,
			a.get_current_state().d,
			j.get_current_state().before.smoothness,
			p.get_current_state().dmax,
			p.get_current_state().lmin));
		WHEN("Process iterations are unlimited") {
			THEN("Side's smoothness should be reduced") {
				{
					auto [new_smoothness, is_limit_reached] = i.adjust_smoothness_for_s(i.get_current_state().before);
					auto state_i = i.get_current_state();
					state_i.before.smoothness = new_smoothness;
					i.update_ls(state_i);
					i.set_next_state(state_i);
					REQUIRE_FALSE(is_limit_reached);
					REQUIRE(i.get_current_state().before.smoothness < p.get_current_state().smoothness);
				}

				AND_THEN("The space between the last ls line and the middle of the Interval should be reduced") {
					Coord new_last_space = i.get_current_state().before.ls.back() - i.s(i.get_current_state().before);
					REQUIRE(new_last_space < old_last_space);

					AND_WHEN("Process iterations are limited") {
						THEN("Side's smoothness should be reduced") {
							{
								auto [new_smoothness, is_limit_reached] = j.adjust_smoothness_for_s(j.get_current_state().before, 20);
								auto state_j = j.get_current_state();
								state_j.before.smoothness = new_smoothness;
								j.update_ls(state_j);
								j.set_next_state(state_j);
								REQUIRE(is_limit_reached);
								REQUIRE(j.get_current_state().before.smoothness < p.get_current_state().smoothness);
							}

							AND_THEN("The space between the last ls line and the middle of the Interval should be reduced, but less than if unlimited") {
								Coord new_last_space_limited = j.get_current_state().before.ls.back() - j.s(i.get_current_state().before);
								REQUIRE(new_last_space_limited < old_last_space);
								REQUIRE(new_last_space_limited > new_last_space);
							}
						}
					}
				}
			}
		}
	}

	GIVEN("A Side of an Interval with previously computed valid ls with smoothness being 1") {
		GlobalParams p(t);
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			&p,
			0,
			t);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			&p,
			20,
			t);
		auto state_p = p.get_current_state();
		auto state_a = a.get_current_state();
		state_p.dmax = 30.0;
		state_p.lmin = 5;
		state_p.smoothness = 1;
		state_a.d = 0.1;
		p.set_next_state(state_p);
		a.set_next_state(state_a);
		Interval i(&a, &b, Y, &p, t);
		auto state_i = i.get_current_state();
		i.update_ls(state_i);
		i.set_next_state(state_i);
		Coord old_last_space = i.get_current_state().before.ls.back() - i.s(i.get_current_state().before);
		REQUIRE(is_ls_valid_for_dmax_lmin_smoothness(
			i.get_current_state().before.ls,
			a.get_current_state().d,
			i.get_current_state().before.smoothness,
			p.get_current_state().dmax,
			p.get_current_state().lmin));
		THEN("Side's smoothness should not be reduced") {
			auto [new_smoothness, is_limit_reached] = i.adjust_smoothness_for_s(i.get_current_state().before);
			auto state_i = i.get_current_state();
			state_i.before.smoothness = new_smoothness;
			i.update_ls(state_i);
			i.set_next_state(state_i);
			REQUIRE_FALSE(is_limit_reached);
			REQUIRE(new_smoothness == p.get_current_state().smoothness);

			AND_THEN("The space between the last ls line and the middle of the Interval should not be reduced") {
				Coord new_last_space = i.get_current_state().before.ls.back() - i.s(i.get_current_state().before);
				REQUIRE(new_last_space == old_last_space);
			}
		}
	}
}

//******************************************************************************
SCENARIO("std::vector<std::unique_ptr<Meshline>> Interval::mesh() const", "[interval]") {
	auto const match_line =
		[](std::vector<std::shared_ptr<Meshline>> const& mesh, Coord const& line) {
			for(auto const& it : mesh)
				if(it->coord == line)
					return true;
			return false;
		};

	Timepoint* t = Caretaker::singleton().get_history_root();

	GIVEN("An Interval with ls computed for both Sides") {
		WHEN("Sides' policies are HALFS or THIRDS") {
			GlobalParams p(t);
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				&p,
				10,
				t);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::THIRDS,
				MeshlinePolicy::Normal::MAX,
				&p,
				30,
				t);
			MeshlinePolicy c(
				Y,
				MeshlinePolicy::Policy::THIRDS,
				MeshlinePolicy::Normal::MAX,
				&p,
				10,
				t);
			MeshlinePolicy d(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				&p,
				30,
				t);
			auto state_p = p.get_current_state();
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			auto state_c = c.get_current_state();
			auto state_d = d.get_current_state();
			state_p.dmax = 2;
			state_p.lmin = 5;
			state_p.smoothness = 2;
			state_a.d = 0.1;
			state_b.d = 0.1;
			state_c.d = 0.1;
			state_d.d = 0.1;
			p.set_next_state(state_p);
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			c.set_next_state(state_c);
			d.set_next_state(state_d);
			Interval i(&a, &b, Y, &p, t);
			Interval j(&c, &d, Y, &p, t);
			auto state_i = i.get_current_state();
			auto state_j = j.get_current_state();
			i.update_ls(state_i);
			j.update_ls(state_j);
			i.set_next_state(state_i);
			j.set_next_state(state_j);
			auto mesh_i = i.mesh();
			auto mesh_j = j.mesh();
			THEN("Policy lines should be placed") {
				REQUIRE(mesh_i.size() >= 2);
				REQUIRE(mesh_j.size() >= 2);
				REQUIRE(mesh_i.front()->coord == Coord(10.05));
				REQUIRE(mesh_i.back()->coord.value() == Coord(30 - 0.1/3));
				REQUIRE(mesh_j.front()->coord == Coord(10 + 2 * 0.1/3));
				REQUIRE(mesh_j.back()->coord.value() == Coord(29.95));
			}

			THEN("Last line of both sides should not be placed") {
				REQUIRE_FALSE(match_line(mesh_i, Coord(
					i.get_current_state().before.meshline_policy->coord
					+ i.get_current_state().before.d_init()
					+ i.get_current_state().before.ls.back())));
				REQUIRE_FALSE(match_line(mesh_i, Coord(
					i.get_current_state().after.meshline_policy->coord
					- i.get_current_state().after.d_init()
					- i.get_current_state().after.ls.back())));

				REQUIRE_FALSE(match_line(mesh_j, Coord(
					j.get_current_state().before.meshline_policy->coord
					+ j.get_current_state().before.d_init()
					+ j.get_current_state().before.ls.back())));
				REQUIRE_FALSE(match_line(mesh_j, Coord(
					j.get_current_state().after.meshline_policy->coord
					- j.get_current_state().after.d_init()
					- j.get_current_state().after.ls.back())));

				AND_THEN("Other lines should be placed") {
					for(auto it = begin(i.get_current_state().before.ls); it != prev(end(i.get_current_state().before.ls)); ++it)
						REQUIRE(match_line(mesh_i, Coord(
							i.get_current_state().before.meshline_policy->coord
							+ i.get_current_state().before.d_init()
							+ (*it))));
					for(auto it = begin(i.get_current_state().after.ls); it != prev(end(i.get_current_state().after.ls)); ++it)
						REQUIRE(match_line(mesh_i, Coord(
							i.get_current_state().after.meshline_policy->coord
							- i.get_current_state().after.d_init()
							- (*it))));

					for(auto it = begin(j.get_current_state().before.ls); it != prev(end(j.get_current_state().before.ls)); ++it)
						REQUIRE(match_line(mesh_j, Coord(
							j.get_current_state().before.meshline_policy->coord
							+ j.get_current_state().before.d_init()
							+ (*it))));
					for(auto it = begin(j.get_current_state().after.ls); it != prev(end(j.get_current_state().after.ls)); ++it)
						REQUIRE(match_line(mesh_j, Coord(
							j.get_current_state().after.meshline_policy->coord
							- j.get_current_state().after.d_init()
							- (*it))));
				}
			}

			THEN("Middle line should be placed instead") {
				REQUIRE(match_line(mesh_i, i.m));
				REQUIRE(match_line(mesh_j, j.m));
			}
		}

		WHEN("Sides' policies are ONELINE") {
			GlobalParams p(t);
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::ONELINE,
				MeshlinePolicy::Normal::NONE,
				&p,
				10,
				t);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::ONELINE,
				MeshlinePolicy::Normal::NONE,
				&p,
				30,
				t);
			auto state_p = p.get_current_state();
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			state_p.dmax = 1.5;
			state_p.lmin = 5;
			state_p.smoothness = 2;
			state_a.d = 0.1;
			state_b.d = 0.1;
			p.set_next_state(state_p);
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			Interval i(&a, &b, Y, &p, t);
			auto state_i = i.get_current_state();
			i.update_ls(state_i);
			i.set_next_state(state_i);
			auto mesh = i.mesh();
			THEN("Policy lines should not be placed") {
				REQUIRE_FALSE(match_line(mesh, i.get_current_state().before.meshline_policy->coord));
				REQUIRE_FALSE(match_line(mesh, i.get_current_state().after.meshline_policy->coord));
			}

			THEN("Last line of both sides should not be placed") {
				REQUIRE_FALSE(match_line(mesh, Coord(
					i.get_current_state().before.meshline_policy->coord
					+ i.get_current_state().before.d_init()
					+ i.get_current_state().before.ls.back())));
				REQUIRE_FALSE(match_line(mesh, Coord(
					i.get_current_state().after.meshline_policy->coord
					- i.get_current_state().after.d_init()
					- i.get_current_state().after.ls.back())));

				AND_THEN("Other lines should be placed") {
					for(auto it = begin(i.get_current_state().before.ls); it != prev(end(i.get_current_state().before.ls)); ++it)
						REQUIRE(match_line(mesh, Coord(
							i.get_current_state().before.meshline_policy->coord
							+ i.get_current_state().before.d_init()
							+ (*it))));
					for(auto it = begin(i.get_current_state().after.ls); it != prev(end(i.get_current_state().after.ls)); ++it)
						REQUIRE(match_line(mesh, Coord(
							i.get_current_state().after.meshline_policy->coord
							- i.get_current_state().after.d_init()
							- (*it))));
				}
			}

			THEN("Middle line should be placed instead") {
				REQUIRE(match_line(mesh, i.m));
				REQUIRE(match_line(mesh, i.m));
			}
		}
	}

	GIVEN("An Interval without ls computed for both Sides") {
		WHEN("Sides' policies are HALFS or THIRDS") {
			GlobalParams p(t);
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				&p,
				10,
				t);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::THIRDS,
				MeshlinePolicy::Normal::MAX,
				&p,
				30,
				t);
			MeshlinePolicy c(
				Y,
				MeshlinePolicy::Policy::THIRDS,
				MeshlinePolicy::Normal::MAX,
				&p,
				10,
				t);
			MeshlinePolicy d(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				&p,
				30,
				t);
			auto state_p = p.get_current_state();
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			auto state_c = c.get_current_state();
			auto state_d = d.get_current_state();
			state_p.dmax = 2;
			state_p.lmin = 5;
			state_p.smoothness = 2;
			state_a.d = 0.1;
			state_b.d = 0.1;
			state_c.d = 0.1;
			state_d.d = 0.1;
			p.set_next_state(state_p);
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			c.set_next_state(state_c);
			d.set_next_state(state_d);
			Interval i(&a, &b, Y, &p, t);
			Interval j(&c, &d, Y, &p, t);
			auto mesh_i = i.mesh();
			auto mesh_j = j.mesh();
			THEN("The only lines should be policy lines and middle line") {
				REQUIRE(mesh_i.size() == 3);
				REQUIRE(mesh_j.size() == 3);
				REQUIRE(mesh_i[0]->coord == Coord(10.05));
				REQUIRE(mesh_i[1]->coord == i.m);
				REQUIRE(mesh_i[2]->coord == Coord(30 - 0.1/3));
				REQUIRE(mesh_j[0]->coord == Coord(10 + 2 * 0.1/3));
				REQUIRE(mesh_j[1]->coord == j.m);
				REQUIRE(mesh_j[2]->coord == Coord(29.95));
			}
		}

		WHEN("Sides' policies are ONELINE") {
			GlobalParams p(t);
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::ONELINE,
				MeshlinePolicy::Normal::NONE,
				&p,
				10,
				t);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::ONELINE,
				MeshlinePolicy::Normal::NONE,
				&p,
				30,
				t);
			auto state_p = p.get_current_state();
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			state_p.dmax = 1.5;
			state_p.lmin = 5;
			state_p.smoothness = 2;
			state_a.d = 0.1;
			state_b.d = 0.1;
			p.set_next_state(state_p);
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			Interval i(&a, &b, Y, &p, t);
			auto mesh = i.mesh();
			THEN("The only line should be the middle line") {
				REQUIRE(mesh.size() == 1);
				REQUIRE(mesh.front()->coord == i.m);
			}
		}
	}
}

// test adjust_d
// TODO init : random ls
// TODO init : d init = 2
// TODO init : invalid ls
// TODO after : different d
// TODO after : valid ls
