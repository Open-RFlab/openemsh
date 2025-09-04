///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/geometrics/space.hpp"
#include "utils/vector_utils.hpp"
#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/point.hpp"

#include "domain/geometrics/polygon.hpp"

/// @test Bounding2D detect_bounding(std::vector<std::unique_ptr<Point const>> const& points) noexcept
/// @test std::vector<std::shared_ptr<Edge>> detect_edges(std::vector<std::unique_ptr<Point const>> const& points, Plane plane)
/// @test template<class T> Polygon::Rotation detect_rotation(T& points) noexcept
/// @test void Polygon::detect_edge_normal() noexcept
/// @test relation::PolygonPoint Polygon::relation_to(Point const* point) const noexcept
/// @test bool does_overlap(Polygon::RangeZ const& a, Polygon::RangeZ const& b) noexcept
///*****************************************************************************

using namespace domain;

//******************************************************************************
SCENARIO("Bounding2D detect_bounding(std::vector<std::unique_ptr<Point const>> const& points) noexcept", "[polygon]") {
	GIVEN("A random polygon") {
		Bounding2D bounding(detect_bounding(from_init_list<Point>({{ 1, 2 }, { 1, 4 }, { 4, 4 }})));
		THEN("Should calcul bounding box") {
			REQUIRE(bounding[XMIN] == 1);
			REQUIRE(bounding[XMAX] == 4);
			REQUIRE(bounding[YMIN] == 2);
			REQUIRE(bounding[YMAX] == 4);
		}
	}
}

//******************************************************************************
SCENARIO("std::vector<std::shared_ptr<Edge>> detect_edges(std::vector<std::unique_ptr<Point const>> const& points, Plane plane)", "[polygon]") {
	Timepoint t;
	GIVEN("A vector of points") {
		std::vector<std::unique_ptr<Point const>> points(from_init_list<Point>({{ 1, 2 }, { 1, 4 }, { 4, 4 }}));
		std::vector<std::shared_ptr<Edge>> edges(detect_edges(points, XY, &t));
		THEN("Should return a vector of edges") {
			REQUIRE(edges.size() == points.size());
			AND_THEN("First edge should be between last point and first point") {
				REQUIRE(edges[0]->_p0 == points[2].get());
				REQUIRE(edges[0]->_p1 == points[0].get());
			}

			AND_THEN("For any other edge, edge[n] should be between point[n-1] and point[n]"){
				REQUIRE(edges[1]->_p0 == points[0].get());
				REQUIRE(edges[1]->_p1 == points[1].get());
				REQUIRE(edges[2]->_p0 == points[1].get());
				REQUIRE(edges[2]->_p1 == points[2].get());
			}
		}
	}
}

