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

/// @test GridAxis cast(MeshlinePolicy::Axis const a)
/// @test MeshlinePolicy* MeshlinePolicyManager::add_meshline_policy(
///       	IMeshLineOrigin* origin,
///       	MeshlinePolicy::Axis const axis,
///       	MeshlinePolicy::Policy const policy,
///       	Normal const normal,
///       	Coord const coord,
///       	bool const is_enabled)
/// @test optional<array<MeshlinePolicy*, 2>> detect_closest_meshline_policies(
///       	vector<MeshlinePolicy*> dimension,
///       	Coord proximity_limit)
/// @test void MeshlinePolicyManager::detect_and_solve_too_close_meshline_policies()
///*****************************************************************************

//******************************************************************************
SCENARIO("GridAxis cast(MeshlinePolicy::Axis const a)", "[meshline_policy_manager]") {
	GIVEN("A MeshLinePolicy::Axis enum class") {
		WHEN("MeshlinePolicy::Axis::H") {
			GridAxis a = cast(MeshlinePolicy::Axis::H);
			THEN("Should return H") {
				REQUIRE(a == H);
			}
		}
		WHEN("a == MeshlinePolicy::Axis::V") {
			GridAxis a = cast(MeshlinePolicy::Axis::V);
			THEN("Should return V") {
				REQUIRE(a == V);
			}
		}
	}
}

