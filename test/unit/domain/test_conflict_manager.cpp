///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/conflicts/conflict_colinear_edges.hpp"
#include "domain/conflicts/conflict_edge_in_polygon.hpp"
#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/point.hpp"
#include "domain/geometrics/polygon.hpp"
#include "domain/geometrics/space.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "domain/global.hpp"
#include "utils/vector_utils.hpp"

#include "domain/conflict_manager.hpp"

/// @test void ConflictManager::add_colinear_edges(Edge* a, Edge* b)
/// @test void ConflictManager::add_edge_in_polygon(
///       	Edge* a,
///       	Polygon* polygon,
///       	Range const range,
///       	std::optional<Edge const*> b)
/// @test ConflictTooCloseMeshlinePolicies& ConflictManager::add_too_close_meshline_policies(
///       	MeshlinePolicy* a,
///       	MeshlinePolicy* b)
///*****************************************************************************

using namespace domain;

//******************************************************************************
SCENARIO("void ConflictManager::add_colinear_edges(Edge* a, Edge* b)", "[conflict_manager]") {
	GIVEN("A conflict manager and some edges") {
		ConflictManager cm(nullptr);
		WHEN("Two vertical edges that are colinear are reported as colinear") {
			Point a0(1, 1), a1(1, 2);
			Point b0(1, 3), b1(1, 4);
			Edge a(XY, &a0, &a1);
			Edge b(XY, &b0, &b1);
			cm.add_colinear_edges(&a, &b);
			THEN("A COLINEAR_EDGES conflict should be registered") {
				REQUIRE(cm.all_colinear_edges[X].size() == 1);
				REQUIRE(cm.all_colinear_edges[X][0]->kind == Conflict::Kind::COLINEAR_EDGES);
				ConflictColinearEdges* conflict = cm.all_colinear_edges[X][0].get();
				REQUIRE(conflict->edges.size() == 2);
				REQUIRE(conflict->edges[0] == &a);
				REQUIRE(conflict->edges[1] == &b);
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(b.conflicts.size() == 1);
				REQUIRE(b.conflicts[0] == conflict);
			}
			AND_WHEN("The same conflict is reported another time") {
				cm.add_colinear_edges(&a, &b);
				cm.add_colinear_edges(&b, &a);
				THEN("No conflict should be added nor modified") {
					REQUIRE(cm.all_colinear_edges[X].size() == 1);
					REQUIRE(cm.all_colinear_edges[X][0]->kind == Conflict::Kind::COLINEAR_EDGES);
					ConflictColinearEdges* conflict = cm.all_colinear_edges[X][0].get();
					REQUIRE(conflict->edges.size() == 2);
					REQUIRE(a.conflicts.size() == 1);
					REQUIRE(b.conflicts.size() == 1);
				}
			}
			AND_WHEN("A third vertical colinear edge is reported as colinear") {
				Point c0(1, 5), c1(1, 6);
				Edge c(XY, &c0, &c1);
				THEN("It should be added to the existing conflict") {
					cm.add_colinear_edges(&a, &c);
					REQUIRE(cm.all_colinear_edges[X].size() == 1);
					REQUIRE(cm.all_colinear_edges[X][0]->kind == Conflict::Kind::COLINEAR_EDGES);
					ConflictColinearEdges* conflict = cm.all_colinear_edges[X][0].get();
					REQUIRE(conflict->edges.size() == 3);
					REQUIRE(conflict->edges[2] == &c);
					REQUIRE(c.conflicts.size() == 1);
					REQUIRE(c.conflicts[0] == conflict);
				}
				THEN("It should be added to the existing conflict") {
					cm.add_colinear_edges(&c, &a);
					REQUIRE(cm.all_colinear_edges[X].size() == 1);
					REQUIRE(cm.all_colinear_edges[X][0]->kind == Conflict::Kind::COLINEAR_EDGES);
					ConflictColinearEdges* conflict = cm.all_colinear_edges[X][0].get();
					REQUIRE(conflict->edges.size() == 3);
					REQUIRE(conflict->edges[2] == &c);
					REQUIRE(c.conflicts.size() == 1);
					REQUIRE(c.conflicts[0] == conflict);
				}
			}
			AND_WHEN("Two other colinear edges elsewhere are reported as colinear") {
				Point d0(2, 1), d1(2, 2);
				Point e0(2, 3), e1(2, 4);
				Edge d(XY, &d0, &d1);
				Edge e(XY, &e0, &e1);
				cm.add_colinear_edges(&d, &e);
				THEN("A new COLINEAR_EDGES conflict should be registered") {
					REQUIRE(cm.all_colinear_edges[X].size() == 2);
					REQUIRE(cm.all_colinear_edges[X][1]->kind == Conflict::Kind::COLINEAR_EDGES);
					ConflictColinearEdges* conflict = cm.all_colinear_edges[X][1].get();
					REQUIRE(conflict->edges.size() == 2);
					REQUIRE(conflict->edges[0] == &d);
					REQUIRE(conflict->edges[1] == &e);
				}
			}
		}

		WHEN("Two horizontal edges that are colinear are reported as colinear") {
			Point a0(1, 1), a1(2, 1);
			Point b0(3, 1), b1(4, 1);
			Edge a(XY, &a0, &a1);
			Edge b(XY, &b0, &b1);
			cm.add_colinear_edges(&a, &b);
			THEN("A COLINEAR_EDGES conflict should be registered") {
				REQUIRE(cm.all_colinear_edges[Y].size() == 1);
				REQUIRE(cm.all_colinear_edges[Y][0]->kind == Conflict::Kind::COLINEAR_EDGES);
				ConflictColinearEdges* conflict = cm.all_colinear_edges[Y][0].get();
				REQUIRE(conflict->edges.size() == 2);
				REQUIRE(conflict->edges[0] == &a);
				REQUIRE(conflict->edges[1] == &b);
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(b.conflicts.size() == 1);
				REQUIRE(b.conflicts[0] == conflict);
			}
			AND_WHEN("The same conflict is reported another time") {
				cm.add_colinear_edges(&a, &b);
				cm.add_colinear_edges(&b, &a);
				THEN("No conflict should be added nor modified") {
					REQUIRE(cm.all_colinear_edges[Y].size() == 1);
					REQUIRE(cm.all_colinear_edges[Y][0]->kind == Conflict::Kind::COLINEAR_EDGES);
					ConflictColinearEdges* conflict = cm.all_colinear_edges[Y][0].get();
					REQUIRE(conflict->edges.size() == 2);
					REQUIRE(a.conflicts.size() == 1);
					REQUIRE(b.conflicts.size() == 1);
				}
			}
			AND_WHEN("A third horizontal colinear edge is reported as colinear") {
				Point c0(5, 1), c1(6, 1);
				Edge c(XY, &c0, &c1);
				THEN("It should be added to the existing conflict") {
					cm.add_colinear_edges(&a, &c);
					REQUIRE(cm.all_colinear_edges[Y].size() == 1);
					REQUIRE(cm.all_colinear_edges[Y][0]->kind == Conflict::Kind::COLINEAR_EDGES);
					ConflictColinearEdges* conflict = cm.all_colinear_edges[Y][0].get();
					REQUIRE(conflict->edges.size() == 3);
					REQUIRE(conflict->edges[2] == &c);
					REQUIRE(c.conflicts.size() == 1);
					REQUIRE(c.conflicts[0] == conflict);
				}
				THEN("It should be added to the existing conflict") {
					cm.add_colinear_edges(&c, &a);
					REQUIRE(cm.all_colinear_edges[Y].size() == 1);
					REQUIRE(cm.all_colinear_edges[Y][0]->kind == Conflict::Kind::COLINEAR_EDGES);
					ConflictColinearEdges* conflict = cm.all_colinear_edges[Y][0].get();
					REQUIRE(conflict->edges.size() == 3);
					REQUIRE(conflict->edges[2] == &c);
					REQUIRE(c.conflicts.size() == 1);
					REQUIRE(c.conflicts[0] == conflict);
				}
			}
			AND_WHEN("Two other colinear edges elsewhere are reported as colinear") {
				Point d0(2, 1), d1(2, 2);
				Point e0(2, 3), e1(2, 4);
				Edge d(XY, &d0, &d1);
				Edge e(XY, &e0, &e1);
				cm.add_colinear_edges(&d, &e);
				THEN("A new COLINEAR_EDGES conflict should be registered") {
					REQUIRE(cm.all_colinear_edges[Y].size() == 1);
					REQUIRE(cm.all_colinear_edges[X].size() == 1);
					REQUIRE(cm.all_colinear_edges[X][0]->kind == Conflict::Kind::COLINEAR_EDGES);
					ConflictColinearEdges* conflict = cm.all_colinear_edges[X][0].get();
					REQUIRE(conflict->edges.size() == 2);
					REQUIRE(conflict->edges[0] == &d);
					REQUIRE(conflict->edges[1] == &e);
				}
			}
		}

		// TODO Is this good?
		WHEN("Two vertical edges that are not colinear are reported as colinear") {
			Point a0(1, 1), a1(1, 2);
			Point b0(2, 3), b1(2, 4);
			Edge a(XY, &a0, &a1);
			Edge b(XY, &b0, &b1);
			cm.add_colinear_edges(&a, &b);
			THEN("A COLINEAR_EDGES conflict should be registered") {
				REQUIRE(cm.all_colinear_edges[X].size() == 1);
				REQUIRE(cm.all_colinear_edges[X][0]->kind == Conflict::Kind::COLINEAR_EDGES);
				ConflictColinearEdges* conflict = cm.all_colinear_edges[X][0].get();
				REQUIRE(conflict->edges.size() == 2);
				REQUIRE(conflict->edges[0] == &a);
				REQUIRE(conflict->edges[1] == &b);
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(b.conflicts.size() == 1);
				REQUIRE(b.conflicts[0] == conflict);
			}
		}

		// TODO Is this good?
		WHEN("Two horizontal edges that are not colinear are reported as colinear") {
			Point a0(1, 1), a1(2, 1);
			Point b0(3, 2), b1(4, 2);
			Edge a(XY, &a0, &a1);
			Edge b(XY, &b0, &b1);
			cm.add_colinear_edges(&a, &b);
			THEN("A COLINEAR_EDGES conflict should be registered") {
				REQUIRE(cm.all_colinear_edges[Y].size() == 1);
				REQUIRE(cm.all_colinear_edges[Y][0]->kind == Conflict::Kind::COLINEAR_EDGES);
				ConflictColinearEdges* conflict = cm.all_colinear_edges[Y][0].get();
				REQUIRE(conflict->edges.size() == 2);
				REQUIRE(conflict->edges[0] == &a);
				REQUIRE(conflict->edges[1] == &b);
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(b.conflicts.size() == 1);
				REQUIRE(b.conflicts[0] == conflict);
			}
		}

		WHEN("Two diagonal edges that are colinear are reported as colinear") {
			Point a0(1, 1), a1(2, 2);
			Point b0(3, 3), b1(4, 4);
			Edge a(XY, &a0, &a1);
			Edge b(XY, &b0, &b1);
			cm.add_colinear_edges(&a, &b);
			THEN("No conflict should be registered") {
				REQUIRE_FALSE(cm.all_colinear_edges[X].size());
				REQUIRE_FALSE(cm.all_colinear_edges[Y].size());
				REQUIRE_FALSE(cm.all_colinear_edges[Z].size());
			}
		}

		WHEN("Edges from different planes and/or axis are reported as colinear") {
			Point a0(1, 1), a1(1, 2);
			Point b0(3, 3), b1(4, 3);
			Edge a(XY, &a0, &a1);
			Edge b(XY, &b0, &b1);
			Edge c(YZ, &b0, &b1);
			cm.add_colinear_edges(&a, &b);
			cm.add_colinear_edges(&a, &c);
			cm.add_colinear_edges(&b, &c);
			THEN("No conflict should be registered") {
				REQUIRE_FALSE(cm.all_colinear_edges[X].size());
				REQUIRE_FALSE(cm.all_colinear_edges[Y].size());
				REQUIRE_FALSE(cm.all_colinear_edges[Z].size());
			}
		}
	}
}

