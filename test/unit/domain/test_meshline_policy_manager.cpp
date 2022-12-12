///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include <vector>

#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/point.hpp"
#include "domain/conflict_manager.hpp"

#include "domain/meshline_policy_manager.hpp"

/// @test MeshlinePolicy* MeshlinePolicyManager::add_meshline_policy(
///       	IMeshLineOrigin* origin,
///       	Axis const axis,
///       	MeshlinePolicy::Policy const policy,
///       	Normal const normal,
///       	Coord const coord,
///       	bool const is_enabled)
/// @test optional<array<MeshlinePolicy*, 2>> detect_closest_meshline_policies(
///       	vector<MeshlinePolicy*> dimension,
///       	Coord proximity_limit)
/// @test void MeshlinePolicyManager::detect_and_solve_too_close_meshline_policies()
/// @test void MeshlinePolicyManager::detect_intervals()
/// @test void MeshlinePolicyManager::mesh()
///*****************************************************************************

using namespace domain;

//******************************************************************************
SCENARIO("MeshlinePolicy* MeshlinePolicyManager::add_meshline_policy( \
IMeshLineOrigin* origin, \
Axis const axis, \
MeshlinePolicy::Policy const policy, \
Normal const normal, \
Coord const coord, \
bool const is_enabled)", "[meshline_policy_manager]") {
	GIVEN("A meshline policy manager") {
		Params params;
		MeshlinePolicyManager mpm(params, nullptr);
		Point e0(1, 1), e1(1, 3);
		Edge e(XY, &e0, &e1);

		WHEN("We add a meshline policy with incoherent policy and normal parameters") { // TODO
			MeshlinePolicy* m = mpm.add_meshline_policy(
				&e, Y, MeshlinePolicy::Policy::THIRDS, MeshlinePolicy::Normal::NONE, 1);
			MeshlinePolicy* n = mpm.add_meshline_policy(
				&e, Y, MeshlinePolicy::Policy::ONELINE, MeshlinePolicy::Normal::MIN, 1);
			MeshlinePolicy* o = mpm.add_meshline_policy(
				&e, Y, MeshlinePolicy::Policy::ONELINE, MeshlinePolicy::Normal::MAX, 1);
			MeshlinePolicy* p = mpm.add_meshline_policy(
				&e, X, MeshlinePolicy::Policy::ONELINE, MeshlinePolicy::Normal::MIN, 1);
			MeshlinePolicy* q = mpm.add_meshline_policy(
				&e, X, MeshlinePolicy::Policy::ONELINE, MeshlinePolicy::Normal::MAX, 1);
			MeshlinePolicy* r = mpm.add_meshline_policy(
				&e, Y, MeshlinePolicy::Policy::HALFS, MeshlinePolicy::Normal::MIN, 1);
			MeshlinePolicy* s = mpm.add_meshline_policy(
				&e, Y, MeshlinePolicy::Policy::HALFS, MeshlinePolicy::Normal::MAX, 1);
			MeshlinePolicy* t = mpm.add_meshline_policy(
				&e, X, MeshlinePolicy::Policy::HALFS, MeshlinePolicy::Normal::MIN, 1);
			MeshlinePolicy* u = mpm.add_meshline_policy(
				&e, X, MeshlinePolicy::Policy::HALFS, MeshlinePolicy::Normal::MAX, 1);
			THEN("No meshline policy should be added nor returned") {
				REQUIRE(mpm.line_policies[X].size() == 0);
				REQUIRE(mpm.line_policies[Y].size() == 0);
				REQUIRE(mpm.line_policies[Z].size() == 0);
				REQUIRE(m == nullptr);
				REQUIRE(n == nullptr);
				REQUIRE(o == nullptr);
				REQUIRE(p == nullptr);
				REQUIRE(q == nullptr);
				REQUIRE(r == nullptr);
				REQUIRE(s == nullptr);
				REQUIRE(t == nullptr);
				REQUIRE(u == nullptr);
			}
		}

		WHEN("We add a random meshline policy with coherent parameters") {
			MeshlinePolicy* m = mpm.add_meshline_policy(
				&e, X, MeshlinePolicy::Policy::THIRDS, MeshlinePolicy::Normal::MAX, 1);
			THEN("Should add a meshline policy to the inner container and return a pointer to it") {
				REQUIRE(mpm.line_policies[Y].size() == 0);
				REQUIRE(mpm.line_policies[X].size() == 1);
				REQUIRE(mpm.line_policies[X][0].get() == m);
				AND_THEN("All the meshline policy properties should be correct") {
					REQUIRE(m->origins.size() == 1);
					REQUIRE(m->origins[0] == &e);
					REQUIRE(m->axis == X);
					REQUIRE(m->policy == MeshlinePolicy::Policy::THIRDS);
					REQUIRE(m->normal == MeshlinePolicy::Normal::MAX);
					REQUIRE(m->coord == 1);
					REQUIRE(m->is_enabled);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("optional<array<MeshlinePolicy*, 2>> detect_closest_meshline_policies( \
vector<MeshlinePolicy*> dimension, \
Coord proximity_limit)", "[meshline_policy_manager]") {
	GIVEN("A vector of pointers to meshline policies and a proximity limit") {
		std::vector<MeshlinePolicy*> vec;
		Params params;
		params.proximity_limit = 1;
		WHEN("Containing no meshline policy") {
			THEN("Should return nullopt") {
				auto ret = detect_closest_meshline_policies(vec, params.proximity_limit);
				REQUIRE_FALSE(ret.has_value());
			}
		}

		WHEN("Containing one meshline policy") {
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				10);
			vec.push_back(&a);
			THEN("Should return nullopt") {
				auto ret = detect_closest_meshline_policies(vec, params.proximity_limit);
				REQUIRE_FALSE(ret.has_value());
			}
		}

		WHEN("Containing two meshline policies more distant than proximity limit") {
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				20);
			vec.push_back(&a);
			vec.push_back(&b);
			THEN("Should return nullopt") {
				auto ret = detect_closest_meshline_policies(vec, params.proximity_limit);
				REQUIRE_FALSE(ret.has_value());
			}
		}

		WHEN("Containing two meshline policies as distant as proximity limit") {
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				11);
			vec.push_back(&a);
			vec.push_back(&b);
			THEN("Should return both") {
				auto ret = detect_closest_meshline_policies(vec, params.proximity_limit);
				REQUIRE(ret.has_value());
				REQUIRE_THAT(*ret, Catch::Matchers::Contains(&a));
				REQUIRE_THAT(*ret, Catch::Matchers::Contains(&b));
			}
		}

		WHEN("Containing two meshline policies less distant than proximity limit") {
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				10.5);
			vec.push_back(&a);
			vec.push_back(&b);
			THEN("Should return both") {
				auto ret = detect_closest_meshline_policies(vec, params.proximity_limit);
				REQUIRE(ret.has_value());
				REQUIRE_THAT(*ret, Catch::Matchers::Contains(&a));
				REQUIRE_THAT(*ret, Catch::Matchers::Contains(&b));
			}
		}

		WHEN("Containing three meshline policies with two less distant than proximity limit") {
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				10.5);
			MeshlinePolicy c(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				20);
			vec.push_back(&a);
			vec.push_back(&b);
			vec.push_back(&c);
			THEN("Should return the two closer than the limit") {
				auto ret = detect_closest_meshline_policies(vec, params.proximity_limit);
				REQUIRE(ret.has_value());
				REQUIRE_THAT(*ret, Catch::Matchers::Contains(&a));
				REQUIRE_THAT(*ret, Catch::Matchers::Contains(&b));
			}
		}

		WHEN("Containing three meshline policies with two pairs less distant than proximity limit") {
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				11.2);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				10.8);
			MeshlinePolicy c(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				10);
			vec.push_back(&a);
			vec.push_back(&b);
			vec.push_back(&c);
			THEN("Should return the two closer") {
				auto ret = detect_closest_meshline_policies(vec, params.proximity_limit);
				REQUIRE(ret.has_value());
				REQUIRE_THAT(*ret, Catch::Matchers::Contains(&a));
				REQUIRE_THAT(*ret, Catch::Matchers::Contains(&b));
			}
		}

		WHEN("Containing two meshline policies less distant than proximity limit but one is disabled") {
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				10,
				false);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				10.5);
			vec.push_back(&a);
			vec.push_back(&b);
			THEN("Should return nullopt") {
				auto ret = detect_closest_meshline_policies(vec, params.proximity_limit);
				REQUIRE_FALSE(ret.has_value());
			}
		}

		WHEN("Containing two meshline policies less distant than proximity limit but one is ONELINE policy") {
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::ONELINE,
				MeshlinePolicy::Normal::NONE,
				params,
				10);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				params,
				10.5);
			vec.push_back(&a);
			vec.push_back(&b);
			THEN("Should return nullopt") {
				auto ret = detect_closest_meshline_policies(vec, params.proximity_limit);
				REQUIRE_FALSE(ret.has_value());
			}
		}
	}
}

