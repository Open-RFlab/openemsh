///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/global.hpp"
#include "domain/meshline_policy_manager.hpp"

#include "domain/conflicts/conflict_too_close_meshline_policies.hpp"

/// @test void ConflictTooCloseMeshlinePolicies::auto_solve(MeshlinePolicyManager& line_policy_manager)
///*****************************************************************************

//******************************************************************************
SCENARIO("void ConflictTooCloseMeshlinePolicies::auto_solve(MeshlinePolicyManager& line_policy_manager)", "[conflict_too_close_meshline_policies]") {
	Params params;
	MeshlinePolicyManager mpm(params, nullptr);

	GIVEN("A conflict between HALFS and HALFS meshline policies") {
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
		ConflictTooCloseMeshlinePolicies x(&a, &b);
		THEN("Should add a HALFS meshline policy in the meshline policy manager") {
			x.auto_solve(mpm);
			REQUIRE(mpm.line_policies[V].size() == 0);
			REQUIRE(mpm.line_policies[H].size() == 1);
			REQUIRE(mpm.line_policies[H][0].get() == x.solution);
			REQUIRE(mpm.line_policies[H][0]->origins.size() == 1);
			REQUIRE(mpm.line_policies[H][0]->origins[0] == &x);
			REQUIRE(mpm.line_policies[H][0]->policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.line_policies[H][0]->normal == Normal::NONE);
			REQUIRE(mpm.line_policies[H][0]->coord == 10.5);
			REQUIRE(mpm.line_policies[H][0]->is_enabled);
			REQUIRE(x.is_solved);
			REQUIRE(x.solution == mpm.line_policies[H][0].get());
		}
	}

	GIVEN("A conflict between HALFS and THIRDS meshline policies") {
		MeshlinePolicy a(
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::HALFS,
			Normal::NONE,
			params,
			10);
		MeshlinePolicy b(
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::THIRDS,
			Normal::YMIN,
			params,
			11);
		ConflictTooCloseMeshlinePolicies x(&a, &b);
		ConflictTooCloseMeshlinePolicies y(&b, &a);
		THEN("Should add a HALFS meshline policy in the meshline policy manager") {
			x.auto_solve(mpm);
			y.auto_solve(mpm);
			REQUIRE(mpm.line_policies[V].size() == 0);
			REQUIRE(mpm.line_policies[H].size() == 2);
			REQUIRE(mpm.line_policies[H][0].get() == x.solution);
			REQUIRE(mpm.line_policies[H][0]->origins.size() == 1);
			REQUIRE(mpm.line_policies[H][0]->origins[0] == &x);
			REQUIRE(mpm.line_policies[H][0]->policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.line_policies[H][0]->normal == Normal::NONE);
			REQUIRE(mpm.line_policies[H][0]->coord == 10.5);
			REQUIRE(mpm.line_policies[H][0]->is_enabled);
			REQUIRE(mpm.line_policies[H][1].get() == y.solution);
			REQUIRE(mpm.line_policies[H][1]->origins.size() == 1);
			REQUIRE(mpm.line_policies[H][1]->origins[0] == &y);
			REQUIRE(mpm.line_policies[H][1]->policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.line_policies[H][1]->normal == Normal::NONE);
			REQUIRE(mpm.line_policies[H][1]->coord == 10.5);
			REQUIRE(mpm.line_policies[H][1]->is_enabled);
			REQUIRE(x.is_solved);
			REQUIRE(y.is_solved);
			REQUIRE(x.solution == mpm.line_policies[H][0].get());
			REQUIRE(y.solution == mpm.line_policies[H][1].get());
		}
	}

	GIVEN("A conflict between THIRDS and THIRDS meshline policies with different normal") {
		MeshlinePolicy a(
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::THIRDS,
			Normal::YMIN,
			params,
			10);
		MeshlinePolicy b(
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::THIRDS,
			Normal::YMAX,
			params,
			11);
		MeshlinePolicy c(
			MeshlinePolicy::Axis::V,
			MeshlinePolicy::Policy::THIRDS,
			Normal::XMIN,
			params,
			10);
		MeshlinePolicy d(
			MeshlinePolicy::Axis::V,
			MeshlinePolicy::Policy::THIRDS,
			Normal::XMAX,
			params,
			11);
		ConflictTooCloseMeshlinePolicies x(&a, &b);
		ConflictTooCloseMeshlinePolicies y(&c, &d);
		THEN("Should add a HALFS meshline policy in the meshline policy manager") {
			x.auto_solve(mpm);
			y.auto_solve(mpm);
			REQUIRE(mpm.line_policies[V].size() == 1);
			REQUIRE(mpm.line_policies[H].size() == 1);
			REQUIRE(mpm.line_policies[H][0].get() == x.solution);
			REQUIRE(mpm.line_policies[H][0]->origins.size() == 1);
			REQUIRE(mpm.line_policies[H][0]->origins[0] == &x);
			REQUIRE(mpm.line_policies[H][0]->policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.line_policies[H][0]->normal == Normal::NONE);
			REQUIRE(mpm.line_policies[H][0]->coord == 10.5);
			REQUIRE(mpm.line_policies[H][0]->is_enabled);
			REQUIRE(mpm.line_policies[V][0].get() == y.solution);
			REQUIRE(mpm.line_policies[V][0]->origins.size() == 1);
			REQUIRE(mpm.line_policies[V][0]->origins[0] == &y);
			REQUIRE(mpm.line_policies[V][0]->policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.line_policies[V][0]->normal == Normal::NONE);
			REQUIRE(mpm.line_policies[V][0]->coord == 10.5);
			REQUIRE(mpm.line_policies[V][0]->is_enabled);
			REQUIRE(x.is_solved);
			REQUIRE(y.is_solved);
			REQUIRE(x.solution == mpm.line_policies[H][0].get());
			REQUIRE(y.solution == mpm.line_policies[V][0].get());
		}
	}

	GIVEN("A conflict between THIRDS and THIRDS meshline policies with same normal") {
		MeshlinePolicy a(
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::THIRDS,
			Normal::YMIN,
			params,
			10);
		MeshlinePolicy b(
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::THIRDS,
			Normal::YMIN,
			params,
			11);
		MeshlinePolicy c(
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::THIRDS,
			Normal::YMAX,
			params,
			10);
		MeshlinePolicy d(
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::THIRDS,
			Normal::YMAX,
			params,
			11);
		MeshlinePolicy e(
			MeshlinePolicy::Axis::V,
			MeshlinePolicy::Policy::THIRDS,
			Normal::XMIN,
			params,
			10);
		MeshlinePolicy f(
			MeshlinePolicy::Axis::V,
			MeshlinePolicy::Policy::THIRDS,
			Normal::XMIN,
			params,
			11);
		MeshlinePolicy g(
			MeshlinePolicy::Axis::V,
			MeshlinePolicy::Policy::THIRDS,
			Normal::XMAX,
			params,
			10);
		MeshlinePolicy h(
			MeshlinePolicy::Axis::V,
			MeshlinePolicy::Policy::THIRDS,
			Normal::XMAX,
			params,
			11);
		ConflictTooCloseMeshlinePolicies w(&a, &b);
		ConflictTooCloseMeshlinePolicies x(&c, &d);
		ConflictTooCloseMeshlinePolicies y(&e, &f);
		ConflictTooCloseMeshlinePolicies z(&g, &h);
		THEN("Should add a THIRDS meshline policy with the same normal in the meshline policy manager") {
			w.auto_solve(mpm);
			x.auto_solve(mpm);
			y.auto_solve(mpm);
			z.auto_solve(mpm);
			REQUIRE(mpm.line_policies[V].size() == 2);
			REQUIRE(mpm.line_policies[H].size() == 2);
			REQUIRE(mpm.line_policies[H][0].get() == w.solution);
			REQUIRE(mpm.line_policies[H][0]->origins.size() == 1);
			REQUIRE(mpm.line_policies[H][0]->origins[0] == &w);
			REQUIRE(mpm.line_policies[H][0]->policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(mpm.line_policies[H][0]->normal == Normal::YMIN);
			REQUIRE(mpm.line_policies[H][0]->coord == 10.5);
			REQUIRE(mpm.line_policies[H][0]->is_enabled);
			REQUIRE(mpm.line_policies[H][1].get() == x.solution);
			REQUIRE(mpm.line_policies[H][1]->origins.size() == 1);
			REQUIRE(mpm.line_policies[H][1]->origins[0] == &x);
			REQUIRE(mpm.line_policies[H][1]->policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(mpm.line_policies[H][1]->normal == Normal::YMAX);
			REQUIRE(mpm.line_policies[H][1]->coord == 10.5);
			REQUIRE(mpm.line_policies[H][1]->is_enabled);
			REQUIRE(mpm.line_policies[V][0].get() == y.solution);
			REQUIRE(mpm.line_policies[V][0]->origins.size() == 1);
			REQUIRE(mpm.line_policies[V][0]->origins[0] == &y);
			REQUIRE(mpm.line_policies[V][0]->policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(mpm.line_policies[V][0]->normal == Normal::XMIN);
			REQUIRE(mpm.line_policies[V][0]->coord == 10.5);
			REQUIRE(mpm.line_policies[V][0]->is_enabled);
			REQUIRE(mpm.line_policies[V][1].get() == z.solution);
			REQUIRE(mpm.line_policies[V][1]->origins.size() == 1);
			REQUIRE(mpm.line_policies[V][1]->origins[0] == &z);
			REQUIRE(mpm.line_policies[V][1]->policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(mpm.line_policies[V][1]->normal == Normal::XMAX);
			REQUIRE(mpm.line_policies[V][1]->coord == 10.5);
			REQUIRE(mpm.line_policies[V][1]->is_enabled);
			REQUIRE(w.is_solved);
			REQUIRE(x.is_solved);
			REQUIRE(y.is_solved);
			REQUIRE(z.is_solved);
			REQUIRE(w.solution == mpm.line_policies[H][0].get());
			REQUIRE(x.solution == mpm.line_policies[H][1].get());
			REQUIRE(y.solution == mpm.line_policies[V][0].get());
			REQUIRE(z.solution == mpm.line_policies[V][1].get());
		}
	}

	GIVEN("A conflict between meshline policies of different axes") {
		MeshlinePolicy a(
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::HALFS,
			Normal::NONE,
			params,
			10);
		MeshlinePolicy b(
			MeshlinePolicy::Axis::V,
			MeshlinePolicy::Policy::HALFS,
			Normal::NONE,
			params,
			11);
		ConflictTooCloseMeshlinePolicies x(&a, &b);
		THEN("Should add nothing to the meshline policy manager") {
			x.auto_solve(mpm);
			REQUIRE(mpm.line_policies[V].size() == 0);
			REQUIRE(mpm.line_policies[H].size() == 0);
			REQUIRE_FALSE(x.is_solved);
			REQUIRE(x.solution == nullptr);
		}
	}

	GIVEN("A conflict with ONELINE meshline policies") {
		MeshlinePolicy a(
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::HALFS,
			Normal::NONE,
			params,
			10);
		MeshlinePolicy b(
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::THIRDS,
			Normal::NONE,
			params,
			10);
		MeshlinePolicy c(
			MeshlinePolicy::Axis::H,
			MeshlinePolicy::Policy::ONELINE,
			Normal::NONE,
			params,
			11);
		ConflictTooCloseMeshlinePolicies x(&a, &c);
		ConflictTooCloseMeshlinePolicies y(&b, &c);
		THEN("Should add nothing to the meshline policy manager") {
			x.auto_solve(mpm);
			y.auto_solve(mpm);
			REQUIRE(mpm.line_policies[V].size() == 0);
			REQUIRE(mpm.line_policies[H].size() == 0);
			REQUIRE_FALSE(x.is_solved);
			REQUIRE_FALSE(y.is_solved);
			REQUIRE(x.solution == nullptr);
			REQUIRE(y.solution == nullptr);
		}
	}
}
