///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/point.hpp"

#include "domain/edge.hpp"

/// @test relation::EdgeEdge Edge::relation_to(Edge const* edge)
/// @test std::optional<Point> intersection(Edge const* a, Edge const* b)
///*****************************************************************************

//******************************************************************************
SCENARIO("relation::EdgeEdge Edge::relation_to(Edge const* edge)", "[edge]") {
	GIVEN("Two edges") {
		WHEN("A vertical edge and an horizontal edge are crossing") {
			Point a0(1, 2), a1(3, 2);
			Point b0(2, 1), b1(2, 3);
			Edge a(&a0, &a1), b(&b0, &b1);
			THEN("Should be detected as CROSSING") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::CROSSING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::CROSSING);
			}
		}

		WHEN("Two diagonal edges are crossing") {
			Point a0(1, 1), a1(3, 3);
			Point b0(3, 1), b1(1, 3);
			Edge a(&a0, &a1), b(&b0, &b1);
			THEN("Should be detected as CROSSING") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::CROSSING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::CROSSING);
			}
		}

		WHEN("Two vertical edges are colinear") {
			Point a0(1, 1), a1(1, 2);
			Point b0(1, 3), b1(1, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			THEN("Should be detected as COLINEAR") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::COLINEAR);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::COLINEAR);
			}
		}

		WHEN("Two horizontal edges are colinear") {
			Point a0(1, 1), a1(2, 1);
			Point b0(3, 1), b1(4, 1);
			Edge a(&a0, &a1), b(&b0, &b1);
			THEN("Should be detected as COLINEAR") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::COLINEAR);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::COLINEAR);
			}
		}

		WHEN("Two diagonal edges are colinear") {
			Point a0(1, 1), a1(2, 2);
			Point b0(3, 3), b1(4, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			THEN("Should be detected as COLINEAR") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::COLINEAR);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::COLINEAR);
			}
		}

		WHEN("Two vertical edges are overlapping") {
			Point a0(1, 1), a1(1, 3);
			Point b0(1, 2), b1(1, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			THEN("Should be detected as OVERLAPPING") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::OVERLAPPING);
			}
		}

		WHEN("Two horizontal edges are overlapping") {
			Point a0(1, 1), a1(3, 1);
			Point b0(2, 1), b1(4, 1);
			Edge a(&a0, &a1), b(&b0, &b1);
			THEN("Should be detected as OVERLAPPING") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::OVERLAPPING);
			}
		}

		WHEN("Two diagonal edges are overlapping") {
			Point a0(1, 1), a1(3, 3);
			Point b0(2, 2), b1(4, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			THEN("Should be detected as OVERLAPPING") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::OVERLAPPING);
			}
		}

		WHEN("Two diagonal edges are apart") {
			Point a0(1, 1), a1(2, 2);
			Point b0(3, 3), b1(4, 5);
			Edge a(&a0, &a1), b(&b0, &b1);
			THEN("Should be detected as APART") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::APART);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::APART);
			}
		}
	}
}

//******************************************************************************
SCENARIO("std::optional<Point> intersection(Edge const* a, Edge const* b)", "[edge]") {
	GIVEN("Two edges") {
		WHEN("A vertical edge and an horizontal edge are crossing") {
			Point a0(1, 2), a1(3, 2); // Horizontal
			Point b0(2, 1), b1(2, 3); // Vertical
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Point> p_hv(intersection(&a, &b));
			std::optional<Point> p_vh(intersection(&b, &a));
			Point x(2, 2);
			THEN("Should calcul intersection point") {
				REQUIRE((a.direction == Edge::Direction::XMIN || a.direction == Edge::Direction::XMAX));
				REQUIRE((b.direction == Edge::Direction::YMIN || b.direction == Edge::Direction::YMAX));
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::CROSSING);
				REQUIRE(p_hv);
				REQUIRE(p_vh);
				REQUIRE((p_hv->x == x.x && p_hv->y == x.y));
				REQUIRE((p_vh->x == x.x && p_vh->y == x.y));
			}
		}

		WHEN("A diagonal edge and a random edge are crossing") {
			Point a0(1, 1), a1(3, 3);
			Point b0(3, 1), b1(1, 3);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Point> p_hv(intersection(&a, &b));
			std::optional<Point> p_vh(intersection(&b, &a));
			Point x(2, 2);
			THEN("Should calcul intersection point") {
				REQUIRE(a.direction == Edge::Direction::DIAGONAL);
				REQUIRE(b.direction == Edge::Direction::DIAGONAL);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::CROSSING);
				REQUIRE(p_hv);
				REQUIRE(p_vh);
				REQUIRE((p_hv->x == x.x && p_hv->y == x.y));
				REQUIRE((p_vh->x == x.x && p_vh->y == x.y));
			}
		}

		WHEN("Two edges are colinear") {
			Point a0(1, 1), a1(1, 2);
			Point b0(1, 3), b1(1, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Point> p(intersection(&a, &b));
			THEN("There should not be any intersection point") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::COLINEAR);
				REQUIRE_FALSE(p);
			}
		}

		WHEN("Two edges are overlapping") {
			Point a0(1, 1), a1(1, 3);
			Point b0(1, 2), b1(1, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Point> p(intersection(&a, &b));
			THEN("There should not be any intersection point") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE_FALSE(p);
			}
		}

		WHEN("Two edges are apart") {
			Point a0(1, 1), a1(2, 2);
			Point b0(3, 3), b1(4, 5);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Point> p(intersection(&a, &b));
			THEN("There should not be any intersection point") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::APART);
				REQUIRE_FALSE(p);
			}
		}

	}
}

//******************************************************************************
SCENARIO("Range* overlap(Edge const* a, Edge const* b)", "[edge]") {
}