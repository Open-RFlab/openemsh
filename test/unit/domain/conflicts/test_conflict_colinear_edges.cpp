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

//******************************************************************************
SCENARIO("void ConflictColinearEdges::auto_solve(MeshlinePolicyManager& line_policy_manager)", "[conflict_colinear_edges]") {
	GIVEN("A conflict about four colinear vertical edges with mixed normals") {
		Params params;
		MeshlinePolicyManager mpm(params, nullptr);
		Point a0(1, 1), a1(1, 2);
		Point b0(1, 3), b1(1, 4);
		Point c0(1, 5), c1(1, 6);
		Point d0(1, 7), d1(1, 8);
		Edge a(&a0, &a1), b(&b0, &b1), c(&c0, &c1), d(&d0, &d1);
		a.normal = Normal::XMIN;
		b.normal = Normal::XMIN;
		c.normal = Normal::XMAX;
		d.normal = Normal::XMAX;
		ConflictColinearEdges cce(&a, &b);
		cce.append(&c);
		cce.append(&d);
		WHEN("Two edges normals are Normal::XMIN and two are Normal::XMAX") {
			a.to_mesh = true;
			b.to_mesh = true;
			c.to_mesh = true;
			d.to_mesh = true;
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.line_policies[H].size() == 0);
				REQUIRE(mpm.line_policies[V].size() == 1);
				REQUIRE(mpm.line_policies[V][0].get() == cce.solution);
				REQUIRE(mpm.line_policies[V][0]->origins.size() == 1);
				REQUIRE(mpm.line_policies[V][0]->origins[0] == &cce);
				REQUIRE(mpm.line_policies[V][0]->policy == MeshlinePolicy::Policy::HALFS);
				REQUIRE(mpm.line_policies[V][0]->normal == Normal::NONE);
				REQUIRE(mpm.line_policies[V][0]->coord == 1);
				REQUIRE(mpm.line_policies[V][0]->is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::XMIN, one is Normal::XMAX and one is not to mesh") {
			a.to_mesh = true;
			b.to_mesh = true;
			c.to_mesh = false;
			d.to_mesh = true;
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.line_policies[H].size() == 0);
				REQUIRE(mpm.line_policies[V].size() == 1);
				REQUIRE(mpm.line_policies[V][0].get() == cce.solution);
				REQUIRE(mpm.line_policies[V][0]->origins.size() == 1);
				REQUIRE(mpm.line_policies[V][0]->origins[0] == &cce);
				REQUIRE(mpm.line_policies[V][0]->policy == MeshlinePolicy::Policy::HALFS);
				REQUIRE(mpm.line_policies[V][0]->normal == Normal::NONE);
				REQUIRE(mpm.line_policies[V][0]->coord == 1);
				REQUIRE(mpm.line_policies[V][0]->is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::XMIN, and two are not to mesh") {
			a.to_mesh = true;
			b.to_mesh = true;
			c.to_mesh = false;
			d.to_mesh = false;
			cce.auto_solve(mpm);
			THEN("Should add a thirds meshline policy in the meshline policy manager") {
				REQUIRE(mpm.line_policies[H].size() == 0);
				REQUIRE(mpm.line_policies[V].size() == 1);
				REQUIRE(mpm.line_policies[V][0].get() == cce.solution);
				REQUIRE(mpm.line_policies[V][0]->origins.size() == 1);
				REQUIRE(mpm.line_policies[V][0]->origins[0] == &cce);
				REQUIRE(mpm.line_policies[V][0]->policy == MeshlinePolicy::Policy::THIRDS);
				REQUIRE(mpm.line_policies[V][0]->normal == Normal::XMIN);
				REQUIRE(mpm.line_policies[V][0]->coord == 1);
				REQUIRE(mpm.line_policies[V][0]->is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::XMAX, one is Normal::XMIN and one is not to mesh") {
			a.to_mesh = false;
			b.to_mesh = true;
			c.to_mesh = true;
			d.to_mesh = true;
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.line_policies[H].size() == 0);
				REQUIRE(mpm.line_policies[V].size() == 1);
				REQUIRE(mpm.line_policies[V][0].get() == cce.solution);
				REQUIRE(mpm.line_policies[V][0]->origins.size() == 1);
				REQUIRE(mpm.line_policies[V][0]->origins[0] == &cce);
				REQUIRE(mpm.line_policies[V][0]->policy == MeshlinePolicy::Policy::HALFS);
				REQUIRE(mpm.line_policies[V][0]->normal == Normal::NONE);
				REQUIRE(mpm.line_policies[V][0]->coord == 1);
				REQUIRE(mpm.line_policies[V][0]->is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::XMAX, and two are not to mesh") {
			a.to_mesh = false;
			b.to_mesh = false;
			c.to_mesh = true;
			d.to_mesh = true;
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.line_policies[H].size() == 0);
				REQUIRE(mpm.line_policies[V].size() == 1);
				REQUIRE(mpm.line_policies[V][0].get() == cce.solution);
				REQUIRE(mpm.line_policies[V][0]->origins.size() == 1);
				REQUIRE(mpm.line_policies[V][0]->origins[0] == &cce);
				REQUIRE(mpm.line_policies[V][0]->policy == MeshlinePolicy::Policy::THIRDS);
				REQUIRE(mpm.line_policies[V][0]->normal == Normal::XMAX);
				REQUIRE(mpm.line_policies[V][0]->coord == 1);
				REQUIRE(mpm.line_policies[V][0]->is_enabled);
			}
		}
	}

	GIVEN("A conflict about four colinear horizontal edges with mixed normals") {
		Params params;
		MeshlinePolicyManager mpm(params, nullptr);
		Point a0(1, 1), a1(2, 1);
		Point b0(3, 1), b1(4, 1);
		Point c0(5, 1), c1(6, 1);
		Point d0(7, 1), d1(8, 1);
		Edge a(&a0, &a1), b(&b0, &b1), c(&c0, &c1), d(&d0, &d1);
		a.normal = Normal::YMIN;
		b.normal = Normal::YMIN;
		c.normal = Normal::YMAX;
		d.normal = Normal::YMAX;
		ConflictColinearEdges cce(&a, &b);
		cce.append(&c);
		cce.append(&d);
		WHEN("Two edges normals are Normal::YMIN and two are Normal::YMAX") {
			a.to_mesh = true;
			b.to_mesh = true;
			c.to_mesh = true;
			d.to_mesh = true;
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.line_policies[H].size() == 1);
				REQUIRE(mpm.line_policies[V].size() == 0);
				REQUIRE(mpm.line_policies[H][0].get() == cce.solution);
				REQUIRE(mpm.line_policies[H][0]->origins.size() == 1);
				REQUIRE(mpm.line_policies[H][0]->origins[0] == &cce);
				REQUIRE(mpm.line_policies[H][0]->policy == MeshlinePolicy::Policy::HALFS);
				REQUIRE(mpm.line_policies[H][0]->normal == Normal::NONE);
				REQUIRE(mpm.line_policies[H][0]->coord == 1);
				REQUIRE(mpm.line_policies[H][0]->is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::YMIN, one is Normal::YMAX and one is not to mesh") {
			a.to_mesh = true;
			b.to_mesh = true;
			c.to_mesh = false;
			d.to_mesh = true;
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.line_policies[H].size() == 1);
				REQUIRE(mpm.line_policies[V].size() == 0);
				REQUIRE(mpm.line_policies[H][0].get() == cce.solution);
				REQUIRE(mpm.line_policies[H][0]->origins.size() == 1);
				REQUIRE(mpm.line_policies[H][0]->origins[0] == &cce);
				REQUIRE(mpm.line_policies[H][0]->policy == MeshlinePolicy::Policy::HALFS);
				REQUIRE(mpm.line_policies[H][0]->normal == Normal::NONE);
				REQUIRE(mpm.line_policies[H][0]->coord == 1);
				REQUIRE(mpm.line_policies[H][0]->is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::YMIN, and two are not to mesh") {
			a.to_mesh = true;
			b.to_mesh = true;
			c.to_mesh = false;
			d.to_mesh = false;
			cce.auto_solve(mpm);
			THEN("Should add a thirds meshline policy in the meshline policy manager") {
				REQUIRE(mpm.line_policies[H].size() == 1);
				REQUIRE(mpm.line_policies[V].size() == 0);
				REQUIRE(mpm.line_policies[H][0].get() == cce.solution);
				REQUIRE(mpm.line_policies[H][0]->origins.size() == 1);
				REQUIRE(mpm.line_policies[H][0]->origins[0] == &cce);
				REQUIRE(mpm.line_policies[H][0]->policy == MeshlinePolicy::Policy::THIRDS);
				REQUIRE(mpm.line_policies[H][0]->normal == Normal::YMIN);
				REQUIRE(mpm.line_policies[H][0]->coord == 1);
				REQUIRE(mpm.line_policies[H][0]->is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::YMAX, one is Normal::YMIN and one is not to mesh") {
			a.to_mesh = false;
			b.to_mesh = true;
			c.to_mesh = true;
			d.to_mesh = true;
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.line_policies[H].size() == 1);
				REQUIRE(mpm.line_policies[V].size() == 0);
				REQUIRE(mpm.line_policies[H][0].get() == cce.solution);
				REQUIRE(mpm.line_policies[H][0]->origins.size() == 1);
				REQUIRE(mpm.line_policies[H][0]->origins[0] == &cce);
				REQUIRE(mpm.line_policies[H][0]->policy == MeshlinePolicy::Policy::HALFS);
				REQUIRE(mpm.line_policies[H][0]->normal == Normal::NONE);
				REQUIRE(mpm.line_policies[H][0]->coord == 1);
				REQUIRE(mpm.line_policies[H][0]->is_enabled);
			}
		}

		WHEN("Two edges normals are Normal::YMAX, and two are not to mesh") {
			a.to_mesh = false;
			b.to_mesh = false;
			c.to_mesh = true;
			d.to_mesh = true;
			cce.auto_solve(mpm);
			THEN("Should add a halfs meshline policy in the meshline policy manager") {
				REQUIRE(mpm.line_policies[H].size() == 1);
				REQUIRE(mpm.line_policies[V].size() == 0);
				REQUIRE(mpm.line_policies[H][0].get() == cce.solution);
				REQUIRE(mpm.line_policies[H][0]->origins.size() == 1);
				REQUIRE(mpm.line_policies[H][0]->origins[0] == &cce);
				REQUIRE(mpm.line_policies[H][0]->policy == MeshlinePolicy::Policy::THIRDS);
				REQUIRE(mpm.line_policies[H][0]->normal == Normal::YMAX);
				REQUIRE(mpm.line_policies[H][0]->coord == 1);
				REQUIRE(mpm.line_policies[H][0]->is_enabled);
			}
		}
	}
}