//******************************************************************************
SCENARIO("void MeshlinePolicyManager::detect_and_solve_too_close_meshline_policies()", "[meshline_policy_manager]") {
	class Wrapper {
	public:
		Params params;
		ConflictManager cm;
		MeshlinePolicyManager mpm;

		Wrapper()
		: cm(&mpm)
		, mpm(params, &cm)
		{}
	};

	GIVEN("A meshline policy manager and some meshline policies") {
		Wrapper w;
		w.params.proximity_limit = 1;
		Point e0(1, 1), e1(1, 3);
		Edge e(XY, &e0, &e1);

		// 10  10,2  10,5   11,1  11,7
		//   10,1    10,5   11,1  11,7
		//       10,3       11,1  11,7
		//       10,3          11,4

		w.mpm.add_meshline_policy(
			&e,
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			10);
		w.mpm.add_meshline_policy(
			&e,
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			10.2);
		w.mpm.add_meshline_policy(
			&e,
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			10.5);
		w.mpm.add_meshline_policy(
			&e,
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			11.1);
		w.mpm.add_meshline_policy(
			&e,
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			11.7);

		THEN("Meshline policies should be recursively merged till any is closer than the proximity limit") {
			w.mpm.detect_and_solve_too_close_meshline_policies();
			REQUIRE(w.mpm.line_policies[X].size() == 0);
			REQUIRE(w.mpm.line_policies[Y].size() == 8);
			REQUIRE(w.mpm.line_policies[Y][0]->coord == 10);
			REQUIRE_FALSE(w.mpm.line_policies[Y][0]->is_enabled);
			REQUIRE(w.mpm.line_policies[Y][1]->coord == 10.2);
			REQUIRE_FALSE(w.mpm.line_policies[Y][1]->is_enabled);
			REQUIRE(w.mpm.line_policies[Y][2]->coord == 10.5);
			REQUIRE_FALSE(w.mpm.line_policies[Y][2]->is_enabled);
			REQUIRE(w.mpm.line_policies[Y][3]->coord == 11.1);
			REQUIRE_FALSE(w.mpm.line_policies[Y][3]->is_enabled);
			REQUIRE(w.mpm.line_policies[Y][4]->coord == 11.7);
			REQUIRE_FALSE(w.mpm.line_policies[Y][4]->is_enabled);
			REQUIRE(w.mpm.line_policies[Y][5]->coord == 10.1);
			REQUIRE_FALSE(w.mpm.line_policies[Y][5]->is_enabled);
			REQUIRE(w.mpm.line_policies[Y][6]->coord == 10.3);
			REQUIRE(w.mpm.line_policies[Y][6]->is_enabled);
			REQUIRE(w.mpm.line_policies[Y][7]->coord == 11.4);
			REQUIRE(w.mpm.line_policies[Y][7]->is_enabled);
		}
	}
}

