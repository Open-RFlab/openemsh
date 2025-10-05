///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/point.hpp"
#include "domain/meshline_policy_manager.hpp"

#include "domain/conflicts/conflict_colinear_edges.hpp"

/// @test void ConflictColinearEdges::auto_solve(MeshlinePolicyManager& line_policy_manager)
///*****************************************************************************

using namespace domain;

//******************************************************************************
SCENARIO("void ConflictColinearEdges::auto_solve(MeshlinePolicyManager& line_policy_manager)", "[conflict_colinear_edges]") {
	Timepoint* t = Caretaker::singleton().get_history_root();
	GIVEN("A conflict about four colinear vertical edges with mixed normals") {
		GlobalParams params(t);
		MeshlinePolicyManager mpm(&params, t);
		Point a0(1, 1), a1(1, 2);
		Point b0(1, 3), b1(1, 4);
		Point c0(1, 5), c1(1, 6);
		Point d0(1, 7), d1(1, 8);
		Edge a(XY, &a0, &a1, t), b(XY, &b0, &b1, t), c(XY, &c0, &c1, t), d(XY, &d0, &d1, t);
		a.normal = Normal::XMIN;
		b.normal = Normal::XMIN;
		c.normal = Normal::XMAX;
		d.normal = Normal::XMAX;
		ConflictColinearEdges cce(X, &a, &b, t);
		cce.append(&c);
		cce.append(&d);
		WHEN("Two edges normals are Normal::XMIN and two are Normal::XMAX") {
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			auto state_c = c.get_current_state();
			auto state_d = d.get_current_state();
			state_a.to_mesh = true;
			state_b.to_mesh = true;
			state_c.to_mesh = true;
			state_d.to_mesh = true;
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			c.set_next_state(state_c);
			d.set_next_state(state_d);
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.get_current_state().line_policies[Y].size() == 0);
				REQUIRE(mpm.get_current_state().line_policies[X].size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0].get() == cce.get_current_state().solution);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins.size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins[0] == &cce);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().policy == MeshlinePolicy::Policy::HALFS);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().normal == MeshlinePolicy::Normal::NONE);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->coord == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::XMIN, one is Normal::XMAX and one is not to mesh") {
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			auto state_c = c.get_current_state();
			auto state_d = d.get_current_state();
			state_a.to_mesh = true;
			state_b.to_mesh = true;
			state_c.to_mesh = false;
			state_d.to_mesh = true;
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			c.set_next_state(state_c);
			d.set_next_state(state_d);
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.get_current_state().line_policies[Y].size() == 0);
				REQUIRE(mpm.get_current_state().line_policies[X].size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0].get() == cce.get_current_state().solution);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins.size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins[0] == &cce);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().policy == MeshlinePolicy::Policy::HALFS);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().normal == MeshlinePolicy::Normal::NONE);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->coord == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::XMIN, one is Normal::XMAX but to reverse and one is not to mesh") {
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			auto state_c = c.get_current_state();
			auto state_d = d.get_current_state();
			state_a.to_mesh = true;
			state_b.to_mesh = true;
			state_c.to_mesh = false;
			state_d.to_mesh = true;
			state_d.to_reverse = true;
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			c.set_next_state(state_c);
			d.set_next_state(state_d);
			cce.auto_solve(mpm);
			THEN("Should add a thirds meshline policy in the meshline policy manager") {
				REQUIRE(mpm.get_current_state().line_policies[Y].size() == 0);
				REQUIRE(mpm.get_current_state().line_policies[X].size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0].get() == cce.get_current_state().solution);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins.size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins[0] == &cce);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().policy == MeshlinePolicy::Policy::THIRDS);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().normal == MeshlinePolicy::Normal::MIN);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->coord == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::XMIN, and two are not to mesh") {
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			auto state_c = c.get_current_state();
			auto state_d = d.get_current_state();
			state_a.to_mesh = true;
			state_b.to_mesh = true;
			state_c.to_mesh = false;
			state_d.to_mesh = false;
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			c.set_next_state(state_c);
			d.set_next_state(state_d);
			cce.auto_solve(mpm);
			THEN("Should add a thirds meshline policy in the meshline policy manager") {
				REQUIRE(mpm.get_current_state().line_policies[Y].size() == 0);
				REQUIRE(mpm.get_current_state().line_policies[X].size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0].get() == cce.get_current_state().solution);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins.size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins[0] == &cce);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().policy == MeshlinePolicy::Policy::THIRDS);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().normal == MeshlinePolicy::Normal::MIN);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->coord == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::XMAX, one is Normal::XMIN and one is not to mesh") {
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			auto state_c = c.get_current_state();
			auto state_d = d.get_current_state();
			state_a.to_mesh = false;
			state_b.to_mesh = true;
			state_c.to_mesh = true;
			state_d.to_mesh = true;
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			c.set_next_state(state_c);
			d.set_next_state(state_d);
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.get_current_state().line_policies[Y].size() == 0);
				REQUIRE(mpm.get_current_state().line_policies[X].size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0].get() == cce.get_current_state().solution);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins.size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins[0] == &cce);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().policy == MeshlinePolicy::Policy::HALFS);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().normal == MeshlinePolicy::Normal::NONE);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->coord == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::XMAX, and two are not to mesh") {
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			auto state_c = c.get_current_state();
			auto state_d = d.get_current_state();
			state_a.to_mesh = false;
			state_b.to_mesh = false;
			state_c.to_mesh = true;
			state_d.to_mesh = true;
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			c.set_next_state(state_c);
			d.set_next_state(state_d);
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.get_current_state().line_policies[Y].size() == 0);
				REQUIRE(mpm.get_current_state().line_policies[X].size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0].get() == cce.get_current_state().solution);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins.size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().origins[0] == &cce);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().policy == MeshlinePolicy::Policy::THIRDS);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().normal == MeshlinePolicy::Normal::MAX);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->coord == 1);
				REQUIRE(mpm.get_current_state().line_policies[X][0]->get_current_state().is_enabled);
			}
		}
	}

	GIVEN("A conflict about four colinear horizontal edges with mixed normals") {
		GlobalParams params(t);
		MeshlinePolicyManager mpm(&params, t);
		Point a0(1, 1), a1(2, 1);
		Point b0(3, 1), b1(4, 1);
		Point c0(5, 1), c1(6, 1);
		Point d0(7, 1), d1(8, 1);
		Edge a(XY, &a0, &a1, t), b(XY, &b0, &b1, t), c(XY, &c0, &c1, t), d(XY, &d0, &d1, t);
		a.normal = Normal::YMIN;
		b.normal = Normal::YMIN;
		c.normal = Normal::YMAX;
		d.normal = Normal::YMAX;
		ConflictColinearEdges cce(Y, &a, &b, t);
		cce.append(&c);
		cce.append(&d);
		WHEN("Two edges normals are Normal::YMIN and two are Normal::YMAX") {
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			auto state_c = c.get_current_state();
			auto state_d = d.get_current_state();
			state_a.to_mesh = true;
			state_b.to_mesh = true;
			state_c.to_mesh = true;
			state_d.to_mesh = true;
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			c.set_next_state(state_c);
			d.set_next_state(state_d);
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.get_current_state().line_policies[Y].size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X].size() == 0);
				REQUIRE(mpm.get_current_state().line_policies[Y][0].get() == cce.get_current_state().solution);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins.size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins[0] == &cce);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().policy == MeshlinePolicy::Policy::HALFS);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().normal == MeshlinePolicy::Normal::NONE);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->coord == 1);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::YMIN, one is Normal::YMAX and one is not to mesh") {
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			auto state_c = c.get_current_state();
			auto state_d = d.get_current_state();
			state_a.to_mesh = true;
			state_b.to_mesh = true;
			state_c.to_mesh = false;
			state_d.to_mesh = true;
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			c.set_next_state(state_c);
			d.set_next_state(state_d);
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.get_current_state().line_policies[Y].size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X].size() == 0);
				REQUIRE(mpm.get_current_state().line_policies[Y][0].get() == cce.get_current_state().solution);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins.size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins[0] == &cce);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().policy == MeshlinePolicy::Policy::HALFS);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().normal == MeshlinePolicy::Normal::NONE);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->coord == 1);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::YMIN, one is Normal::YMAX but to reverse and one is not to mesh") {
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			auto state_c = c.get_current_state();
			auto state_d = d.get_current_state();
			state_a.to_mesh = true;
			state_b.to_mesh = true;
			state_c.to_mesh = false;
			state_d.to_mesh = true;
			state_d.to_reverse = true;
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			c.set_next_state(state_c);
			d.set_next_state(state_d);
			cce.auto_solve(mpm);
			THEN("Should add a thirds meshline policy in the meshline policy manager") {
				REQUIRE(mpm.get_current_state().line_policies[Y].size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X].size() == 0);
				REQUIRE(mpm.get_current_state().line_policies[Y][0].get() == cce.get_current_state().solution);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins.size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins[0] == &cce);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().policy == MeshlinePolicy::Policy::THIRDS);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().normal == MeshlinePolicy::Normal::MIN);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->coord == 1);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::YMIN, and two are not to mesh") {
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			auto state_c = c.get_current_state();
			auto state_d = d.get_current_state();
			state_a.to_mesh = true;
			state_b.to_mesh = true;
			state_c.to_mesh = false;
			state_d.to_mesh = false;
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			c.set_next_state(state_c);
			d.set_next_state(state_d);
			cce.auto_solve(mpm);
			THEN("Should add a thirds meshline policy in the meshline policy manager") {
				REQUIRE(mpm.get_current_state().line_policies[Y].size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X].size() == 0);
				REQUIRE(mpm.get_current_state().line_policies[Y][0].get() == cce.get_current_state().solution);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins.size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins[0] == &cce);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().policy == MeshlinePolicy::Policy::THIRDS);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().normal == MeshlinePolicy::Normal::MIN);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->coord == 1);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::YMAX, one is Normal::YMIN and one is not to mesh") {
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			auto state_c = c.get_current_state();
			auto state_d = d.get_current_state();
			state_a.to_mesh = false;
			state_b.to_mesh = true;
			state_c.to_mesh = true;
			state_d.to_mesh = true;
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			c.set_next_state(state_c);
			d.set_next_state(state_d);
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.get_current_state().line_policies[Y].size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X].size() == 0);
				REQUIRE(mpm.get_current_state().line_policies[Y][0].get() == cce.get_current_state().solution);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins.size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins[0] == &cce);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().policy == MeshlinePolicy::Policy::HALFS);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().normal == MeshlinePolicy::Normal::NONE);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->coord == 1);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::YMAX, and two are not to mesh") {
			auto state_a = a.get_current_state();
			auto state_b = b.get_current_state();
			auto state_c = c.get_current_state();
			auto state_d = d.get_current_state();
			state_a.to_mesh = false;
			state_b.to_mesh = false;
			state_c.to_mesh = true;
			state_d.to_mesh = true;
			a.set_next_state(state_a);
			b.set_next_state(state_b);
			c.set_next_state(state_c);
			d.set_next_state(state_d);
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.get_current_state().line_policies[Y].size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[X].size() == 0);
				REQUIRE(mpm.get_current_state().line_policies[Y][0].get() == cce.get_current_state().solution);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins.size() == 1);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().origins[0] == &cce);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().policy == MeshlinePolicy::Policy::THIRDS);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().normal == MeshlinePolicy::Normal::MAX);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->coord == 1);
				REQUIRE(mpm.get_current_state().line_policies[Y][0]->get_current_state().is_enabled);
			}
		}
	}
}
