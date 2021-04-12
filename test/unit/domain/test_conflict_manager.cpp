///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/conflict_colinear_edges.hpp"
#include "domain/conflict_edge_in_polygon.hpp"
#include "domain/edge.hpp"
#include "domain/point.hpp"

#include "domain/conflict_manager.hpp"

/// @test void ConflictManager::add_colinear_edges(Edge* a, Edge* b)
/// @test void ConflictManager::add_edge_in_polygon(Edge* a, Polygon* polygon, Range const range, optional<Edge const*> b)
///*****************************************************************************

//******************************************************************************
SCENARIO("void ConflictManager::add_colinear_edges(Edge* a, Edge* b)", "[conflict_manager]") {
	GIVEN("A conflict vector, a conflict manager and some edges") {
		std::vector<std::unique_ptr<Conflict>> conflicts;
		ConflictManager cm(conflicts);
		WHEN("Two vertical edges are colinear") {
			Point a0(1, 1), a1(1, 2);
			Point b0(1, 3), b1(1, 4);
			Edge a(&a0, &a1);
			Edge b(&b0, &b1);
			cm.add_colinear_edges(&a, &b);
			THEN("A COLINEAR_EDGES conflict should be registered") {
				REQUIRE(conflicts.size() == 1);
				REQUIRE(conflicts[0]->kind == Conflict::Kind::COLINEAR_EDGES);
				auto conflict = dynamic_cast<ConflictColinearEdges*>(conflicts[0].get());
				REQUIRE(conflict->edges.size() == 2);
				REQUIRE(conflict->edges[0] == &a);
				REQUIRE(conflict->edges[1] == &b);
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(b.conflicts.size() == 1);
				REQUIRE(b.conflicts[0] == conflict);
			}
			AND_WHEN("We add the same conflict another time") {
				cm.add_colinear_edges(&a, &b);
				cm.add_colinear_edges(&b, &a);
				THEN("No conflict should be added nor modified") {
					REQUIRE(conflicts.size() == 1);
					REQUIRE(conflicts[0]->kind == Conflict::Kind::COLINEAR_EDGES);
					auto conflict = dynamic_cast<ConflictColinearEdges*>(conflicts[0].get());
					REQUIRE(conflict->edges.size() == 2);
					REQUIRE(a.conflicts.size() == 1);
					REQUIRE(b.conflicts.size() == 1);
				}
			}
			AND_WHEN("A third vertical colinear edge is added") {
				Point c0(1, 5), c1(1, 6);
				Edge c(&c0, &c1);
				THEN("It should be added to the existing conflict") {
					cm.add_colinear_edges(&a, &c);
					REQUIRE(conflicts.size() == 1);
					REQUIRE(conflicts[0]->kind == Conflict::Kind::COLINEAR_EDGES);
					auto conflict = dynamic_cast<ConflictColinearEdges*>(conflicts[0].get());
					REQUIRE(conflict->edges.size() == 3);
					REQUIRE(conflict->edges[2] == &c);
					REQUIRE(c.conflicts.size() == 1);
					REQUIRE(c.conflicts[0] == conflict);
				}
				THEN("It should be added to the existing conflict") {
					cm.add_colinear_edges(&c, &a);
					REQUIRE(conflicts.size() == 1);
					REQUIRE(conflicts[0]->kind == Conflict::Kind::COLINEAR_EDGES);
					auto conflict = dynamic_cast<ConflictColinearEdges*>(conflicts[0].get());
					REQUIRE(conflict->edges.size() == 3);
					REQUIRE(conflict->edges[2] == &c);
					REQUIRE(c.conflicts.size() == 1);
					REQUIRE(c.conflicts[0] == conflict);
				}
			}
			AND_WHEN("Two other edges are colinear elsewhere") {
				Point d0(2, 1), d1(2, 2);
				Point e0(2, 3), e1(2, 4);
				Edge d(&d0, &d1);
				Edge e(&e0, &e1);
				cm.add_colinear_edges(&d, &e);
				THEN("A new COLINEAR_EDGES conflict should be registered") {
					REQUIRE(conflicts.size() == 2);
					REQUIRE(conflicts[1]->kind == Conflict::Kind::COLINEAR_EDGES);
					auto conflict = dynamic_cast<ConflictColinearEdges*>(conflicts[1].get());
					REQUIRE(conflict->edges.size() == 2);
					REQUIRE(conflict->edges[0] == &d);
					REQUIRE(conflict->edges[1] == &e);
				}
			}
		}

		WHEN("Two horizontal edges are colinear") {
			Point a0(1, 1), a1(2, 1);
			Point b0(3, 1), b1(4, 1);
			Edge a(&a0, &a1);
			Edge b(&b0, &b1);
			cm.add_colinear_edges(&a, &b);
			THEN("A COLINEAR_EDGES conflict should be registered") {
				REQUIRE(conflicts.size() == 1);
				REQUIRE(conflicts[0]->kind == Conflict::Kind::COLINEAR_EDGES);
				auto conflict = dynamic_cast<ConflictColinearEdges*>(conflicts[0].get());
				REQUIRE(conflict->edges.size() == 2);
				REQUIRE(conflict->edges[0] == &a);
				REQUIRE(conflict->edges[1] == &b);
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(b.conflicts.size() == 1);
				REQUIRE(b.conflicts[0] == conflict);
			}
			AND_WHEN("We add the same conflict another time") {
				cm.add_colinear_edges(&a, &b);
				cm.add_colinear_edges(&b, &a);
				THEN("No conflict should be added nor modified") {
					REQUIRE(conflicts.size() == 1);
					REQUIRE(conflicts[0]->kind == Conflict::Kind::COLINEAR_EDGES);
					auto conflict = dynamic_cast<ConflictColinearEdges*>(conflicts[0].get());
					REQUIRE(conflict->edges.size() == 2);
					REQUIRE(a.conflicts.size() == 1);
					REQUIRE(b.conflicts.size() == 1);
				}
			}
			AND_WHEN("A third horizontal colinear edge is added") {
				Point c0(5, 1), c1(6, 1);
				Edge c(&c0, &c1);
				THEN("It should be added to the existing conflict") {
					cm.add_colinear_edges(&a, &c);
					REQUIRE(conflicts.size() == 1);
					REQUIRE(conflicts[0]->kind == Conflict::Kind::COLINEAR_EDGES);
					auto conflict = dynamic_cast<ConflictColinearEdges*>(conflicts[0].get());
					REQUIRE(conflict->edges.size() == 3);
					REQUIRE(conflict->edges[2] == &c);
					REQUIRE(c.conflicts.size() == 1);
					REQUIRE(c.conflicts[0] == conflict);
				}
				THEN("It should be added to the existing conflict") {
					cm.add_colinear_edges(&c, &a);
					REQUIRE(conflicts.size() == 1);
					REQUIRE(conflicts[0]->kind == Conflict::Kind::COLINEAR_EDGES);
					auto conflict = dynamic_cast<ConflictColinearEdges*>(conflicts[0].get());
					REQUIRE(conflict->edges.size() == 3);
					REQUIRE(conflict->edges[2] == &c);
					REQUIRE(c.conflicts.size() == 1);
					REQUIRE(c.conflicts[0] == conflict);
				}
			}
			AND_WHEN("Two other edges are colinear elsewhere") {
				Point d0(2, 1), d1(2, 2);
				Point e0(2, 3), e1(2, 4);
				Edge d(&d0, &d1);
				Edge e(&e0, &e1);
				cm.add_colinear_edges(&d, &e);
				THEN("A new COLINEAR_EDGES conflict should be registered") {
					REQUIRE(conflicts.size() == 2);
					REQUIRE(conflicts[1]->kind == Conflict::Kind::COLINEAR_EDGES);
					auto conflict = dynamic_cast<ConflictColinearEdges*>(conflicts[1].get());
					REQUIRE(conflict->edges.size() == 2);
					REQUIRE(conflict->edges[0] == &d);
					REQUIRE(conflict->edges[1] == &e);
				}
			}
		}

		WHEN("Two diagonal edges are colinear") {
			Point a0(1, 1), a1(2, 2);
			Point b0(3, 3), b1(4, 4);
			Edge a(&a0, &a1);
			Edge b(&b0, &b1);
			cm.add_colinear_edges(&a, &b);
			THEN("No conflict should be registered") {
				REQUIRE_FALSE(conflicts.size());
			}
		}
	}
}

//******************************************************************************
SCENARIO("void ConflictManager::add_edge_in_polygon(Edge* a, Polygon* polygon, Range const range, optional<Edge const*> b)", "[conflict_manager]") {
}
