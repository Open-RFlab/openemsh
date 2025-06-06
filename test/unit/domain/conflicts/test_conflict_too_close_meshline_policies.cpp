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

using namespace domain;

//******************************************************************************
SCENARIO("void ConflictTooCloseMeshlinePolicies::auto_solve(MeshlinePolicyManager& line_policy_manager)", "[conflict_too_close_meshline_policies]") {
	Timepoint* t = Caretaker::singleton().get_history_root();
	GlobalParams params(t);
	MeshlinePolicyManager mpm(&params, t);

	GIVEN("A conflict between HALFS and HALFS meshline policies") {
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			&params,
			10,
			t);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			&params,
			11,
			t);
		ConflictTooCloseMeshlinePolicies x(Y, &a, &b, t);
		THEN("Should add a HALFS meshline policy in the meshline policy manager") {
			x.auto_solve(mpm);
			REQUIRE(mpm.get_current_state().line_policies[X].size() == 0);
			REQUIRE(mpm.get_current_state().line_policies[Y].size() == 1);
			REQUIRE(mpm.get_current_state().line_policies[Y][0].get() == x.get_current_state().solution);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins.size() == 1);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins[0] == &x);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().normal == MeshlinePolicy::Normal::NONE);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->coord == 10.5);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().is_enabled);
			REQUIRE(x.get_current_state().is_solved);
			REQUIRE(x.get_current_state().solution == mpm.get_current_state().line_policies[Y][0].get());
		}
	}

	GIVEN("A conflict between HALFS and THIRDS meshline policies") {
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			&params,
			10,
			t);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			&params,
			11,
			t);
		ConflictTooCloseMeshlinePolicies x(Y, &a, &b, t);
		ConflictTooCloseMeshlinePolicies y(Y, &b, &a, t);
		THEN("Should add a HALFS meshline policy in the meshline policy manager") {
			x.auto_solve(mpm);
			y.auto_solve(mpm);
			REQUIRE(mpm.get_current_state().line_policies[X].size() == 0);
			REQUIRE(mpm.get_current_state().line_policies[Y].size() == 2);
			REQUIRE(mpm.get_current_state().line_policies[Y][0].get() == x.get_current_state().solution);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins.size() == 1);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins[0] == &x);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().normal == MeshlinePolicy::Normal::NONE);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->coord == 10.5);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().is_enabled);
			REQUIRE(mpm.get_current_state().line_policies[Y][1].get() == y.get_current_state().solution);
			REQUIRE(mpm.get_current_state().line_policies[Y][1]->get_current_state().origins.size() == 1);
			REQUIRE(mpm.get_current_state().line_policies[Y][1]->get_current_state().origins[0] == &y);
			REQUIRE(mpm.get_current_state().line_policies[Y][1]->get_current_state().policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.get_current_state().line_policies[Y][1]->get_current_state().normal == MeshlinePolicy::Normal::NONE);
			REQUIRE(mpm.get_current_state().line_policies[Y][1]->coord == 10.5);
			REQUIRE(mpm.get_current_state().line_policies[Y][1]->get_current_state().is_enabled);
			REQUIRE(x.get_current_state().is_solved);
			REQUIRE(y.get_current_state().is_solved);
			REQUIRE(x.get_current_state().solution == mpm.get_current_state().line_policies[Y][0].get());
			REQUIRE(y.get_current_state().solution == mpm.get_current_state().line_policies[Y][1].get());
		}
	}

	GIVEN("A conflict between THIRDS and THIRDS meshline policies with different normal") {
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			&params,
			10,
			t);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MAX,
			&params,
			11,
			t);
		MeshlinePolicy c(
			X,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			&params,
			10,
			t);
		MeshlinePolicy d(
			X,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MAX,
			&params,
			11,
			t);
		ConflictTooCloseMeshlinePolicies x(Y, &a, &b, t);
		ConflictTooCloseMeshlinePolicies y(X, &c, &d, t);
		THEN("Should add a HALFS meshline policy in the meshline policy manager") {
			x.auto_solve(mpm);
			y.auto_solve(mpm);
			REQUIRE(mpm.get_current_state().line_policies[X].size() == 1);
			REQUIRE(mpm.get_current_state().line_policies[Y].size() == 1);
			REQUIRE(mpm.get_current_state().line_policies[Y][0].get() == x.get_current_state().solution);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins.size() == 1);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins[0] == &x);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().normal == MeshlinePolicy::Normal::NONE);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->coord == 10.5);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().is_enabled);
			REQUIRE(mpm.get_current_state().line_policies[X][0].get() == y.get_current_state().solution);
			REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins.size() == 1);
			REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins[0] == &y);
			REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().policy == MeshlinePolicy::Policy::HALFS);
			REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().normal == MeshlinePolicy::Normal::NONE);
			REQUIRE(mpm.get_current_state().line_policies[X][0]->coord == 10.5);
			REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().is_enabled);
			REQUIRE(x.get_current_state().is_solved);
			REQUIRE(y.get_current_state().is_solved);
			REQUIRE(x.get_current_state().solution == mpm.get_current_state().line_policies[Y][0].get());
			REQUIRE(y.get_current_state().solution == mpm.get_current_state().line_policies[X][0].get());
		}
	}

	GIVEN("A conflict between THIRDS and THIRDS meshline policies with same normal") {
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			&params,
			10,
			t);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			&params,
			11,
			t);
		MeshlinePolicy c(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MAX,
			&params,
			10,
			t);
		MeshlinePolicy d(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MAX,
			&params,
			11,
			t);
		MeshlinePolicy e(
			X,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			&params,
			10,
			t);
		MeshlinePolicy f(
			X,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MIN,
			&params,
			11,
			t);
		MeshlinePolicy g(
			X,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MAX,
			&params,
			10,
			t);
		MeshlinePolicy h(
			X,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::MAX,
			&params,
			11,
			t);
		ConflictTooCloseMeshlinePolicies w(Y, &a, &b, t);
		ConflictTooCloseMeshlinePolicies x(Y, &c, &d, t);
		ConflictTooCloseMeshlinePolicies y(X, &e, &f, t);
		ConflictTooCloseMeshlinePolicies z(X, &g, &h, t);
		THEN("Should add a THIRDS meshline policy with the same normal in the meshline policy manager") {
			w.auto_solve(mpm);
			x.auto_solve(mpm);
			y.auto_solve(mpm);
			z.auto_solve(mpm);
			REQUIRE(mpm.get_current_state().line_policies[X].size() == 2);
			REQUIRE(mpm.get_current_state().line_policies[Y].size() == 2);
			REQUIRE(mpm.get_current_state().line_policies[Y][0].get() == w.get_current_state().solution);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins.size() == 1);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins[0] == &w);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().normal == MeshlinePolicy::Normal::MIN);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->coord == 10.5);
			REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().is_enabled);
			REQUIRE(mpm.get_current_state().line_policies[Y][1].get() == x.get_current_state().solution);
			REQUIRE(mpm.get_current_state().line_policies[Y][1]->get_current_state().origins.size() == 1);
			REQUIRE(mpm.get_current_state().line_policies[Y][1]->get_current_state().origins[0] == &x);
			REQUIRE(mpm.get_current_state().line_policies[Y][1]->get_current_state().policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(mpm.get_current_state().line_policies[Y][1]->get_current_state().normal == MeshlinePolicy::Normal::MAX);
			REQUIRE(mpm.get_current_state().line_policies[Y][1]->coord == 10.5);
			REQUIRE(mpm.get_current_state().line_policies[Y][1]->get_current_state().is_enabled);
			REQUIRE(mpm.get_current_state().line_policies[X][0].get() == y.get_current_state().solution);
			REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins.size() == 1);
			REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins[0] == &y);
			REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().normal == MeshlinePolicy::Normal::MIN);
			REQUIRE(mpm.get_current_state().line_policies[X][0]->coord == 10.5);
			REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().is_enabled);
			REQUIRE(mpm.get_current_state().line_policies[X][1].get() == z.get_current_state().solution);
			REQUIRE(mpm.get_current_state().line_policies[X][1]->get_current_state().origins.size() == 1);
			REQUIRE(mpm.get_current_state().line_policies[X][1]->get_current_state().origins[0] == &z);
			REQUIRE(mpm.get_current_state().line_policies[X][1]->get_current_state().policy == MeshlinePolicy::Policy::THIRDS);
			REQUIRE(mpm.get_current_state().line_policies[X][1]->get_current_state().normal == MeshlinePolicy::Normal::MAX);
			REQUIRE(mpm.get_current_state().line_policies[X][1]->coord == 10.5);
			REQUIRE(mpm.get_current_state().line_policies[X][1]->get_current_state().is_enabled);
			REQUIRE(w.get_current_state().is_solved);
			REQUIRE(x.get_current_state().is_solved);
			REQUIRE(y.get_current_state().is_solved);
			REQUIRE(z.get_current_state().is_solved);
			REQUIRE(w.get_current_state().solution == mpm.get_current_state().line_policies[Y][0].get());
			REQUIRE(x.get_current_state().solution == mpm.get_current_state().line_policies[Y][1].get());
			REQUIRE(y.get_current_state().solution == mpm.get_current_state().line_policies[X][0].get());
			REQUIRE(z.get_current_state().solution == mpm.get_current_state().line_policies[X][1].get());
		}
	}

	GIVEN("A conflict between meshline policies of different axes") {
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			&params,
			10,
			t);
		MeshlinePolicy b(
			X,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			&params,
			11,
			t);
		ConflictTooCloseMeshlinePolicies x(X, &a, &b, t);
		THEN("Should add nothing to the meshline policy manager") {
			x.auto_solve(mpm);
			REQUIRE(mpm.get_current_state().line_policies[X].size() == 0);
			REQUIRE(mpm.get_current_state().line_policies[Y].size() == 0);
			REQUIRE_FALSE(x.get_current_state().is_solved);
			REQUIRE(x.get_current_state().solution == nullptr);
		}
	}

	GIVEN("A conflict with ONELINE meshline policies") {
		MeshlinePolicy a(
			Y,
			MeshlinePolicy::Policy::HALFS,
			MeshlinePolicy::Normal::NONE,
			&params,
			10,
			t);
		MeshlinePolicy b(
			Y,
			MeshlinePolicy::Policy::THIRDS,
			MeshlinePolicy::Normal::NONE,
			&params,
			10,
			t);
		MeshlinePolicy c(
			Y,
			MeshlinePolicy::Policy::ONELINE,
			MeshlinePolicy::Normal::NONE,
			&params,
			11,
			t);
		ConflictTooCloseMeshlinePolicies x(X, &a, &c, t);
		ConflictTooCloseMeshlinePolicies y(X, &b, &c, t);
		THEN("Should add nothing to the meshline policy manager") {
			x.auto_solve(mpm);
			y.auto_solve(mpm);
			REQUIRE(mpm.get_current_state().line_policies[X].size() == 0);
			REQUIRE(mpm.get_current_state().line_policies[Y].size() == 0);
			REQUIRE_FALSE(x.get_current_state().is_solved);
			REQUIRE_FALSE(y.get_current_state().is_solved);
			REQUIRE(x.get_current_state().solution == nullptr);
			REQUIRE(y.get_current_state().solution == nullptr);
		}
	}
}
