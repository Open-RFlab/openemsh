///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include <array>

#include "domain/conflict_colinear_edges.hpp"
#include "domain/conflict_edge_in_polygon.hpp"

#include "domain/board.hpp"

/// @test void detect_edges_in_polygons()
/// @test void detect_colinear_edges()
///*****************************************************************************

//******************************************************************************
SCENARIO("void detect_edges_in_polygons()", "[board]") {
	GIVEN("A board holding two simple polygons (orthogonal squares)") {
		WHEN("A polygon is totally inside the other") {
			std::unique_ptr<Board> b;
			{
				std::vector<std::unique_ptr<Polygon>> tmp;
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 1, 1 }, { 4, 1 }, { 4, 4 }, { 1, 4 }})));
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 2, 2 }, { 2, 3 }, { 3, 3 }, { 3, 2 }})));
				b = std::make_unique<Board>(tmp);
			}
			b->detect_edges_in_polygons();
			THEN("4 EDGE_IN_POLYGON conflicts should be registered") {
				REQUIRE(b->conflicts.size() == 4);
				for(std::unique_ptr<Conflict>& conflict : b->conflicts)
					REQUIRE(conflict->kind == Conflict::Kind::EDGE_IN_POLYGON);
				AND_THEN("The registered conflicts should be between the right edges and polygons") {
					bool is_first_registered = false;
					bool is_second_registered = false;
					bool is_third_registered = false;
					bool is_fourth_registered = false;
					for(std::unique_ptr<Conflict>& conflict : b->conflicts) {
						auto c = dynamic_cast<ConflictEdgeInPolygon*>(conflict.get());
						if(c->edge == b->polygons[1]->edges[0].get()) {
							is_first_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 3, 2 }, { 2, 2 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[1].get()) {
							is_second_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 2, 2 }, { 2, 3 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[2].get()) {
							is_third_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 2, 3 }, { 3, 3 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[3].get()) {
							is_fourth_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 3, 3 }, { 3, 2 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						}
					}
					REQUIRE(is_first_registered);
					REQUIRE(is_second_registered);
					REQUIRE(is_third_registered);
					REQUIRE(is_fourth_registered);
				}
				AND_THEN("The edges and polygons should register the conflicts") {
					REQUIRE(b->polygons[1]->edges[0]->conflicts.size() == 1);
					REQUIRE(b->polygons[1]->edges[1]->conflicts.size() == 1);
					REQUIRE(b->polygons[1]->edges[2]->conflicts.size() == 1);
					REQUIRE(b->polygons[1]->edges[3]->conflicts.size() == 1);
					REQUIRE(b->polygons[0]->conflicts.size() == 4);

					auto p0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->conflicts[0]);
					auto p0c1 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->conflicts[1]);
					auto p0c2 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->conflicts[2]);
					auto p0c3 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->conflicts[3]);
					auto p1e0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->edges[0]->conflicts[0]);
					auto p1e1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->edges[1]->conflicts[0]);
					auto p1e2c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->edges[2]->conflicts[0]);
					auto p1e3c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->edges[3]->conflicts[0]);
					REQUIRE(std::get<0>(p0c0->overlaps[0]) == b->polygons[0].get());
					REQUIRE(std::get<0>(p0c1->overlaps[0]) == b->polygons[0].get());
					REQUIRE(std::get<0>(p0c2->overlaps[0]) == b->polygons[0].get());
					REQUIRE(std::get<0>(p0c3->overlaps[0]) == b->polygons[0].get());
					REQUIRE(p1e0c0->edge == b->polygons[1]->edges[0].get());
					REQUIRE(p1e1c0->edge == b->polygons[1]->edges[1].get());
					REQUIRE(p1e2c0->edge == b->polygons[1]->edges[2].get());
					REQUIRE(p1e3c0->edge == b->polygons[1]->edges[3].get());
				}
			}
		}

		WHEN("Polygons overlap by a corner") {
			std::unique_ptr<Board> b;
			{
				std::vector<std::unique_ptr<Polygon>> tmp;
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 1, 2 }, { 3, 2 }, { 3, 4 }, { 1, 4 }})));
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 2, 1 }, { 4, 1 }, { 4, 3 }, { 2, 3 }})));
				b = std::make_unique<Board>(tmp);
			}
			b->detect_edges_in_polygons();
			THEN("4 EDGE_IN_POLYGON conflicts should be registered") {
				REQUIRE(b->conflicts.size() == 4);
				for(std::unique_ptr<Conflict>& conflict : b->conflicts)
					REQUIRE(conflict->kind == Conflict::Kind::EDGE_IN_POLYGON);
				AND_THEN("The registered conflicts should be between the right edges and polygons") {
					bool is_first_registered = false;
					bool is_second_registered = false;
					bool is_third_registered = false;
					bool is_fourth_registered = false;
					for(std::unique_ptr<Conflict>& conflict : b->conflicts) {
						auto c = dynamic_cast<ConflictEdgeInPolygon*>(conflict.get());
						if(c->edge == b->polygons[0]->edges[1].get()) {
							is_first_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[1].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 2, 2 }, { 3, 2 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[0]->edges[2].get()) {
							is_second_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[1].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 3, 2 }, { 3, 3 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[0].get()) {
							is_third_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 2, 3 }, { 2, 2 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[3].get()) {
							is_fourth_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 3, 3 }, { 2, 3 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						}
					}
					REQUIRE(is_first_registered);
					REQUIRE(is_second_registered);
					REQUIRE(is_third_registered);
					REQUIRE(is_fourth_registered);
				}
				AND_THEN("The edges and polygons should register the conflicts") {
					REQUIRE(b->polygons[0]->edges[1]->conflicts.size() == 1);
					REQUIRE(b->polygons[0]->edges[2]->conflicts.size() == 1);
					REQUIRE(b->polygons[1]->edges[0]->conflicts.size() == 1);
					REQUIRE(b->polygons[1]->edges[3]->conflicts.size() == 1);
					REQUIRE(b->polygons[0]->conflicts.size() == 2);
					REQUIRE(b->polygons[1]->conflicts.size() == 2);

					auto p0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->conflicts[0]);
					auto p0c1 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->conflicts[1]);
					auto p1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->conflicts[0]);
					auto p1c1 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->conflicts[1]);
					auto p0e1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->edges[1]->conflicts[0]);
					auto p0e2c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->edges[2]->conflicts[0]);
					auto p1e0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->edges[0]->conflicts[0]);
					auto p1e3c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->edges[3]->conflicts[0]);
					REQUIRE(std::get<0>(p0c0->overlaps[0]) == b->polygons[0].get());
					REQUIRE(std::get<0>(p0c1->overlaps[0]) == b->polygons[0].get());
					REQUIRE(std::get<0>(p1c0->overlaps[0]) == b->polygons[1].get());
					REQUIRE(std::get<0>(p1c1->overlaps[0]) == b->polygons[1].get());
					REQUIRE(p0e1c0->edge == b->polygons[0]->edges[1].get());
					REQUIRE(p0e2c0->edge == b->polygons[0]->edges[2].get());
					REQUIRE(p1e0c0->edge == b->polygons[1]->edges[0].get());
					REQUIRE(p1e3c0->edge == b->polygons[1]->edges[3].get());
				}
			}
		}

		WHEN("One half of a polygon is totally inside the other polygon") {
			std::unique_ptr<Board> b;
			{
				std::vector<std::unique_ptr<Polygon>> tmp;
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 1, 2 }, { 4, 2 }, { 4, 4 }, { 1, 4 }})));
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 2, 1 }, { 3, 1 }, { 3, 3 }, { 2, 3 }})));
				b = std::make_unique<Board>(tmp);
			}
			b->detect_edges_in_polygons();
			THEN("4 EDGE_IN_POLYGON conflicts should be registered") {
				REQUIRE(b->conflicts.size() == 4);
				for(std::unique_ptr<Conflict>& conflict : b->conflicts)
					REQUIRE(conflict->kind == Conflict::Kind::EDGE_IN_POLYGON);
				AND_THEN("The registered conflicts should be between the right edges and polygons") {
					bool is_first_registered = false;
					bool is_second_registered = false;
					bool is_third_registered = false;
					bool is_fourth_registered = false;
					for(std::unique_ptr<Conflict>& conflict : b->conflicts) {
						auto c = dynamic_cast<ConflictEdgeInPolygon*>(conflict.get());
						if(c->edge == b->polygons[0]->edges[1].get()) {
							is_first_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[1].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 2, 2 }, { 3, 2 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[0].get()) {
							is_second_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 2, 3 }, { 2, 2 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[2].get()) {
							is_third_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 3, 2 }, { 3, 3 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[3].get()) {
							is_fourth_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 3, 3 }, { 2, 3 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						}
					}
					REQUIRE(is_first_registered);
					REQUIRE(is_second_registered);
					REQUIRE(is_third_registered);
					REQUIRE(is_fourth_registered);
				}
				AND_THEN("The edges and polygons should register the conflicts") {
					REQUIRE(b->polygons[0]->edges[1]->conflicts.size() == 1);
					REQUIRE(b->polygons[1]->edges[0]->conflicts.size() == 1);
					REQUIRE(b->polygons[1]->edges[2]->conflicts.size() == 1);
					REQUIRE(b->polygons[1]->edges[3]->conflicts.size() == 1);
					REQUIRE(b->polygons[0]->conflicts.size() == 3);
					REQUIRE(b->polygons[1]->conflicts.size() == 1);

					auto p0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->conflicts[0]);
					auto p0c1 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->conflicts[1]);
					auto p0c2 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->conflicts[2]);
					auto p1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->conflicts[0]);
					auto p0e1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->edges[1]->conflicts[0]);
					auto p1e0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->edges[0]->conflicts[0]);
					auto p1e2c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->edges[2]->conflicts[0]);
					auto p1e3c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->edges[3]->conflicts[0]);
					REQUIRE(std::get<0>(p0c0->overlaps[0]) == b->polygons[0].get());
					REQUIRE(std::get<0>(p0c1->overlaps[0]) == b->polygons[0].get());
					REQUIRE(std::get<0>(p0c2->overlaps[0]) == b->polygons[0].get());
					REQUIRE(std::get<0>(p1c0->overlaps[0]) == b->polygons[1].get());
					REQUIRE(p0e1c0->edge == b->polygons[0]->edges[1].get());
					REQUIRE(p1e0c0->edge == b->polygons[1]->edges[0].get());
					REQUIRE(p1e2c0->edge == b->polygons[1]->edges[2].get());
					REQUIRE(p1e3c0->edge == b->polygons[1]->edges[3].get());
				}
			}
		}

		WHEN("One half of a diagonal polygon is totally inside the other polygon") {
			std::unique_ptr<Board> b;
			{
				std::vector<std::unique_ptr<Polygon>> tmp;
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 1, 1 }, { 4, 1 }, { 4, 4 }, { 1, 4 }})));
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 2, 3 }, { 3, 2 }, { 5, 4 }, { 4, 5 }})));
				b = std::make_unique<Board>(tmp);
			}
			b->detect_edges_in_polygons();
			THEN("5 EDGE_IN_POLYGON conflicts should be registered") {
				REQUIRE(b->conflicts.size() == 5);
				for(std::unique_ptr<Conflict>& conflict : b->conflicts)
					REQUIRE(conflict->kind == Conflict::Kind::EDGE_IN_POLYGON);
				AND_THEN("The registered conflicts should be between the right edges and polygons") {
					bool is_first_registered = false;
					bool is_second_registered = false;
					bool is_third_registered = false;
					bool is_fourth_registered = false;
					bool is_fifth_registered = false;
					for(std::unique_ptr<Conflict>& conflict : b->conflicts) {
						auto c = dynamic_cast<ConflictEdgeInPolygon*>(conflict.get());
						if(c->edge == b->polygons[0]->edges[2].get()) {
							is_first_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[1].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 4, 3 }, { 4, 4 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[0]->edges[3].get()) {
							is_second_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[1].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 4, 4 }, { 3, 4 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[0].get()) {
							is_third_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 3, 4 }, { 2, 3 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[1].get()) {
							is_fourth_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 2, 3 }, { 3, 2 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[2].get()) {
							is_fifth_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 3, 2 }, { 4, 3 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						}
					}
					REQUIRE(is_first_registered);
					REQUIRE(is_second_registered);
					REQUIRE(is_third_registered);
					REQUIRE(is_fourth_registered);
					REQUIRE(is_fifth_registered);
				}
				AND_THEN("The edges and polygons should register the conflicts") {
					REQUIRE(b->polygons[0]->edges[2]->conflicts.size() == 1);
					REQUIRE(b->polygons[0]->edges[3]->conflicts.size() == 1);
					REQUIRE(b->polygons[1]->edges[0]->conflicts.size() == 1);
					REQUIRE(b->polygons[1]->edges[1]->conflicts.size() == 1);
					REQUIRE(b->polygons[1]->edges[2]->conflicts.size() == 1);
					REQUIRE(b->polygons[0]->conflicts.size() == 3);
					REQUIRE(b->polygons[1]->conflicts.size() == 2);

					auto p0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->conflicts[0]);
					auto p0c1 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->conflicts[1]);
					auto p0c2 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->conflicts[2]);
					auto p1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->conflicts[0]);
					auto p1c1 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->conflicts[1]);
					auto p0e2c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->edges[2]->conflicts[0]);
					auto p0e3c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->edges[3]->conflicts[0]);
					auto p1e0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->edges[0]->conflicts[0]);
					auto p1e1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->edges[1]->conflicts[0]);
					auto p1e2c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->edges[2]->conflicts[0]);
					REQUIRE(std::get<0>(p0c0->overlaps[0]) == b->polygons[0].get());
					REQUIRE(std::get<0>(p0c1->overlaps[0]) == b->polygons[0].get());
					REQUIRE(std::get<0>(p0c2->overlaps[0]) == b->polygons[0].get());
					REQUIRE(std::get<0>(p1c0->overlaps[0]) == b->polygons[1].get());
					REQUIRE(std::get<0>(p1c1->overlaps[0]) == b->polygons[1].get());
					REQUIRE(p0e2c0->edge == b->polygons[0]->edges[2].get());
					REQUIRE(p0e3c0->edge == b->polygons[0]->edges[3].get());
					REQUIRE(p1e0c0->edge == b->polygons[1]->edges[0].get());
					REQUIRE(p1e1c0->edge == b->polygons[1]->edges[1].get());
					REQUIRE(p1e2c0->edge == b->polygons[1]->edges[2].get());
				}
			}
		}

		WHEN("Polygons touch each other by an edge") {
			std::unique_ptr<Board> b;
			{
				std::vector<std::unique_ptr<Polygon>> tmp;
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 1, 2 }, { 4, 2 }, { 4, 4 }, { 1, 4 }})));
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 2, 1 }, { 3, 1 }, { 3, 2 }, { 2, 2 }})));
				b = std::make_unique<Board>(tmp);
			}
			b->detect_edges_in_polygons();
			THEN("2 EDGE_IN_POLYGON conflicts should be registered") {
				REQUIRE(b->conflicts.size() == 2);
				for(std::unique_ptr<Conflict>& conflict : b->conflicts)
					REQUIRE(conflict->kind == Conflict::Kind::EDGE_IN_POLYGON);
				AND_THEN("The registered conflicts should be between the right edges and polygons") {
					bool is_first_registered = false;
					bool is_second_registered = false;
					for(std::unique_ptr<Conflict>& conflict : b->conflicts) {
						auto c = dynamic_cast<ConflictEdgeInPolygon*>(conflict.get());
						if(c->edge == b->polygons[0]->edges[1].get()) {
							is_first_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[1].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 2, 2 }, { 3, 2 }));
							REQUIRE(*std::get<2>(c->overlaps[0]) == b->polygons[1]->edges[3].get());
						} else if(c->edge == b->polygons[1]->edges[3].get()) {
							is_second_registered = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 3, 2 }, { 2, 2 }));
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 2, 2 }, { 3, 2 }));
							REQUIRE(*std::get<2>(c->overlaps[0]) == b->polygons[0]->edges[1].get());
						}
					}
					REQUIRE(is_first_registered);
					REQUIRE(is_second_registered);
				}
				AND_THEN("The edges and polygons should register the conflicts") {
					REQUIRE(b->polygons[0]->edges[1]->conflicts.size() == 1);
					REQUIRE(b->polygons[1]->edges[3]->conflicts.size() == 1);
					REQUIRE(b->polygons[0]->conflicts.size() == 1);
					REQUIRE(b->polygons[1]->conflicts.size() == 1);

					auto p0c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->conflicts[0]);
					auto p1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->conflicts[0]);
					auto p0e1c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[0]->edges[1]->conflicts[0]);
					auto p1e3c0 = dynamic_cast<ConflictEdgeInPolygon*>(b->polygons[1]->edges[3]->conflicts[0]);
					REQUIRE(std::get<0>(p0c0->overlaps[0]) == b->polygons[0].get());
					REQUIRE(std::get<0>(p1c0->overlaps[0]) == b->polygons[1].get());
					REQUIRE(p0e1c0->edge == b->polygons[0]->edges[1].get());
					REQUIRE(p1e3c0->edge == b->polygons[1]->edges[3].get());
				}
			}
		}

		WHEN("Polygons touch each other by a corner") {
			std::unique_ptr<Board> b;
			{
				std::vector<std::unique_ptr<Polygon>> tmp;
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 1, 2 }, { 2, 2 }, { 2, 3 }, { 1, 3 }})));
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 2, 1 }, { 3, 1 }, { 3, 2 }, { 2, 2 }})));
				b = std::make_unique<Board>(tmp);
			}
			b->detect_edges_in_polygons();
			THEN("There should not be any conflict registered") {
				REQUIRE_FALSE(b->conflicts.size());
			}
		}

		WHEN("Polygons are apart") {
			std::unique_ptr<Board> b;
			{
				std::vector<std::unique_ptr<Polygon>> tmp;
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 1, 1 }, { 2, 1 }, { 2, 2 }, { 2, 1 }})));
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 3, 3 }, { 3, 4 }, { 4, 4 }, { 4, 3 }})));
				b = std::make_unique<Board>(tmp);
			}
			b->detect_edges_in_polygons();
			THEN("There should not be any conflict registered") {
				REQUIRE_FALSE(b->conflicts.size());
			}
		}
	}

	GIVEN("A board holding two complex polygons") {
		WHEN("Polygons overlap by many segments") {
			std::unique_ptr<Board> b;
			{
				std::vector<std::unique_ptr<Polygon>> tmp;
				tmp.push_back(std::make_unique<Polygon>(Polygon({
					{ 1, 1 }, { 10, 1 }, { 10, 10 }, { 1, 10 }})));
				tmp.push_back(std::make_unique<Polygon>(Polygon({
					{ 11, 4 }, { 9, 3 }, { 13, 2 }, { 5, 2 },
					{ 5, 11 }, { 4, 11.3 }, { 3, 10 }, { 2, 10 },
					{ 2.3, 12 }, { 6, 11.3 }, { 6, 9 }, { 10, 10 },
					{ 11, 9 }, { 9, 7 }, { 10, 6.3 }, { 10, 5 }})));
				b = std::make_unique<Board>(tmp);
			}
			b->detect_edges_in_polygons();
			THEN("12 EDGE_IN_POLYGON conflicts should be registered") {
				REQUIRE(b->conflicts.size() == 12);
				for(std::unique_ptr<Conflict>& conflict : b->conflicts)
					REQUIRE(conflict->kind == Conflict::Kind::EDGE_IN_POLYGON);
				AND_THEN("The registered conflicts should be between the right edges and polygons") {
					std::array<bool, 12> are_conflicts_registered;
					are_conflicts_registered.fill(false);
					for(std::unique_ptr<Conflict>& conflict : b->conflicts) {
						auto c = dynamic_cast<ConflictEdgeInPolygon*>(conflict.get());
						if(c->edge == b->polygons[0]->edges[2].get()) {
							are_conflicts_registered[0] = true;
							REQUIRE(c->overlaps.size() == 4);
							std::array<bool, 4> are_overlaps_registered;
							are_overlaps_registered.fill(false);
							for(auto const& overlap : c->overlaps) {
								if(std::get<0>(overlap) == b->polygons[1].get()
								&& *std::get<1>(overlap) == Range({ 10, 2 }, { 10, 2.75 })
								&& std::get<2>(overlap) == std::nullopt)
									are_overlaps_registered[0] = true;
								else if(std::get<0>(overlap) == b->polygons[1].get()
								     && *std::get<1>(overlap) == Range({ 10, 3.5 }, { 10, 5 })
								     && std::get<2>(overlap) == std::nullopt)
									are_overlaps_registered[1] = true;
								else if(std::get<0>(overlap) == b->polygons[1].get()
								     && *std::get<1>(overlap) == Range({ 10, 5 }, { 10, 6.3 })
								     && std::get<2>(overlap) == b->polygons[1]->edges[15].get())
									are_overlaps_registered[2] = true;
								else if(std::get<0>(overlap) == b->polygons[1].get()
								     && *std::get<1>(overlap) == Range({ 10, 8 }, { 10, 10 })
								     && std::get<2>(overlap) == std::nullopt)
									are_overlaps_registered[3] = true;
							}
							for(bool is_overlap_registered : are_overlaps_registered)
								REQUIRE(is_overlap_registered);
						} else if(c->edge == b->polygons[0]->edges[3].get()) {
							are_conflicts_registered[1] = true;
							REQUIRE(c->overlaps.size() == 2);
							std::array<bool, 2> are_overlaps_registered;
							are_overlaps_registered.fill(false);
							for(auto const& overlap : c->overlaps) {
								if(std::get<0>(overlap) == b->polygons[1].get()
								&& *std::get<1>(overlap) == Range({ 5, 10 }, { 6, 10 })
								&& std::get<2>(overlap) == std::nullopt)
									are_overlaps_registered[0] = true;
								else if(std::get<0>(overlap) == b->polygons[1].get()
								     && *std::get<1>(overlap) == Range({ 3, 10 }, { 2, 10 })
								     && std::get<2>(overlap) == b->polygons[1]->edges[7].get())
									are_overlaps_registered[1] = true;
							}
							for(bool is_overlap_registered : are_overlaps_registered)
								REQUIRE(is_overlap_registered);
						} else if(c->edge == b->polygons[1]->edges[1].get()) {
							are_conflicts_registered[2] = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 10, 3.5 }, { 9, 3 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[2].get()) {
							are_conflicts_registered[3] = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 9, 3 }, { 10, 2.75 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[3].get()) {
							are_conflicts_registered[4] = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 10, 2 }, { 5, 2 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[4].get()) {
							are_conflicts_registered[5] = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 5, 2 }, { 5, 10 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[7].get()) {
							are_conflicts_registered[6] = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 3, 10 }, { 2, 10 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == b->polygons[0]->edges[3].get());
						} else if(c->edge == b->polygons[1]->edges[10].get()) {
							are_conflicts_registered[7] = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 6, 10 }, { 6, 9 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[11].get()) {
							are_conflicts_registered[8] = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 6, 9 }, { 10, 10 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[13].get()) {
							are_conflicts_registered[9] = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 10, 8 }, { 9, 7 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[14].get()) {
							are_conflicts_registered[10] = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 9, 7 }, { 10, 6.3 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == std::nullopt);
						} else if(c->edge == b->polygons[1]->edges[15].get()) {
							are_conflicts_registered[11] = true;
							REQUIRE(c->overlaps.size() == 1);
							REQUIRE(std::get<0>(c->overlaps[0]) == b->polygons[0].get());
							REQUIRE(*std::get<1>(c->overlaps[0]) == Range({ 10, 6.3 }, { 10, 5 }));
							REQUIRE(std::get<2>(c->overlaps[0]) == b->polygons[0]->edges[2].get());
						}
					}
					for(bool is_conflict_registered : are_conflicts_registered)
						REQUIRE(is_conflict_registered);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("void detect_colinear_edges()", "[board]") {
	GIVEN("A board holding three polygons") {
		WHEN("Three polygons share a colinear edge") {
			std::unique_ptr<Board> b;
			{
				std::vector<std::unique_ptr<Polygon>> tmp;
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 1, 1 }, { 2, 1 }, { 2, 2 }, { 1, 2 }})));
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 0.5, 3 }, { 2, 3 }, { 2, 4 }, { 0.5, 4 }})));
				tmp.push_back(std::make_unique<Polygon>(Polygon({{ 3, 5 }, { 2, 5 }, { 2, 6 }, { 3, 6 }})));
				b = std::make_unique<Board>(tmp);
			}
			b->detect_colinear_edges();
			THEN("A COLINEAR_EDGES conflict should be registered") {
				REQUIRE(b->conflicts.size() == 1);
				REQUIRE(b->conflicts.back()->kind == Conflict::Kind::COLINEAR_EDGES);
				AND_THEN("The registered conflict should be between tree edges") {
					auto c = dynamic_cast<ConflictColinearEdges*>(b->conflicts.back().get());
					REQUIRE(c->edges.size() == 3);
					bool is_first_registered = false;
					bool is_second_registered = false;
					bool is_third_registered = false;
					for(Edge* edge : c->edges) {
						if(edge == b->polygons[0]->edges[2].get())
							is_first_registered = true;
						if(edge == b->polygons[1]->edges[2].get())
							is_second_registered = true;
						if(edge == b->polygons[2]->edges[2].get())
							is_third_registered = true;
					}
					REQUIRE(is_first_registered);
					REQUIRE(is_second_registered);
					REQUIRE(is_third_registered);
				}
				AND_THEN("The three edges should register the conflict") {
					auto c = dynamic_cast<ConflictColinearEdges*>(b->conflicts.back().get());
					REQUIRE(b->polygons[0]->edges[2]->conflicts.size() == 1);
					REQUIRE(b->polygons[1]->edges[2]->conflicts.size() == 1);
					REQUIRE(b->polygons[2]->edges[2]->conflicts.size() == 1);
					REQUIRE(b->polygons[0]->edges[2]->conflicts[0] == c);
					REQUIRE(b->polygons[1]->edges[2]->conflicts[0] == c);
					REQUIRE(b->polygons[2]->edges[2]->conflicts[0] == c);
				}
			}
		}
	}
}
