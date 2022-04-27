///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/point.hpp"

#include "domain/meshline_policy_manager.hpp"

/// @test GridAxis cast(MeshlinePolicy::Axis const a)
/// @test MeshlinePolicy* MeshlinePolicyManager::add_meshline_policy(
///       	IMeshLineOrigin* origin,
///       	MeshlinePolicy::Axis const axis,
///       	MeshlinePolicy::Policy const policy,
///       	Normal const normal,
///       	Coord const coord,
///       	bool const is_enabled)
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
		MeshlinePolicyManager mpm(params);
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
