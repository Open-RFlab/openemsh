///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/edge.hpp"
#include "domain/meshline_policy_manager.hpp"
#include "domain/point.hpp"

#include "domain/conflict_colinear_edges.hpp"

/// @test void ConflictColinearEdges::auto_solve(MeshlinePolicyManager& line_policy_manager)
///*****************************************************************************

//******************************************************************************
SCENARIO("void ConflictColinearEdges::auto_solve(MeshlinePolicyManager& line_policy_manager)", "[conflict_colinear_edges]") {
	GIVEN("A meshline policy manager and a conflict between 3 colinear vertical edges with the same normal vector") {
		Params params;
		MeshlinePolicyManager mpm(params);
		Point a0(1, 1), a1(1, 2);
		Point b0(1, 3), b1(1, 4);
		Point c0(1, 5), c1(1, 6);
		Edge a(&a0, &a1), b(&b0, &b1), c(&c0, &c1);
		a.normal = Normal::XMAX;
		b.normal = Normal::XMAX;
		c.normal = Normal::XMAX;
		ConflictColinearEdges cce(&a, &b);
		cce.append(&c);
		WHEN("Auto solving the conflict") {
			cce.auto_solve(mpm);
			THEN("Should add a thirds meshline policy in the meshline policy manager") {
				REQUIRE(mpm.line_policies[H].size() == 0);
				REQUIRE(mpm.line_policies[V].size() == 1);
				REQUIRE(mpm.line_policies[V][0]->origins.size() == 1);
				REQUIRE(mpm.line_policies[V][0]->origins[0] == &cce);
				REQUIRE(mpm.line_policies[V][0]->policy == MeshlinePolicy::Policy::THIRDS);
				REQUIRE(mpm.line_policies[V][0]->normal == Normal::XMAX);
				REQUIRE(mpm.line_policies[V][0]->coord == 1);
				REQUIRE(mpm.line_policies[V][0]->is_enabled);
				REQUIRE(mpm.line_policies[V][0]->res_factor == 1);
			}
		}
	}
}