//******************************************************************************
SCENARIO("MeshlinePolicy* MeshlinePolicyManager::add_meshline_policy( \
IMeshLineOrigin* origin, \
MeshlinePolicy::Axis const axis, \
MeshlinePolicy::Policy const policy, \
Normal const normal, \
Coord const coord, \
bool const is_enabled)", "[meshline_policy_manager]") {
	GIVEN("A meshline policy manager") {
		Params params;
		MeshlinePolicyManager mpm(params, nullptr);
		Point e0(1, 1), e1(1, 3);
		Edge e(&e0, &e1);
		WHEN("We add a meshline policy with incoherent axis and normal parameters") {
			MeshlinePolicy* m = mpm.add_meshline_policy(&e, MeshlinePolicy::Axis::H, MeshlinePolicy::Policy::THIRDS, Normal::XMIN, 1);
			MeshlinePolicy* n = mpm.add_meshline_policy(&e, MeshlinePolicy::Axis::H, MeshlinePolicy::Policy::THIRDS, Normal::XMAX, 1);
			MeshlinePolicy* o = mpm.add_meshline_policy(&e, MeshlinePolicy::Axis::V, MeshlinePolicy::Policy::THIRDS, Normal::YMIN, 1);
			MeshlinePolicy* p = mpm.add_meshline_policy(&e, MeshlinePolicy::Axis::V, MeshlinePolicy::Policy::THIRDS, Normal::YMAX, 1);
			THEN("No meshline policy should be added nor returned") {
				REQUIRE(mpm.line_policies[H].size() == 0);
				REQUIRE(mpm.line_policies[V].size() == 0);
				REQUIRE(m == nullptr);
				REQUIRE(n == nullptr);
				REQUIRE(o == nullptr);
				REQUIRE(p == nullptr);
			}
		}

		WHEN("We add a meshline policy with incoherent policy and normal parameters") {
			MeshlinePolicy* m = mpm.add_meshline_policy(
				&e, MeshlinePolicy::Axis::H, MeshlinePolicy::Policy::THIRDS, Normal::NONE, 1);
			MeshlinePolicy* n = mpm.add_meshline_policy(
				&e, MeshlinePolicy::Axis::H, MeshlinePolicy::Policy::ONELINE, Normal::YMIN, 1);
			MeshlinePolicy* o = mpm.add_meshline_policy(
				&e, MeshlinePolicy::Axis::H, MeshlinePolicy::Policy::ONELINE, Normal::YMAX, 1);
			MeshlinePolicy* p = mpm.add_meshline_policy(
				&e, MeshlinePolicy::Axis::V, MeshlinePolicy::Policy::ONELINE, Normal::XMIN, 1);
			MeshlinePolicy* q = mpm.add_meshline_policy(
				&e, MeshlinePolicy::Axis::V, MeshlinePolicy::Policy::ONELINE, Normal::XMAX, 1);
			MeshlinePolicy* r = mpm.add_meshline_policy(
				&e, MeshlinePolicy::Axis::H, MeshlinePolicy::Policy::HALFS, Normal::YMIN, 1);
			MeshlinePolicy* s = mpm.add_meshline_policy(
				&e, MeshlinePolicy::Axis::H, MeshlinePolicy::Policy::HALFS, Normal::YMAX, 1);
			MeshlinePolicy* t = mpm.add_meshline_policy(
				&e, MeshlinePolicy::Axis::V, MeshlinePolicy::Policy::HALFS, Normal::XMIN, 1);
			MeshlinePolicy* u = mpm.add_meshline_policy(
				&e, MeshlinePolicy::Axis::V, MeshlinePolicy::Policy::HALFS, Normal::XMAX, 1);
			MeshlinePolicy* v = mpm.add_meshline_policy(
				&e, MeshlinePolicy::Axis::H, MeshlinePolicy::Policy::INTERVAL, Normal::YMIN, 1);
			MeshlinePolicy* w = mpm.add_meshline_policy(
				&e, MeshlinePolicy::Axis::H, MeshlinePolicy::Policy::INTERVAL, Normal::YMAX, 1);
			MeshlinePolicy* x = mpm.add_meshline_policy(
				&e, MeshlinePolicy::Axis::V, MeshlinePolicy::Policy::INTERVAL, Normal::XMIN, 1);
			MeshlinePolicy* y = mpm.add_meshline_policy(
				&e, MeshlinePolicy::Axis::V, MeshlinePolicy::Policy::INTERVAL, Normal::XMAX, 1);
			THEN("No meshline policy should be added nor returned") {
				REQUIRE(mpm.line_policies[H].size() == 0);
				REQUIRE(mpm.line_policies[V].size() == 0);
				REQUIRE(m == nullptr);
				REQUIRE(n == nullptr);
				REQUIRE(o == nullptr);
				REQUIRE(p == nullptr);
				REQUIRE(q == nullptr);
				REQUIRE(r == nullptr);
				REQUIRE(s == nullptr);
				REQUIRE(t == nullptr);
				REQUIRE(u == nullptr);
				REQUIRE(v == nullptr);
				REQUIRE(w == nullptr);
				REQUIRE(x == nullptr);
				REQUIRE(y == nullptr);
			}
		}

		WHEN("We add a random meshline policy with coherent parameters") {
			MeshlinePolicy* m = mpm.add_meshline_policy(
				&e, MeshlinePolicy::Axis::V, MeshlinePolicy::Policy::THIRDS, Normal::XMAX, 1);
			THEN("Should add a meshline policy to the inner container and return a pointer to it") {
				REQUIRE(mpm.line_policies[H].size() == 0);
				REQUIRE(mpm.line_policies[V].size() == 1);
				REQUIRE(mpm.line_policies[V][0].get() == m);
				AND_THEN("All the meshline policy properties should be correct") {
					REQUIRE(m->origins.size() == 1);
					REQUIRE(m->origins[0] == &e);
					REQUIRE(m->axis == MeshlinePolicy::Axis::V);
					REQUIRE(m->policy == MeshlinePolicy::Policy::THIRDS);
					REQUIRE(m->normal == Normal::XMAX);
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
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
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
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
				params,
				10);
			MeshlinePolicy b(
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
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
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
				params,
				10);
			MeshlinePolicy b(
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
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
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
				params,
				10);
			MeshlinePolicy b(
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
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
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
				params,
				10);
			MeshlinePolicy b(
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
				params,
				10.5);
			MeshlinePolicy c(
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
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
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
				params,
				11.2);
			MeshlinePolicy b(
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
				params,
				10.8);
			MeshlinePolicy c(
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
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
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
				params,
				10,
				false);
			MeshlinePolicy b(
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
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
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::ONELINE,
				Normal::NONE,
				params,
				10);
			MeshlinePolicy b(
				MeshlinePolicy::Axis::H,
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
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
		Edge e(&e0, &e1);

		// 10  10,2  10,5   11,1  11,7
		//   10,1    10,5   11,1  11,7
		//       10,3       11,1  11,7
		//       10,3          11,4

		w.mpm.add_meshline_policy(
			&e,
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::HALFS,
			Normal::NONE,
			10);
		w.mpm.add_meshline_policy(
			&e,
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::HALFS,
			Normal::NONE,
			10.2);
		w.mpm.add_meshline_policy(
			&e,
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::HALFS,
			Normal::NONE,
			10.5);
		w.mpm.add_meshline_policy(
			&e,
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::HALFS,
			Normal::NONE,
			11.1);
		w.mpm.add_meshline_policy(
			&e,
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::HALFS,
			Normal::NONE,
			11.7);

		THEN("Meshline policies should be recursively merged till any is closer than the proximity limit") {
			w.mpm.detect_and_solve_too_close_meshline_policies();
			REQUIRE(w.mpm.line_policies[V].size() == 0);
			REQUIRE(w.mpm.line_policies[H].size() == 8);
			REQUIRE(w.mpm.line_policies[H][0]->coord == 10);
			REQUIRE_FALSE(w.mpm.line_policies[H][0]->is_enabled);
			REQUIRE(w.mpm.line_policies[H][1]->coord == 10.2);
			REQUIRE_FALSE(w.mpm.line_policies[H][1]->is_enabled);
			REQUIRE(w.mpm.line_policies[H][2]->coord == 10.5);
			REQUIRE_FALSE(w.mpm.line_policies[H][2]->is_enabled);
			REQUIRE(w.mpm.line_policies[H][3]->coord == 11.1);
			REQUIRE_FALSE(w.mpm.line_policies[H][3]->is_enabled);
			REQUIRE(w.mpm.line_policies[H][4]->coord == 11.7);
			REQUIRE_FALSE(w.mpm.line_policies[H][4]->is_enabled);
			REQUIRE(w.mpm.line_policies[H][5]->coord == 10.1);
			REQUIRE_FALSE(w.mpm.line_policies[H][5]->is_enabled);
			REQUIRE(w.mpm.line_policies[H][6]->coord == 10.3);
			REQUIRE(w.mpm.line_policies[H][6]->is_enabled);
			REQUIRE(w.mpm.line_policies[H][7]->coord == 11.4);
			REQUIRE(w.mpm.line_policies[H][7]->is_enabled);
		}
	}
}