//******************************************************************************
SCENARIO("void ConflictManager::add_edge_in_polygon(Edge* a, Polygon* polygon, Range const range, std::optional<Edge const*> b)", "[conflict_manager]") {
	GIVEN("A conflict manager, an edge and some polygons") {
		ConflictManager cm(nullptr);
		Polygon p(XY, "", from_init_list<Point>({
			{ 3, 1 }, { 6, 1 }, { 6, 6 }, { 1, 6 }, { 1, 3 },
			{ 2, 3 }, { 2, 5 }, { 5, 5 }, { 5, 2 }, { 3, 2 }}));
		Polygon q(XY, "", from_init_list<Point>({{ 0, 0 }, { 0, 8 }, { 8, 8 }, { 8, 0 }}));
		WHEN("A vertical edge that is in a polygon is reported as partially in this polygon") {
			Point a0(4, 1), a1(4, 7);
			Edge a(XY, &a0, &a1);
			cm.add_edge_in_polygon(&a, &p, Range({ 4, 1 }, { 4, 2 }));
			THEN("An EDGE_IN_POLYGON conflict should be registered") {
				REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
				REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
				ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
				REQUIRE(conflict->edge == &a);
				REQUIRE(conflict->overlaps.size() == 1);
				REQUIRE(std::get<POLYGON>(conflict->overlaps[0]) == &p);
				REQUIRE(std::get<RANGE>(conflict->overlaps[0]) == Range({ 4, 1 }, { 4, 2 }));
				REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[0]));
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(p.conflicts.size() == 1);
				REQUIRE(p.conflicts[0] == conflict);
			}
			AND_WHEN("The same conflict is reported another time") {
				cm.add_edge_in_polygon(&a, &p, Range({ 4, 1 }, { 4, 2 }));
				THEN("No conflict should be added nor modified") {
					REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
					REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
					ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
					REQUIRE(conflict->overlaps.size() == 1);
					REQUIRE(a.conflicts.size() == 1);
					REQUIRE(p.conflicts.size() == 1);
				}
			}
			AND_WHEN("The edge is reported as also partially in the same polygon but by another overlap range") {
				cm.add_edge_in_polygon(&a, &p, Range({ 4, 5 }, { 4, 6 }));
				THEN("It should be added to the existing conflict") {
					REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
					REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
					ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
					REQUIRE(conflict->overlaps.size() == 2);
					REQUIRE(std::get<POLYGON>(conflict->overlaps[1]) == &p);
					REQUIRE(std::get<RANGE>(conflict->overlaps[1]) == Range({ 4, 5 }, { 4, 6 }));
					REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[1]));
					REQUIRE(a.conflicts.size() == 1);
					REQUIRE(p.conflicts.size() == 1);
				}
			}
			AND_WHEN("An other edge is reported as totally in the polygon") {
				Point b0(5, 5), b1(5, 6);
				Edge b(XY, &b0, &b1);
				cm.add_edge_in_polygon(&b, &p);
				THEN("An other EDGE_IN_POLYGON conflict should be registered") {
					REQUIRE(cm.all_edge_in_polygons[XY].size() == 2);
					REQUIRE(cm.all_edge_in_polygons[XY][1]->kind == Conflict::Kind::EDGE_IN_POLYGON);
					ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][1].get();
					REQUIRE(conflict->edge == &b);
					REQUIRE(conflict->overlaps.size() == 1);
					REQUIRE(std::get<POLYGON>(conflict->overlaps[0]) == &p);
					REQUIRE(std::get<RANGE>(conflict->overlaps[0]) == Range({ 5, 5 }, { 5, 6 }));
					REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[0]));
					REQUIRE(b.conflicts.size() == 1);
					REQUIRE(b.conflicts[0] == conflict);
					REQUIRE(p.conflicts.size() == 2);
					REQUIRE(p.conflicts[1] == conflict);
				}
			}
			AND_WHEN("The edge is reported as also totally in a second polygon") {
				cm.add_edge_in_polygon(&a, &q);
				THEN("It should be added to the existing conflict") {
					REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
					REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
					ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
					REQUIRE(conflict->overlaps.size() == 2);
					REQUIRE(std::get<POLYGON>(conflict->overlaps[1]) == &q);
					REQUIRE(std::get<RANGE>(conflict->overlaps[1]) == Range({ 4, 1 }, { 4, 7 }));
					REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[1]));
					REQUIRE(a.conflicts.size() == 1);
					REQUIRE(p.conflicts.size() == 1);
				}
			}
		}

		WHEN("An horizontal edge that is in a polygon is reported as partially in this polygon") {
			Point a0(1, 4), a1(7, 4);
			Edge a(XY, &a0, &a1);
			cm.add_edge_in_polygon(&a, &p, Range({ 1, 4 }, { 2, 4 }));
			THEN("An EDGE_IN_POLYGON conflict should be registered") {
				REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
				REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
				ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
				REQUIRE(conflict->edge == &a);
				REQUIRE(conflict->overlaps.size() == 1);
				REQUIRE(std::get<POLYGON>(conflict->overlaps[0]) == &p);
				REQUIRE(std::get<RANGE>(conflict->overlaps[0]) == Range({ 1, 4 }, { 2, 4 }));
				REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[0]));
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(p.conflicts.size() == 1);
				REQUIRE(p.conflicts[0] == conflict);
			}
			AND_WHEN("The same conflict is reported another time") {
				cm.add_edge_in_polygon(&a, &p, Range({ 1, 4 }, { 2, 4 }));
				THEN("No conflict should be added nor modified") {
					REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
					REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
					ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
					REQUIRE(conflict->overlaps.size() == 1);
					REQUIRE(a.conflicts.size() == 1);
					REQUIRE(p.conflicts.size() == 1);
				}
			}
			AND_WHEN("The edge is reported as also partially in the same polygon but by another overlap range") {
				cm.add_edge_in_polygon(&a, &p, Range({ 5, 4 }, { 6, 4 }));
				THEN("It should be added to the existing conflict") {
					REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
					REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
					ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
					REQUIRE(conflict->overlaps.size() == 2);
					REQUIRE(std::get<POLYGON>(conflict->overlaps[1]) == &p);
					REQUIRE(std::get<RANGE>(conflict->overlaps[1]) == Range({ 5, 4 }, { 6, 4 }));
					REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[1]));
					REQUIRE(a.conflicts.size() == 1);
					REQUIRE(p.conflicts.size() == 1);
				}
			}
			AND_WHEN("An other edge is reported as totally in the polygon") {
				Point b0(5, 5), b1(6, 5);
				Edge b(XY, &b0, &b1);
				cm.add_edge_in_polygon(&b, &p);
				THEN("An other EDGE_IN_POLYGON conflict should be registered") {
					REQUIRE(cm.all_edge_in_polygons[XY].size() == 2);
					REQUIRE(cm.all_edge_in_polygons[XY][1]->kind == Conflict::Kind::EDGE_IN_POLYGON);
					ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][1].get();
					REQUIRE(conflict->edge == &b);
					REQUIRE(conflict->overlaps.size() == 1);
					REQUIRE(std::get<POLYGON>(conflict->overlaps[0]) == &p);
					REQUIRE(std::get<RANGE>(conflict->overlaps[0]) == Range({ 5, 5 }, { 6, 5 }));
					REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[0]));
					REQUIRE(b.conflicts.size() == 1);
					REQUIRE(b.conflicts[0] == conflict);
					REQUIRE(p.conflicts.size() == 2);
					REQUIRE(p.conflicts[1] == conflict);
				}
			}
			AND_WHEN("The edge is reported as also totally in a second polygon") {
				cm.add_edge_in_polygon(&a, &q);
				THEN("It should be added to the existing conflict") {
					REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
					REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
					ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
					REQUIRE(conflict->overlaps.size() == 2);
					REQUIRE(std::get<POLYGON>(conflict->overlaps[1]) == &q);
					REQUIRE(std::get<RANGE>(conflict->overlaps[1]) == Range({ 1, 4 }, { 7, 4 }));
					REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[1]));
					REQUIRE(a.conflicts.size() == 1);
					REQUIRE(p.conflicts.size() == 1);
				}
			}
		}

		WHEN("A diagonal edge that is in a polygon is reported as partially in this polygon") {
			Point a0(2, 7), a1(7, 2);
			Edge a(XY, &a0, &a1);
			cm.add_edge_in_polygon(&a, &p, Range({ 3, 6 }, { 4, 5 }));
			THEN("An EDGE_IN_POLYGON conflict should be registered") {
				REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
				REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
				ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
				REQUIRE(conflict->edge == &a);
				REQUIRE(conflict->overlaps.size() == 1);
				REQUIRE(std::get<POLYGON>(conflict->overlaps[0]) == &p);
				REQUIRE(std::get<RANGE>(conflict->overlaps[0]) == Range({ 3, 6 }, { 4, 5 }));
				REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[0]));
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(p.conflicts.size() == 1);
				REQUIRE(p.conflicts[0] == conflict);
			}
			AND_WHEN("The same conflict is reported another time") {
				cm.add_edge_in_polygon(&a, &p, Range({ 3, 6 }, { 4, 5 }));
				THEN("No conflict should be added nor modified") {
					REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
					REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
					ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
					REQUIRE(conflict->overlaps.size() == 1);
					REQUIRE(a.conflicts.size() == 1);
					REQUIRE(p.conflicts.size() == 1);
				}
			}
			AND_WHEN("The edge is reported as also partially in the same polygon but by another overlap range") {
				cm.add_edge_in_polygon(&a, &p, Range({ 5, 4 }, { 6, 3 }));
				THEN("It should be added to the existing conflict") {
					REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
					REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
					ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
					REQUIRE(conflict->overlaps.size() == 2);
					REQUIRE(std::get<POLYGON>(conflict->overlaps[1]) == &p);
					REQUIRE(std::get<RANGE>(conflict->overlaps[1]) == Range({ 5, 4 }, { 6, 3 }));
					REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[1]));
					REQUIRE(a.conflicts.size() == 1);
					REQUIRE(p.conflicts.size() == 1);
				}
			}
			AND_WHEN("An other edge is reported as totally in the polygon") {
				Point b0(5, 5), b1(6, 6);
				Edge b(XY, &b0, &b1);
				cm.add_edge_in_polygon(&b, &p);
				THEN("An other EDGE_IN_POLYGON conflict should be registered") {
					REQUIRE(cm.all_edge_in_polygons[XY].size() == 2);
					REQUIRE(cm.all_edge_in_polygons[XY][1]->kind == Conflict::Kind::EDGE_IN_POLYGON);
					ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][1].get();
					REQUIRE(conflict->edge == &b);
					REQUIRE(conflict->overlaps.size() == 1);
					REQUIRE(std::get<POLYGON>(conflict->overlaps[0]) == &p);
					REQUIRE(std::get<RANGE>(conflict->overlaps[0]) == Range({ 5, 5 }, { 6, 6 }));
					REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[0]));
					REQUIRE(b.conflicts.size() == 1);
					REQUIRE(b.conflicts[0] == conflict);
					REQUIRE(p.conflicts.size() == 2);
					REQUIRE(p.conflicts[1] == conflict);
				}
			}
			AND_WHEN("The edge is reported as also totally in a second polygon") {
				cm.add_edge_in_polygon(&a, &q);
				THEN("It should be added to the existing conflict") {
					REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
					REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
					ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
					REQUIRE(conflict->overlaps.size() == 2);
					REQUIRE(std::get<POLYGON>(conflict->overlaps[1]) == &q);
					REQUIRE(std::get<RANGE>(conflict->overlaps[1]) == Range({ 2, 7 }, { 7, 2 }));
					REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[1]));
					REQUIRE(a.conflicts.size() == 1);
					REQUIRE(p.conflicts.size() == 1);
				}
			}
		}

		WHEN("A vertical edge that is not in a polygon is reported as in this polygon") {
			Point a0(8, 8), a1(8, 9);
			Edge a(XY, &a0, &a1);
			cm.add_edge_in_polygon(&a, &p);
			THEN("An EDGE_IN_POLYGON conflict should be registered") {
				REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
				REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
				ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
				REQUIRE(conflict->edge == &a);
				REQUIRE(conflict->overlaps.size() == 1);
				REQUIRE(std::get<POLYGON>(conflict->overlaps[0]) == &p);
				REQUIRE(std::get<RANGE>(conflict->overlaps[0]) == Range({ 8, 8 }, { 8, 9 }));
				REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[0]));
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(p.conflicts.size() == 1);
				REQUIRE(p.conflicts[0] == conflict);
			}
		}

		WHEN("An horizontal edge that is not in a polygon is reported as in this polygon") {
			Point a0(8, 8), a1(9, 8);
			Edge a(XY, &a0, &a1);
			cm.add_edge_in_polygon(&a, &p);
			THEN("An EDGE_IN_POLYGON conflict should be registered") {
				REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
				REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
				ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
				REQUIRE(conflict->edge == &a);
				REQUIRE(conflict->overlaps.size() == 1);
				REQUIRE(std::get<POLYGON>(conflict->overlaps[0]) == &p);
				REQUIRE(std::get<RANGE>(conflict->overlaps[0]) == Range({ 8, 8 }, { 9, 8 }));
				REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[0]));
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(p.conflicts.size() == 1);
				REQUIRE(p.conflicts[0] == conflict);
			}
		}

		WHEN("A diagonal edge that is not in a polygon is reported as in this polygon") {
			Point a0(8, 8), a1(9, 9);
			Edge a(XY, &a0, &a1);
			cm.add_edge_in_polygon(&a, &p);
			THEN("An EDGE_IN_POLYGON conflict should be registered") {
				REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
				REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
				ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
				REQUIRE(conflict->edge == &a);
				REQUIRE(conflict->overlaps.size() == 1);
				REQUIRE(std::get<POLYGON>(conflict->overlaps[0]) == &p);
				REQUIRE(std::get<RANGE>(conflict->overlaps[0]) == Range({ 8, 8 }, { 9, 9 }));
				REQUIRE_FALSE(std::get<EDGE>(conflict->overlaps[0]));
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(p.conflicts.size() == 1);
				REQUIRE(p.conflicts[0] == conflict);
			}
		}

		WHEN("A vertical edge that is on a polygon is reported as in this polygon and sharing an edge with it") {
			Point a0(3, 1), a1(3, 3);
			Edge a(XY, &a0, &a1);
			cm.add_edge_in_polygon(&a, &p, Range({ 3, 1 }, { 3, 2 }), p.edges[0].get());
			THEN("An EDGE_IN_POLYGON conflict should be registered") {
				REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
				REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
				ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
				REQUIRE(conflict->edge == &a);
				REQUIRE(conflict->overlaps.size() == 1);
				REQUIRE(std::get<POLYGON>(conflict->overlaps[0]) == &p);
				REQUIRE(std::get<RANGE>(conflict->overlaps[0]) == Range({ 3, 1 }, { 3, 2 }));
				REQUIRE(std::get<EDGE>(conflict->overlaps[0]));
				REQUIRE(std::get<EDGE>(conflict->overlaps[0]).value() == p.edges[0].get());
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(p.conflicts.size() == 1);
				REQUIRE(p.conflicts[0] == conflict);
			}
		}

		WHEN("An horizontal edge that is on a polygon is reported as in this polygon and sharing an edge with it") {
			Point a0(1, 3), a1(3, 3);
			Edge a(XY, &a0, &a1);
			cm.add_edge_in_polygon(&a, &p, Range({ 1, 3 }, { 2, 3 }), p.edges[5].get());
			THEN("An EDGE_IN_POLYGON conflict should be registered") {
				REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
				REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
				ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
				REQUIRE(conflict->edge == &a);
				REQUIRE(conflict->overlaps.size() == 1);
				REQUIRE(std::get<POLYGON>(conflict->overlaps[0]) == &p);
				REQUIRE(std::get<RANGE>(conflict->overlaps[0]) == Range({ 1, 3 }, { 2, 3 }));
				REQUIRE(std::get<EDGE>(conflict->overlaps[0]));
				REQUIRE(std::get<EDGE>(conflict->overlaps[0]).value() == p.edges[5].get());
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(p.conflicts.size() == 1);
				REQUIRE(p.conflicts[0] == conflict);
			}
		}

		WHEN("A diagonal edge that is not on a polygon is reported as in this polygon and sharing an edge with it") {
			Point a0(1, 1), a1(3, 3);
			Edge a(XY, &a0, &a1);
			cm.add_edge_in_polygon(&a, &p, p.edges[0].get());
			THEN("An EDGE_IN_POLYGON conflict should be registered") {
				REQUIRE(cm.all_edge_in_polygons[XY].size() == 1);
				REQUIRE(cm.all_edge_in_polygons[XY][0]->kind == Conflict::Kind::EDGE_IN_POLYGON);
				ConflictEdgeInPolygon* conflict = cm.all_edge_in_polygons[XY][0].get();
				REQUIRE(conflict->edge == &a);
				REQUIRE(conflict->overlaps.size() == 1);
				REQUIRE(std::get<POLYGON>(conflict->overlaps[0]) == &p);
				REQUIRE(std::get<RANGE>(conflict->overlaps[0]) == Range({ 1, 1 }, { 3, 3 }));
				REQUIRE(std::get<EDGE>(conflict->overlaps[0]));
				REQUIRE(std::get<EDGE>(conflict->overlaps[0]).value() == p.edges[0].get());
				REQUIRE(a.conflicts.size() == 1);
				REQUIRE(a.conflicts[0] == conflict);
				REQUIRE(p.conflicts.size() == 1);
				REQUIRE(p.conflicts[0] == conflict);
			}
		}

		WHEN("An edge is reported as being in a polygon from a different plane") {
			Point a0(1, 1), a1(3, 3);
			Edge a(YZ, &a0, &a1);
			cm.add_edge_in_polygon(&a, &p, p.edges[0].get());
			cm.add_edge_in_polygon(&a, &p);
			THEN("No conflict should be added") {
				REQUIRE(cm.all_edge_in_polygons[YZ].size() == 0);
				REQUIRE(cm.all_edge_in_polygons[ZX].size() == 0);
				REQUIRE(cm.all_edge_in_polygons[XY].size() == 0);
			}
		}
	}
}