//******************************************************************************
SCENARIO("template<class T> Polygon::Rotation detect_rotation(T& points) noexcept", "[polygon]") {
	Timepoint t;
	GIVEN("A polygon or a bunch of points") {
		WHEN("Points order is oriented clockwise") {
			Polygon a(XY, Polygon::Type::SHAPE, "", { 0, 0 }, from_init_list<Point>({{ 1, 2 }, { 1, 4 }, { 4, 4 }}), &t);
			std::vector<Point const*> b({a.points[0].get(), a.points[1].get(), a.points[2].get()});
			THEN("Should be detected as CW") {
				REQUIRE(detect_rotation(a.points) == Polygon::Rotation::CW);
				REQUIRE(detect_rotation(b) == Polygon::Rotation::CW);
			}
		}

		WHEN("Points order is oriented counter clockwise") {
			Polygon a(XY, Polygon::Type::SHAPE, "", { 0, 0 }, from_init_list<Point>({{ 1, 2 }, { 4, 4 }, { 1, 4 }}), &t);
			std::vector<Point const*> b({a.points[0].get(), a.points[1].get(), a.points[2].get()});
			THEN("Should be detected as CCW") {
				REQUIRE(detect_rotation(a.points) == Polygon::Rotation::CCW);
				REQUIRE(detect_rotation(b) == Polygon::Rotation::CCW);
			}
		}

		WHEN("Points are all aligned in an axis") {
			Polygon a(XY, Polygon::Type::SHAPE, "", { 0, 0 }, from_init_list<Point>({{ 1, 2 }, { 1, 4 }, { 1, 1 }}), &t);
			Polygon b(XY, Polygon::Type::SHAPE, "", { 0, 0 }, from_init_list<Point>({{ 51.3539, -44.8024 }, { 120.0290, -44.8024 }, { 140.2830, -44.8024 }}), &t);
			std::vector<Point const*> c({a.points[0].get(), a.points[1].get(), a.points[2].get()});
			std::vector<Point const*> d({b.points[0].get(), b.points[1].get(), b.points[2].get()});
			THEN("Should be detected as COLINEAR") {
				REQUIRE(detect_rotation(a.points) == Polygon::Rotation::COLINEAR);
				REQUIRE(detect_rotation(b.points) == Polygon::Rotation::COLINEAR);
				REQUIRE(detect_rotation(c) == Polygon::Rotation::COLINEAR);
				REQUIRE(detect_rotation(d) == Polygon::Rotation::COLINEAR);
			}
		}

		WHEN("Points are all aligned in diagonal") {
			Polygon a(XY, Polygon::Type::SHAPE, "", { 0, 0 }, from_init_list<Point>({{ 1, 2 }, { 4, 4 }, { 2.5, 3 }}), &t);
			std::vector<Point const*> b({a.points[0].get(), a.points[1].get(), a.points[2].get()});
			THEN("Should be detected as COLINEAR") {
				REQUIRE(detect_rotation(a.points) == Polygon::Rotation::COLINEAR);
				REQUIRE(detect_rotation(b) == Polygon::Rotation::COLINEAR);
			}
		}

		WHEN("Two points of a triangle are at the same place") {
			Polygon a(XY, Polygon::Type::SHAPE, "", { 0, 0 }, from_init_list<Point>({{ 1, 2 }, { 4, 4 }, { 1, 2 }}), &t);
			std::vector<Point const*> b({a.points[0].get(), a.points[1].get(), a.points[2].get()});
			THEN("Should be detected as COLINEAR") {
				REQUIRE(detect_rotation(a.points) == Polygon::Rotation::COLINEAR);
				REQUIRE(detect_rotation(b) == Polygon::Rotation::COLINEAR);
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Polygon::detect_edge_normal() noexcept", "[polygon]") {
	Timepoint t;
	GIVEN("An octogon : 2 horizontal edges, 2 verticals, 4 diagonals") {
		WHEN("Points order is oriented clockwise") {
			Polygon a(XY, Polygon::Type::SHAPE, "", { 0, 0 }, from_init_list<Point>({
				{ 2, 1 }, { 1, 2 }, { 1, 3 }, { 2, 4 },
				{ 3, 4 }, { 4, 3 }, { 4, 2 }, { 3, 1 }}), &t);
			THEN("For edges going down to the X axis, the normal should go down to the Y") {
				REQUIRE(a.edges[0]->direction == Edge::Direction::XMIN);
				REQUIRE(a.edges[0]->normal == Normal::YMIN);
			}
			THEN("For edges going up to the X axis, the normal should go up to the Y") {
				REQUIRE(a.edges[4]->direction == Edge::Direction::XMAX);
				REQUIRE(a.edges[4]->normal == Normal::YMAX);
			}
			THEN("For edges going down to the Y axis, the normal should go up to the X") {
				REQUIRE(a.edges[6]->direction == Edge::Direction::YMIN);
				REQUIRE(a.edges[6]->normal == Normal::XMAX);
			}
			THEN("For edges going up to the Y axis, the normal should go down to the X") {
				REQUIRE(a.edges[2]->direction == Edge::Direction::YMAX);
				REQUIRE(a.edges[2]->normal == Normal::XMIN);
			}
			THEN("For diagonal edges, there should not be any normal") {
				REQUIRE(a.edges[1]->direction == Edge::Direction::NONE);
				REQUIRE(a.edges[3]->direction == Edge::Direction::NONE);
				REQUIRE(a.edges[5]->direction == Edge::Direction::NONE);
				REQUIRE(a.edges[7]->direction == Edge::Direction::NONE);
				REQUIRE(a.edges[1]->normal == Normal::NONE);
				REQUIRE(a.edges[3]->normal == Normal::NONE);
				REQUIRE(a.edges[5]->normal == Normal::NONE);
				REQUIRE(a.edges[7]->normal == Normal::NONE);
			}
		}

		WHEN("Points order is oriented counter clockwise") {
			Polygon a(XY, Polygon::Type::SHAPE, "", { 0, 0 }, from_init_list<Point>({
				{ 2, 1 }, { 3, 1 }, { 4, 2 }, { 4, 3 },
				{ 3, 4 }, { 2, 4 }, { 1, 3 }, { 1, 2 }}), &t);
			THEN("For edges going down to the X axis, the normal should go up to the Y") {
				REQUIRE(a.edges[5]->direction == Edge::Direction::XMIN);
				REQUIRE(a.edges[5]->normal == Normal::YMAX);
			}
			THEN("For edges going up to the X axis, the normal should go down to the Y") {
				REQUIRE(a.edges[1]->direction == Edge::Direction::XMAX);
				REQUIRE(a.edges[1]->normal == Normal::YMIN);
			}
			THEN("For edges going down to the Y axis, the normal should go down to the X") {
				REQUIRE(a.edges[7]->direction == Edge::Direction::YMIN);
				REQUIRE(a.edges[7]->normal == Normal::XMIN);
			}
			THEN("For edges going up to the Y axis, the normal should go up to the X") {
				REQUIRE(a.edges[3]->direction == Edge::Direction::YMAX);
				REQUIRE(a.edges[3]->normal == Normal::XMAX);
			}
			THEN("For diagonal edges, there should not be any normal") {
				REQUIRE(a.edges[0]->direction == Edge::Direction::NONE);
				REQUIRE(a.edges[2]->direction == Edge::Direction::NONE);
				REQUIRE(a.edges[4]->direction == Edge::Direction::NONE);
				REQUIRE(a.edges[6]->direction == Edge::Direction::NONE);
				REQUIRE(a.edges[0]->normal == Normal::NONE);
				REQUIRE(a.edges[2]->normal == Normal::NONE);
				REQUIRE(a.edges[4]->normal == Normal::NONE);
				REQUIRE(a.edges[6]->normal == Normal::NONE);
			}
		}

		WHEN("Points are all aligned or at the same position") {
			Polygon a(XY, Polygon::Type::SHAPE, "", { 0, 0 }, from_init_list<Point>({{ 1, 2 }, { 1, 4 }, { 1, 1 }, { 1, 1 }}), &t);
			THEN("There should not be any normal") {
				REQUIRE(a.rotation == Polygon::Rotation::COLINEAR);
				REQUIRE(a.edges[0]->normal == Normal::NONE);
				REQUIRE(a.edges[1]->normal == Normal::NONE);
				REQUIRE(a.edges[2]->normal == Normal::NONE);
				REQUIRE(a.edges[3]->normal == Normal::NONE);
			}
		}
	}
}

//******************************************************************************
SCENARIO("relation::PolygonPoint Polygon::relation_to(Point const* point) const noexcept", "[polygon]") {
	Timepoint t;
	GIVEN("A simple polygon") {
		Polygon poly(XY, Polygon::Type::SHAPE, "", { 0, 0 }, from_init_list<Point>({{ 1, 1 }, { 1, 3 }, { 3, 3 }, { 3, 1 }}), &t);
		WHEN("A point is inside the polygon") {
			Point p(2, 2);
			THEN("Should be detected as IN") {
				REQUIRE(poly.relation_to(p) == relation::PolygonPoint::IN);
			}
		}

		WHEN("A point is outside the polygon") {
			Point p(0, 2);
			Point q(4, 2);
			Point r(2, 4);
			THEN("Should be detected as OUT") {
				REQUIRE(poly.relation_to(p) == relation::PolygonPoint::OUT);
				REQUIRE(poly.relation_to(q) == relation::PolygonPoint::OUT);
				REQUIRE(poly.relation_to(r) == relation::PolygonPoint::OUT);
			}
		}

		WHEN("A point is outside the polygon, but colinear to an edge") {
			Point p(1, 0);
			Point q(3, 0);
			Point r(4, 1);
			Point s(4, 3);
			Point t(3, 4);
			Point u(1, 4);
			Point v(0, 3);
			Point w(0, 1);
			THEN("Should be detected as OUT") {
				REQUIRE(poly.relation_to(p) == relation::PolygonPoint::OUT);
				REQUIRE(poly.relation_to(q) == relation::PolygonPoint::OUT);
				REQUIRE(poly.relation_to(r) == relation::PolygonPoint::OUT);
				REQUIRE(poly.relation_to(s) == relation::PolygonPoint::OUT);
				REQUIRE(poly.relation_to(t) == relation::PolygonPoint::OUT);
				REQUIRE(poly.relation_to(u) == relation::PolygonPoint::OUT);
				REQUIRE(poly.relation_to(v) == relation::PolygonPoint::OUT);
				REQUIRE(poly.relation_to(w) == relation::PolygonPoint::OUT);
			}
		}

		WHEN("A point is on an edge of the polygon") {
			Point p(1, 2);
			THEN("Should be detected as ON") {
				REQUIRE(poly.relation_to(p) == relation::PolygonPoint::ON);
			}
		}

		WHEN("A point is on a vertex of the polygon") {
			Point p(1, 1);
			THEN("Should be detected as ON") {
				REQUIRE(poly.relation_to(p) == relation::PolygonPoint::ON);
			}
		}
	}

	GIVEN("A polygon with vertices on the firsts rays") {
		WHEN("The polygon have 4 edge colinear to the 4 first rays") {
			Polygon poly(XY, Polygon::Type::SHAPE, "", { 0, 0 }, from_init_list<Point>({
				{ 3, 1 }, { 3, 2 }, { 4, 3 }, { 5, 3 },
				{ 3, 5 }, { 3, 4 }, { 2, 3 }, { 1, 3 }}), &t);
			Point p(3, 3);
			THEN("Should be detected as IN") {
				REQUIRE(poly.relation_to(p) == relation::PolygonPoint::IN);
			}
		}

		WHEN("The polygon have 4 angles on the 4 first rays") {
			Polygon poly(XY, Polygon::Type::SHAPE, "", { 0, 0 }, from_init_list<Point>({{ 2, 1 }, { 3, 2 }, { 2, 3 }, { 1, 2 }}), &t);
			Point p(2, 2);
			THEN("Should be detected as IN") {
				REQUIRE(poly.relation_to(p) == relation::PolygonPoint::IN);
			}
		}
	}

	GIVEN("A simple polygon that flirt with floating points calculus imprecision") {
		WHEN("A point is outside the polygon, but colinear to an edge") {
			Polygon poly(XY, Polygon::Type::SHAPE, "", { 0, 0 }, from_init_list<Point>({
				{ 120.0290, -44.8024 },
				{ 120.0290, -42.0164 },
				{ 140.2830, -42.0164 },
				{ 140.2830, -44.8024 }}), &t);
			Point p(51.3539, -44.8024);
			Point q(31.1000, -44.8024);
			THEN("Should be detected as OUT") {
				REQUIRE(poly.relation_to(p) == relation::PolygonPoint::OUT);
				REQUIRE(poly.relation_to(q) == relation::PolygonPoint::OUT);
			}
		}
	}
}

//******************************************************************************
SCENARIO("bool does_overlap(Polygon::RangeZ const& a, Polygon::RangeZ const& b) noexcept", "[polygon]") {
	GIVEN("Two Z ranges that do not overlap") {
		Polygon::RangeZ a { 1, 4 };
		Polygon::RangeZ b { 5, 6 };
		THEN("Should not be detected as overlapping") {
			REQUIRE_FALSE(does_overlap(a, b));
			REQUIRE_FALSE(does_overlap(b, a));
		}
	}

	GIVEN("Two Z ranges that overlap") {
		Polygon::RangeZ a { 1, 4 };
		Polygon::RangeZ b { 2, 5 };
		THEN("Should be detected as overlapping") {
			REQUIRE(does_overlap(a, b));
			REQUIRE(does_overlap(b, a));
		}
	}

	GIVEN("A Z range that is totally inside an other") {
		Polygon::RangeZ a { 1, 4 };
		Polygon::RangeZ b { 2, 3 };
		THEN("Should be detected as overlapping") {
			REQUIRE(does_overlap(a, b));
			REQUIRE(does_overlap(b, a));
		}
	}

	GIVEN("Two Z ranges that just touch each other") {
		Polygon::RangeZ a { 1, 4 };
		Polygon::RangeZ b { 4, 5 };
		THEN("Should be detected as overlapping") {
			REQUIRE(does_overlap(a, b));
			REQUIRE(does_overlap(b, a));
		}
	}
}
