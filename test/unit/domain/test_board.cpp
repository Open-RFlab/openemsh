///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/conflict_colinear_edges.hpp"

#include "domain/board.hpp"

/// @test void detect_edges_in_polygons()
/// @test void detect_colinear_edges()
///*****************************************************************************

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
			}
			THEN("The registered conflict should be between tree edges") {
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
			THEN("The three edges should register the conflict") {
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