//******************************************************************************
SCENARIO("void MeshlinePolicyManager::detect_intervals()", "[meshline_policy_manager]") {
	class Wrapper {
	public:
		Params params;
		ConflictManager cm;
		MeshlinePolicyManager mpm;

		Wrapper()
		: cm(&mpm)
		, mpm(params, &cm)
		{}
	};

	GIVEN("A meshline policy manager and some meshline policies") {
		Wrapper w;
		w.params.proximity_limit = 1;
		Point e0(1, 1), e1(1, 3);
		Edge e(XY, &e0, &e1);

		w.mpm.add_meshline_policy(
			&e,
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			10);
		w.mpm.add_meshline_policy(
			&e,
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			40);
		w.mpm.add_meshline_policy(
			&e,
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			30);
		w.mpm.add_meshline_policy(
			&e,
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			20);
		w.mpm.add_meshline_policy(
			&e,
			X,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			30);
		w.mpm.add_meshline_policy(
			&e,
			X,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			25,
			false);
		w.mpm.add_meshline_policy(
			&e,
			X,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			20);

		THEN("Intervals betwen each adjacent meshline policies should be detected and ordered") {
			w.mpm.detect_intervals();
			REQUIRE(w.mpm.intervals[Y].size() == 3);
			REQUIRE(w.mpm.intervals[X].size() == 1);
			REQUIRE(w.mpm.intervals[Y][0]->before.meshline_policy == w.mpm.line_policies[Y][0].get());
			REQUIRE(w.mpm.intervals[Y][0]->after.meshline_policy == w.mpm.line_policies[Y][3].get());
			REQUIRE(w.mpm.intervals[Y][1]->before.meshline_policy == w.mpm.line_policies[Y][3].get());
			REQUIRE(w.mpm.intervals[Y][1]->after.meshline_policy == w.mpm.line_policies[Y][2].get());
			REQUIRE(w.mpm.intervals[Y][2]->before.meshline_policy == w.mpm.line_policies[Y][2].get());
			REQUIRE(w.mpm.intervals[Y][2]->after.meshline_policy == w.mpm.line_policies[Y][1].get());
			REQUIRE(w.mpm.intervals[X][0]->before.meshline_policy == w.mpm.line_policies[X][2].get());
			REQUIRE(w.mpm.intervals[X][0]->after.meshline_policy == w.mpm.line_policies[X][0].get());
			REQUIRE(w.mpm.intervals[Y][0]->m.value() == 15);
			REQUIRE(w.mpm.intervals[Y][1]->m.value() == 25);
			REQUIRE(w.mpm.intervals[Y][2]->m.value() == 35);
			REQUIRE(w.mpm.intervals[X][0]->m.value() == 25);
			REQUIRE(w.mpm.intervals[Y][0]->h.value() == 5);
			REQUIRE(w.mpm.intervals[Y][1]->h.value() == 5);
			REQUIRE(w.mpm.intervals[Y][2]->h.value() == 5);
			REQUIRE(w.mpm.intervals[X][0]->h.value() == 5);

		}
	}
}

