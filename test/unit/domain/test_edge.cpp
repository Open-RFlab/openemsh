///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/point.hpp"
#include "domain/range.hpp"

#include "domain/edge.hpp"

/// @test relation::EdgeEdge Edge::relation_to(Edge const* edge) const
/// @test relation::EdgePoint Edge::relation_to(Point const* point) const
/// @test std::optional<Point> intersection(Edge const* a, Edge const* b)
/// @test std::optional<Range> overlap(Edge const* a, Edge const* b)
///*****************************************************************************

//******************************************************************************
SCENARIO("relation::EdgeEdge Edge::relation_to(Edge const* edge) const", "[edge]") {
	GIVEN("Two edges") {
		WHEN("A vertical edge and an horizontal edge are crossing") {
			Point a0(1, 2), a1(3, 2);
			Point b0(2, 1), b1(2, 3);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as CROSSING") {
				REQUIRE(a.axis == Edge::Axis::X);
				REQUIRE(b.axis == Edge::Axis::Y);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::CROSSING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::CROSSING);
				REQUIRE(c.axis == Edge::Axis::X);
				REQUIRE(d.axis == Edge::Axis::Y);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::CROSSING);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::CROSSING);
			}
		}

		WHEN("Two diagonal edges are crossing") {
			Point a0(1, 1), a1(3, 3);
			Point b0(3, 1), b1(1, 3);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as CROSSING") {
				REQUIRE(a.axis == Edge::Axis::DIAGONAL);
				REQUIRE(b.axis == Edge::Axis::DIAGONAL);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::CROSSING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::CROSSING);
				REQUIRE(c.axis == Edge::Axis::DIAGONAL);
				REQUIRE(d.axis == Edge::Axis::DIAGONAL);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::CROSSING);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::CROSSING);
			}
		}

		WHEN("Two vertical edges are colinear") {
			Point a0(1, 1), a1(1, 2);
			Point b0(1, 3), b1(1, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as COLINEAR") {
				REQUIRE(a.axis == Edge::Axis::Y);
				REQUIRE(b.axis == Edge::Axis::Y);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::COLINEAR);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::COLINEAR);
				REQUIRE(c.axis == Edge::Axis::Y);
				REQUIRE(d.axis == Edge::Axis::Y);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::COLINEAR);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::COLINEAR);
			}
		}

		WHEN("Two horizontal edges are colinear") {
			Point a0(1, 1), a1(2, 1);
			Point b0(3, 1), b1(4, 1);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as COLINEAR") {
				REQUIRE(a.axis == Edge::Axis::X);
				REQUIRE(b.axis == Edge::Axis::X);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::COLINEAR);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::COLINEAR);
				REQUIRE(c.axis == Edge::Axis::X);
				REQUIRE(d.axis == Edge::Axis::X);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::COLINEAR);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::COLINEAR);
			}
		}

		WHEN("Two diagonal edges are colinear") {
			Point a0(1, 1), a1(2, 2);
			Point b0(3, 3), b1(4, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as COLINEAR") {
				REQUIRE(a.axis == Edge::Axis::DIAGONAL);
				REQUIRE(b.axis == Edge::Axis::DIAGONAL);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::COLINEAR);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::COLINEAR);
				REQUIRE(c.axis == Edge::Axis::DIAGONAL);
				REQUIRE(d.axis == Edge::Axis::DIAGONAL);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::COLINEAR);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::COLINEAR);
			}
		}

		WHEN("Two vertical edges are overlapping") {
			Point a0(1, 1), a1(1, 3);
			Point b0(1, 2), b1(1, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as OVERLAPPING") {
				REQUIRE(a.axis == Edge::Axis::Y);
				REQUIRE(b.axis == Edge::Axis::Y);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(c.axis == Edge::Axis::Y);
				REQUIRE(d.axis == Edge::Axis::Y);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::OVERLAPPING);
			}
		}

		WHEN("Two horizontal edges are overlapping") {
			Point a0(1, 1), a1(3, 1);
			Point b0(2, 1), b1(4, 1);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as OVERLAPPING") {
				REQUIRE(a.axis == Edge::Axis::X);
				REQUIRE(b.axis == Edge::Axis::X);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(c.axis == Edge::Axis::X);
				REQUIRE(d.axis == Edge::Axis::X);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::OVERLAPPING);
			}
		}

		WHEN("Two diagonal edges are overlapping") {
			Point a0(1, 1), a1(3, 3);
			Point b0(2, 2), b1(4, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as OVERLAPPING") {
				REQUIRE(a.axis == Edge::Axis::DIAGONAL);
				REQUIRE(b.axis == Edge::Axis::DIAGONAL);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(c.axis == Edge::Axis::DIAGONAL);
				REQUIRE(d.axis == Edge::Axis::DIAGONAL);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::OVERLAPPING);
			}
		}

		WHEN("Two vertical edges are colinear and touching by just an extremity") {
			Point a0(1, 1), a1(1, 2);
			Point b0(1, 2), b1(1, 3);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as OVERLAPPING") {
				REQUIRE(a.axis == Edge::Axis::Y);
				REQUIRE(b.axis == Edge::Axis::Y);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(c.axis == Edge::Axis::Y);
				REQUIRE(d.axis == Edge::Axis::Y);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::OVERLAPPING);
			}
		}

		WHEN("Two horizontal edges are colinear and touching by just an extremity") {
			Point a0(1, 1), a1(2, 1);
			Point b0(2, 1), b1(3, 1);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as OVERLAPPING") {
				REQUIRE(a.axis == Edge::Axis::X);
				REQUIRE(b.axis == Edge::Axis::X);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(c.axis == Edge::Axis::X);
				REQUIRE(d.axis == Edge::Axis::X);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::OVERLAPPING);
			}
		}

		WHEN("Two diagonal edges are colinear and touching by just an extremity") {
			Point a0(1, 1), a1(2, 2);
			Point b0(2, 2), b1(3, 3);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as OVERLAPPING") {
				REQUIRE(a.axis == Edge::Axis::DIAGONAL);
				REQUIRE(b.axis == Edge::Axis::DIAGONAL);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(c.axis == Edge::Axis::DIAGONAL);
				REQUIRE(d.axis == Edge::Axis::DIAGONAL);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::OVERLAPPING);
			}
		}

		WHEN("A vertical edge is totally inside an other vertical edge") {
			Point a0(1, 1), a1(1, 4);
			Point b0(1, 2), b1(1, 3);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as OVERLAPPING") {
				REQUIRE(a.axis == Edge::Axis::Y);
				REQUIRE(b.axis == Edge::Axis::Y);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(c.axis == Edge::Axis::Y);
				REQUIRE(d.axis == Edge::Axis::Y);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::OVERLAPPING);
			}
		}

		WHEN("An horizontal edge is totally inside an other horizontal edge") {
			Point a0(1, 1), a1(4, 1);
			Point b0(2, 1), b1(3, 1);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as OVERLAPPING") {
				REQUIRE(a.axis == Edge::Axis::X);
				REQUIRE(b.axis == Edge::Axis::X);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(c.axis == Edge::Axis::X);
				REQUIRE(d.axis == Edge::Axis::X);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::OVERLAPPING);
			}
		}

		WHEN("A diagonal edge is totally inside an other diagonal edge") {
			Point a0(1, 1), a1(4, 4);
			Point b0(2, 2), b1(3, 3);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as OVERLAPPING") {
				REQUIRE(a.axis == Edge::Axis::DIAGONAL);
				REQUIRE(b.axis == Edge::Axis::DIAGONAL);
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(c.axis == Edge::Axis::DIAGONAL);
				REQUIRE(d.axis == Edge::Axis::DIAGONAL);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::OVERLAPPING);
			}
		}

		WHEN("Two diagonal edges are apart") {
			Point a0(1, 1), a1(2, 2);
			Point b0(3, 3), b1(4, 5);
			Edge a(&a0, &a1), b(&b0, &b1);
			Edge c(&a1, &a0), d(&b1, &b0);
			THEN("Should be detected as APART") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::APART);
				REQUIRE(b.relation_to(&a) == relation::EdgeEdge::APART);
				REQUIRE(c.relation_to(&d) == relation::EdgeEdge::APART);
				REQUIRE(d.relation_to(&c) == relation::EdgeEdge::APART);
			}
		}
	}
}

