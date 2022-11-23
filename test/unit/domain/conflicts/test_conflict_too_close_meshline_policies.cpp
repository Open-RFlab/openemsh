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
		ConflictTooCloseMeshlinePolicies x(Y, &a, &b);
		THEN("Should add a HALFS meshline policy in the meshline policy manager") {
			x.auto_solve(mpm);
			REQUIRE(mpm.line_policies[X].size() == 0);
			REQUIRE(mpm.line_policies[Y].size() == 1);
			REQUIRE(mpm.line_policies[Y][0].get() == x.solution);
			REQUIRE(mpm.line_policies[Y][0]->origins.size() == 1);
			REQUIRE(mpm.line_policies[Y][0]->origins[0] == &x);
			REQUIRE(mpm.line_policies[Y][0]->policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.line_policies[Y][0]->normal == MeshlinePolicy::Normal::NONE);
			REQUIRE(mpm.line_policies[Y][0]->coord == 10.5);
			REQUIRE(mpm.line_policies[Y][0]->is_enabled);
			REQUIRE(x.is_solved);
			REQUIRE(x.solution == mpm.line_policies[Y][0].get());
		}
	}

	GIVEN("A conflict between HALFS and THIRDS meshline policies") {
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			params,
			10);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			params,
			11);
		ConflictTooCloseMeshlinePolicies x(Y, &a, &b);
		ConflictTooCloseMeshlinePolicies y(Y, &b, &a);
		THEN("Should add a HALFS meshline policy in the meshline policy manager") {
			x.auto_solve(mpm);
			y.auto_solve(mpm);
			REQUIRE(mpm.line_policies[X].size() == 0);
			REQUIRE(mpm.line_policies[Y].size() == 2);
			REQUIRE(mpm.line_policies[Y][0].get() == x.solution);
			REQUIRE(mpm.line_policies[Y][0]->origins.size() == 1);
			REQUIRE(mpm.line_policies[Y][0]->origins[0] == &x);
			REQUIRE(mpm.line_policies[Y][0]->policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.line_policies[Y][0]->normal == MeshlinePolicy::Normal::NONE);
			REQUIRE(mpm.line_policies[Y][0]->coord == 10.5);
			REQUIRE(mpm.line_policies[Y][0]->is_enabled);
			REQUIRE(mpm.line_policies[Y][1].get() == y.solution);
			REQUIRE(mpm.line_policies[Y][1]->origins.size() == 1);
			REQUIRE(mpm.line_policies[Y][1]->origins[0] == &y);
			REQUIRE(mpm.line_policies[Y][1]->policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.line_policies[Y][1]->normal == MeshlinePolicy::Normal::NONE);
			REQUIRE(mpm.line_policies[Y][1]->coord == 10.5);
			REQUIRE(mpm.line_policies[Y][1]->is_enabled);
			REQUIRE(x.is_solved);
			REQUIRE(y.is_solved);
			REQUIRE(x.solution == mpm.line_policies[Y][0].get());
			REQUIRE(y.solution == mpm.line_policies[Y][1].get());
		}
	}

	GIVEN("A conflict between THIRDS and THIRDS meshline policies with different normal") {
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			params,
			10);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MAX,
			params,
			11);
		MeshlinePolicy c(
			X,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			params,
			10);
		MeshlinePolicy d(
			X,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MAX,
			params,
			11);
		ConflictTooCloseMeshlinePolicies x(Y, &a, &b);
		ConflictTooCloseMeshlinePolicies y(X, &c, &d);
		THEN("Should add a HALFS meshline policy in the meshline policy manager") {
			x.auto_solve(mpm);
			y.auto_solve(mpm);
			REQUIRE(mpm.line_policies[X].size() == 1);
			REQUIRE(mpm.line_policies[Y].size() == 1);
			REQUIRE(mpm.line_policies[Y][0].get() == x.solution);
			REQUIRE(mpm.line_policies[Y][0]->origins.size() == 1);
			REQUIRE(mpm.line_policies[Y][0]->origins[0] == &x);
			REQUIRE(mpm.line_policies[Y][0]->policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.line_policies[Y][0]->normal == MeshlinePolicy::Normal::NONE);
			REQUIRE(mpm.line_policies[Y][0]->coord == 10.5);
			REQUIRE(mpm.line_policies[Y][0]->is_enabled);
			REQUIRE(mpm.line_policies[X][0].get() == y.solution);
			REQUIRE(mpm.line_policies[X][0]->origins.size() == 1);
			REQUIRE(mpm.line_policies[X][0]->origins[0] == &y);
			REQUIRE(mpm.line_policies[X][0]->policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.line_policies[X][0]->normal == MeshlinePolicy::Normal::NONE);
			REQUIRE(mpm.line_policies[X][0]->coord == 10.5);
			REQUIRE(mpm.line_policies[X][0]->is_enabled);
			REQUIRE(x.is_solved);
			REQUIRE(y.is_solved);
			REQUIRE(x.solution == mpm.line_policies[Y][0].get());
			REQUIRE(y.solution == mpm.line_policies[X][0].get());
		}
	}

	GIVEN("A conflict between THIRDS and THIRDS meshline policies with same normal") {
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			params,
			10);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			params,
			11);
		MeshlinePolicy c(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MAX,
			params,
			10);
		MeshlinePolicy d(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MAX,
			params,
			11);
		MeshlinePolicy e(
			X,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			params,
			10);
		MeshlinePolicy f(
			X,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			params,
			11);
		MeshlinePolicy g(
			X,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MAX,
			params,
			10);
		MeshlinePolicy h(
			X,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MAX,
			params,
			11);
		ConflictTooCloseMeshlinePolicies w(Y, &a, &b);
		ConflictTooCloseMeshlinePolicies x(Y, &c, &d);
		ConflictTooCloseMeshlinePolicies y(X, &e, &f);
		ConflictTooCloseMeshlinePolicies z(X, &g, &h);
		THEN("Should add a THIRDS meshline policy with the same normal in the meshline policy manager") {
			w.auto_solve(mpm);
			x.auto_solve(mpm);
			y.auto_solve(mpm);
			z.auto_solve(mpm);
			REQUIRE(mpm.line_policies[X].size() == 2);
			REQUIRE(mpm.line_policies[Y].size() == 2);
			REQUIRE(mpm.line_policies[Y][0].get() == w.solution);
			REQUIRE(mpm.line_policies[Y][0]->origins.size() == 1);
			REQUIRE(mpm.line_policies[Y][0]->origins[0] == &w);
			REQUIRE(mpm.line_policies[Y][0]->policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(mpm.line_policies[Y][0]->normal == MeshlinePolicy::Normal::MIN);
			REQUIRE(mpm.line_policies[Y][0]->coord == 10.5);
			REQUIRE(mpm.line_policies[Y][0]->is_enabled);
			REQUIRE(mpm.line_policies[Y][1].get() == x.solution);
			REQUIRE(mpm.line_policies[Y][1]->origins.size() == 1);
			REQUIRE(mpm.line_policies[Y][1]->origins[0] == &x);
			REQUIRE(mpm.line_policies[Y][1]->policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(mpm.line_policies[Y][1]->normal == MeshlinePolicy::Normal::MAX);
			REQUIRE(mpm.line_policies[Y][1]->coord == 10.5);
			REQUIRE(mpm.line_policies[Y][1]->is_enabled);
			REQUIRE(mpm.line_policies[X][0].get() == y.solution);
			REQUIRE(mpm.line_policies[X][0]->origins.size() == 1);
			REQUIRE(mpm.line_policies[X][0]->origins[0] == &y);
			REQUIRE(mpm.line_policies[X][0]->policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(mpm.line_policies[X][0]->normal == MeshlinePolicy::Normal::MIN);
			REQUIRE(mpm.line_policies[X][0]->coord == 10.5);
			REQUIRE(mpm.line_policies[X][0]->is_enabled);
			REQUIRE(mpm.line_policies[X][1].get() == z.solution);
			REQUIRE(mpm.line_policies[X][1]->origins.size() == 1);
			REQUIRE(mpm.line_policies[X][1]->origins[0] == &z);
			REQUIRE(mpm.line_policies[X][1]->policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(mpm.line_policies[X][1]->normal == MeshlinePolicy::Normal::MAX);
			REQUIRE(mpm.line_policies[X][1]->coord == 10.5);
			REQUIRE(mpm.line_policies[X][1]->is_enabled);
			REQUIRE(w.is_solved);
			REQUIRE(x.is_solved);
			REQUIRE(y.is_solved);
			REQUIRE(z.is_solved);
			REQUIRE(w.solution == mpm.line_policies[Y][0].get());
			REQUIRE(x.solution == mpm.line_policies[Y][1].get());
			REQUIRE(y.solution == mpm.line_policies[X][0].get());
			REQUIRE(z.solution == mpm.line_policies[X][1].get());
		}
	}

	GIVEN("A conflict between meshline policies of different axes") {
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			params,
			10);
		MeshlinePolicy b(
			X,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			params,
			11);
		ConflictTooCloseMeshlinePolicies x(X, &a, &b);
		THEN("Should add nothing to the meshline policy manager") {
			x.auto_solve(mpm);
			REQUIRE(mpm.line_policies[X].size() == 0);
			REQUIRE(mpm.line_policies[Y].size() == 0);
			REQUIRE_FALSE(x.is_solved);
			REQUIRE(x.solution == nullptr);
		}
	}

	GIVEN("A conflict with ONELINE meshline policies") {
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			params,
			10);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::NONE,
			params,
			10);
		MeshlinePolicy c(
			Y,
			MeshlinePolicy::Policy::ONELINE,
			MeshlinePolicy::Normal::NONE,
			params,
			11);
		ConflictTooCloseMeshlinePolicies x(X, &a, &c);
		ConflictTooCloseMeshlinePolicies y(X, &b, &c);
		THEN("Should add nothing to the meshline policy manager") {
			x.auto_solve(mpm);
			y.auto_solve(mpm);
			REQUIRE(mpm.line_policies[X].size() == 0);
			REQUIRE(mpm.line_policies[Y].size() == 0);
			REQUIRE_FALSE(x.is_solved);
			REQUIRE_FALSE(y.is_solved);
			REQUIRE(x.solution == nullptr);
			REQUIRE(y.solution == nullptr);
		}
	}
}