//******************************************************************************
SCENARIO("ConflictTooCloseMeshlinePolicies& ConflictManager::add_too_close_meshline_policies( \
MeshlinePolicy* a, \
MeshlinePolicy* b)", "[conflict_manager]") {
	GIVEN("A conflict manager and some meshline policies") {
		ConflictManager cm(nullptr);
		WHEN("Two meshline policies of different axis are reported") {
			Params p;
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				5);
			MeshlinePolicy b(
				X,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				5);
			THEN("No conflict should be registered") {
				auto* c = cm.add_too_close_meshline_policies(&a, &b);
				REQUIRE(c == nullptr);
			}
		}

		WHEN("Two meshline policies of same axis are reported") {
			Params p;
			MeshlinePolicy a(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				5);
			MeshlinePolicy b(
				Y,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				p,
				5);
			auto* c0 = cm.add_too_close_meshline_policies(&a, &b);
			THEN("A TOO_CLOSE_MESHLINE_POLICIES conflict should be registered") {
				REQUIRE_FALSE(c0 == nullptr);
				REQUIRE(c0->kind == Conflict::Kind::TOO_CLOSE_MESHLINE_POLICIES);
				REQUIRE(c0->meshline_policies[0] == &a);
				REQUIRE(c0->meshline_policies[1] == &b);
				REQUIRE_FALSE(a.is_enabled);
				REQUIRE_FALSE(b.is_enabled);
			}

			AND_WHEN("Two other meshline policies of same axis are reported") {
				MeshlinePolicy d(
					Y,
					MeshlinePolicy::Policy::HALFS,
					MeshlinePolicy::Normal::NONE,
					p,
					5);
				MeshlinePolicy e(
					Y,
					MeshlinePolicy::Policy::HALFS,
					MeshlinePolicy::Normal::NONE,
					p,
					5);
				auto* c1 = cm.add_too_close_meshline_policies(&d, &e);
				THEN("A TOO_CLOSE_MESHLINE_POLICIES conflict should be registered") {
					REQUIRE_FALSE(c1 == nullptr);
					REQUIRE_FALSE(c1 == c0);
					REQUIRE(c1->kind == Conflict::Kind::TOO_CLOSE_MESHLINE_POLICIES);
					REQUIRE(c1->meshline_policies[0] == &d);
					REQUIRE(c1->meshline_policies[1] == &e);
					REQUIRE_FALSE(d.is_enabled);
					REQUIRE_FALSE(e.is_enabled);
				}
			}

			AND_WHEN("A third meshline policy of same axis is reported to conflict with an already conflicting one") {
				MeshlinePolicy d(
					Y,
					MeshlinePolicy::Policy::HALFS,
					MeshlinePolicy::Normal::NONE,
					p,
					5);
				auto* c1 = cm.add_too_close_meshline_policies(&a, &d);
				auto* c2 = cm.add_too_close_meshline_policies(&d, &a);
				auto* c3 = cm.add_too_close_meshline_policies(&b, &d);
				auto* c4 = cm.add_too_close_meshline_policies(&d, &b);
				auto* c5 = cm.add_too_close_meshline_policies(&a, &b);
				THEN("No conflict should be registered") {
					REQUIRE(c1 == nullptr);
					REQUIRE(c2 == nullptr);
					REQUIRE(c3 == nullptr);
					REQUIRE(c4 == nullptr);
					REQUIRE(c5 == nullptr);
				}
			}
		}
	}
}