//******************************************************************************
SCENARIO("relation::EdgePoint Edge::relation_to(Point const* point) const", "[edge]") {
	GIVEN("An edge") {
		Point e0(1, 1), e1(3, 3);
		Edge e(&e0, &e1);
		WHEN("A point is on the edge") {
			Point p(2, 2);
			THEN("Should be detected as ON") {
				REQUIRE(e.relation_to(&p) == relation::EdgePoint::ON);
			}
		}

		WHEN("A point is not on the edge") {
			Point p(1, 2);
			THEN("Should be detected as OUT") {
				REQUIRE(e.relation_to(&p) == relation::EdgePoint::OUT);
			}
		}

		WHEN("A point is on a point of the edge") {
			Point p(1, 1);
			THEN("Should be detected as ON") {
				REQUIRE(e.relation_to(&p) == relation::EdgePoint::ON);
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

		WHEN("Two edges are colinear and touching by just an extremity") {
			Point a0(1, 1), a1(1, 2);
			Point b0(1, 2), b1(1, 3);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Point> p(intersection(&a, &b));
			THEN("") {
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
// TODO
//				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::APART);
//				REQUIRE_FALSE(p);
			}
		}

	}
}

//******************************************************************************
SCENARIO("std::optional<Range> overlap(Edge const* a, Edge const* b)", "[edge]") {
	GIVEN("Two edges") {
		WHEN("Two horizontal edges are overlapping") {
			Point a0(1, 1), a1(3, 1);
			Point b0(2, 1), b1(4, 1);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Range> r(overlap(&a, &b));
			std::optional<Range> s(overlap(&b, &a));
			THEN("Should calcul overlap range") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Range::Axis::X);
				REQUIRE(r->p0.x == 2);
				REQUIRE(r->p1.x == 3);
				REQUIRE(r->p0.y == 1);
				REQUIRE(r->p1.y == 1);
				REQUIRE(s);
				REQUIRE(s->axis == Range::Axis::X);
				REQUIRE(s->p0.x == 2);
				REQUIRE(s->p1.x == 3);
				REQUIRE(s->p0.y == 1);
				REQUIRE(s->p1.y == 1);
			}
		}

		WHEN("An horizontal edge is totally inside another horizontal edge") {
			Point a0(1, 1), a1(4, 1);
			Point b0(3, 1), b1(2, 1);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Range> r(overlap(&a, &b));
			std::optional<Range> s(overlap(&b, &a));
			THEN("Should calcul overlap range") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Range::Axis::X);
				REQUIRE(r->p0.x == 2);
				REQUIRE(r->p1.x == 3);
				REQUIRE(r->p0.y == 1);
				REQUIRE(r->p1.y == 1);
				REQUIRE(s);
				REQUIRE(s->axis == Range::Axis::X);
				REQUIRE(s->p0.x == 2);
				REQUIRE(s->p1.x == 3);
				REQUIRE(s->p0.y == 1);
				REQUIRE(s->p1.y == 1);
			}
		}

		WHEN("Two horizontal edges are colinear and touching by just an extremity") {
			Point a0(1, 1), a1(2, 1);
			Point b0(2, 1), b1(3, 1);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Range> r(overlap(&a, &b));
			std::optional<Range> s(overlap(&a, &b));
			THEN("Should calcul overlap range that should be made of two equal points") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Range::Axis::POINT);
				REQUIRE(r->p0 == r->p1);
				REQUIRE(r->p0.x == 2);
				REQUIRE(r->p1.x == 2);
				REQUIRE(r->p0.y == 1);
				REQUIRE(r->p1.y == 1);
				REQUIRE(s);
				REQUIRE(s->axis == Range::Axis::POINT);
				REQUIRE(s->p0 == s->p1);
				REQUIRE(s->p0.x == 2);
				REQUIRE(s->p1.x == 2);
				REQUIRE(s->p0.y == 1);
				REQUIRE(s->p1.y == 1);
			}
		}

		WHEN("Two horizontal edges are colinear") {
			Point a0(1, 1), a1(2, 1);
			Point b0(3, 1), b1(4, 1);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Range> r(overlap(&a, &b));
			std::optional<Range> s(overlap(&b, &a));
			THEN("There should not be any overlap range") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::COLINEAR);
				REQUIRE_FALSE(r);
				REQUIRE_FALSE(s);
			}
		}

		WHEN("Two horizontal edges are parallels") {
			Point a0(1, 1), a1(2, 1);
			Point b0(3, 2), b1(4, 2);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Range> r(overlap(&a, &b));
			std::optional<Range> s(overlap(&b, &a));
			THEN("There should not be any overlap range") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::APART);
				REQUIRE_FALSE(r);
				REQUIRE_FALSE(s);
			}
		}

		WHEN("Two vertical edges are overlapping") {
			Point a0(1, 1), a1(1, 3);
			Point b0(1, 2), b1(1, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Range> r(overlap(&a, &b));
			std::optional<Range> s(overlap(&b, &a));
			THEN("Should calcul overlap range") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Range::Axis::Y);
				REQUIRE(r->p0.y == 2);
				REQUIRE(r->p1.y == 3);
				REQUIRE(r->p0.x == 1);
				REQUIRE(r->p1.x == 1);
				REQUIRE(s);
				REQUIRE(s->axis == Range::Axis::Y);
				REQUIRE(s->p0.y == 2);
				REQUIRE(s->p1.y == 3);
				REQUIRE(s->p0.x == 1);
				REQUIRE(s->p1.x == 1);
			}
		}

		WHEN("A vertical edge is totally inside another vertical edge") {
			Point a0(1, 1), a1(1, 4);
			Point b0(1, 2), b1(1, 3);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Range> r(overlap(&a, &b));
			std::optional<Range> s(overlap(&b, &a));
			THEN("Should calcul overlap range") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Range::Axis::Y);
				REQUIRE(r->p0.y == 2);
				REQUIRE(r->p1.y == 3);
				REQUIRE(r->p0.x == 1);
				REQUIRE(r->p1.x == 1);
				REQUIRE(s);
				REQUIRE(s->axis == Range::Axis::Y);
				REQUIRE(s->p0.y == 2);
				REQUIRE(s->p1.y == 3);
				REQUIRE(s->p0.x == 1);
				REQUIRE(s->p1.x == 1);
			}
		}

		WHEN("Two vertical edges are colinear and touching by just an extremity") {
			Point a0(1, 1), a1(1, 2);
			Point b0(1, 2), b1(1, 3);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Range> r(overlap(&a, &b));
			std::optional<Range> s(overlap(&a, &b));
			THEN("Should calcul overlap range that should be made of two equal points") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Range::Axis::POINT);
				REQUIRE(r->p0 == r->p1);
				REQUIRE(r->p0.x == 1);
				REQUIRE(r->p1.x == 1);
				REQUIRE(r->p0.y == 2);
				REQUIRE(r->p1.y == 2);
				REQUIRE(s);
				REQUIRE(s->axis == Range::Axis::POINT);
				REQUIRE(s->p0 == s->p1);
				REQUIRE(s->p0.x == 1);
				REQUIRE(s->p1.x == 1);
				REQUIRE(s->p0.y == 2);
				REQUIRE(s->p1.y == 2);
			}
		}

		WHEN("Two vertical edges are colinear") {
			Point a0(1, 1), a1(1, 2);
			Point b0(1, 3), b1(1, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Range> r(overlap(&a, &b));
			std::optional<Range> s(overlap(&b, &a));
			THEN("There should not be any overlap range") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::COLINEAR);
				REQUIRE_FALSE(r);
				REQUIRE_FALSE(s);
			}
		}

		WHEN("Two vertical edges are parallels") {
			Point a0(1, 1), a1(1, 2);
			Point b0(2, 3), b1(2, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Range> r(overlap(&a, &b));
			std::optional<Range> s(overlap(&b, &a));
			THEN("There should not be any overlap range") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::APART);
				REQUIRE_FALSE(r);
				REQUIRE_FALSE(s);
			}
		}

		WHEN("Two diagonal edges are overlapping") {
			Point a0(1, 1), a1(3, 3);
			Point b0(2, 2), b1(4, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Range> r(overlap(&a, &b));
			std::optional<Range> s(overlap(&b, &a));
			THEN("Should calcul overlap range") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Range::Axis::DIAGONAL);
				REQUIRE(r->p0.x == 2);
				REQUIRE(r->p1.x == 3);
				REQUIRE(r->p0.y == 2);
				REQUIRE(r->p1.y == 3);
				REQUIRE(s);
				REQUIRE(s->axis == Range::Axis::DIAGONAL);
				REQUIRE(s->p0.x == 2);
				REQUIRE(s->p1.x == 3);
				REQUIRE(s->p0.y == 2);
				REQUIRE(s->p1.y == 3);
			}
		}

		WHEN("Two diagonal edges are colinear and touching by just an extremity") {
			Point a0(1, 1), a1(2, 2);
			Point b0(2, 2), b1(3, 3);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Range> r(overlap(&a, &b));
			std::optional<Range> s(overlap(&a, &b));
			THEN("Should calcul overlap range that should be made of two equal points") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::OVERLAPPING);
				REQUIRE(r);
				REQUIRE(r->axis == Range::Axis::POINT);
				REQUIRE(r->p0 == r->p1);
				REQUIRE(r->p0.x == 2);
				REQUIRE(r->p1.x == 2);
				REQUIRE(r->p0.y == 2);
				REQUIRE(r->p1.y == 2);
				REQUIRE(s);
				REQUIRE(s->axis == Range::Axis::POINT);
				REQUIRE(s->p0 == s->p1);
				REQUIRE(s->p0.x == 2);
				REQUIRE(s->p1.x == 2);
				REQUIRE(s->p0.y == 2);
				REQUIRE(s->p1.y == 2);
			}
		}

		WHEN("Two edges are crossing") {
			Point a0(1, 1), a1(3, 3);
			Point b0(1, 3), b1(3, 1);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Range> r(overlap(&a, &b));
			std::optional<Range> s(overlap(&b, &a));
			THEN("There should not be any overlap range") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::CROSSING);
				REQUIRE_FALSE(r);
				REQUIRE_FALSE(s);
			}
		}

		WHEN("Two edges are apart") {
			Point a0(1, 1), a1(1, 2);
			Point b0(2, 2), b1(4, 4);
			Edge a(&a0, &a1), b(&b0, &b1);
			std::optional<Range> r(overlap(&a, &b));
			std::optional<Range> s(overlap(&b, &a));
			THEN("There should not be any overlap range") {
				REQUIRE(a.relation_to(&b) == relation::EdgeEdge::APART);
				REQUIRE_FALSE(r);
				REQUIRE_FALSE(s);
			}
		}
	}
}