//******************************************************************************
SCENARIO("void MeshlinePolicyManager::mesh()", "[meshline_policy_manager]") {
	class Wrapper {
	public:
		Params params;
		ConflictManager cm;
		MeshlinePolicyManager mpm;

		Wrapper()
		: cm(&mpm)
		, mpm(params, &cm)
		{}
	};

	GIVEN("A meshline policy manager and some meshline policies") {
		Wrapper w;
		w.params.proximity_limit = 1;
		w.params.lmin = 2;
		w.params.dmax = 4.0;
		Point e0(1, 1), e1(1, 3);
		Edge e(XY, &e0, &e1);

		w.mpm.add_meshline_policy(
			&e,
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			10);
		w.mpm.add_meshline_policy(
			&e,
			Y,
			MeshlinePolicy::Policy::ONELINE,
			MeshlinePolicy::Normal::NONE,
			40);
		w.mpm.add_meshline_policy(
			&e,
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MAX,
			30);
		w.mpm.add_meshline_policy(
			&e,
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			25);
		w.mpm.add_meshline_policy(
			&e,
			Y,
			MeshlinePolicy::Policy::ONELINE,
			MeshlinePolicy::Normal::NONE,
			25.1);

		w.mpm.detect_intervals();
		w.mpm.mesh();

		THEN("Meshlines should be sorted") {
			REQUIRE(w.mpm.meshlines[Y].size() > 2);
			for(auto it = next(begin(w.mpm.meshlines[Y])); it != end(w.mpm.meshlines[Y]); ++it)
				REQUIRE((*prev(it))->coord < (*it)->coord);
		}

		THEN("ONELINE meshlines should be placed precisely") {
			auto does_contains = [&w](Coord a) -> bool {
						for(auto const& it : w.mpm.meshlines[Y])
							if(it->coord == a)
								return true;
						return false;
					};

			REQUIRE(does_contains(25.1));
			REQUIRE(does_contains(40));
		}

		THEN("Every space should be thiner than dmax") {
			REQUIRE(w.mpm.meshlines[Y].size() > 2);
			for(auto it = next(begin(w.mpm.meshlines[Y])); it != end(w.mpm.meshlines[Y]); ++it)
				REQUIRE(distance((*prev(it))->coord, (*it)->coord) <= 4.0);
		}

		THEN("Every space should be [0.5; 2] times its adjacent spaces") {
			REQUIRE(w.mpm.meshlines[Y].size() > 2);
			for(auto it = next(begin(w.mpm.meshlines[Y]), 2); it != end(w.mpm.meshlines[Y]); ++it) {
				Coord a(distance((*prev(it, 2))->coord, (*prev(it))->coord));
				Coord b(distance((*prev(it))->coord, (*it)->coord));
				REQUIRE(a <= b * 2);
				REQUIRE(a >= b / 2);
			}
		}

		THEN("Every space should contain at least lmin * 2 + 1 lines") {
			auto has_enough_lines = [&w](Coord a, Coord b, size_t lmin) -> bool {
				Coord const c(mid(a, b));
				size_t lines = 0;
				for(auto const& it : w.mpm.meshlines[Y])
					if(it->coord >= a && it->coord <= b)
						++lines;
				UNSCOPED_INFO("lines : " << lines);
				return lines >= lmin * 2 + 1;
			};

			REQUIRE(has_enough_lines(10, 25, 2));
			REQUIRE(has_enough_lines(25, 25.1, 2));
			REQUIRE(has_enough_lines(25.1, 30, 2));
			REQUIRE(has_enough_lines(30, 40, 2));
		}
	}
}
