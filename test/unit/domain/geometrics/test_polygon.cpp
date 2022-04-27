///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/point.hpp"

#include "domain/geometrics/polygon.hpp"

/// @test void Polygon::detect_bounding()
/// @test template<class T> Polygon::Rotation detect_rotation(T& points)
/// @test void Polygon::detect_edge_normal()
/// @test relation::PolygonPoint Polygon::relation_to(Point const* point) const
///*****************************************************************************

//******************************************************************************
SCENARIO("void Polygon::detect_bounding()", "[polygon]") {
	GIVEN("A random polygon") {
		Polygon a({{ 1, 2 }, { 1, 4 }, { 4, 4 }});
		THEN("Should calcul bounding box") {
			REQUIRE(a.bounding[XMIN] == 1);
			REQUIRE(a.bounding[XMAX] == 4);
			REQUIRE(a.bounding[YMIN] == 2);
			REQUIRE(a.bounding[YMAX] == 4);
		}
	}
}

//******************************************************************************
SCENARIO("template<class T> Polygon::Rotation detect_rotation(T& points)", "[polygon]") {
	GIVEN("A polygon or a bunch of points") {
		WHEN("Points order is oriented clockwise") {
			Polygon a({{ 1, 2 }, { 1, 4 }, { 4, 4 }});
			std::vector<Point const*> b({a.points[0].get(), a.points[1].get(), a.points[2].get()});
			THEN("Should be detected as CW") {
				REQUIRE(detect_rotation(a.points) == Polygon::Rotation::CW);
				REQUIRE(detect_rotation(b) == Polygon::Rotation::CW);
			}
		}

		WHEN("Points order is oriented counter clockwise") {
			Polygon a({{ 1, 2 }, { 4, 4 }, { 1, 4 }});
			std::vector<Point const*> b({a.points[0].get(), a.points[1].get(), a.points[2].get()});
			THEN("Should be detected as CCW") {
				REQUIRE(detect_rotation(a.points) == Polygon::Rotation::CCW);
				REQUIRE(detect_rotation(b) == Polygon::Rotation::CCW);
			}
		}

		WHEN("Points are all aligned in an axis") {
			Polygon a({{ 1, 2 }, { 1, 4 }, { 1, 1 }});
			std::vector<Point const*> b({a.points[0].get(), a.points[1].get(), a.points[2].get()});
			THEN("Should be detected as COLINEAR") {
				REQUIRE(detect_rotation(a.points) == Polygon::Rotation::COLINEAR);
				REQUIRE(detect_rotation(b) == Polygon::Rotation::COLINEAR);
			}
		}

		WHEN("Points are all aligned in diagonal") {
			Polygon a({{ 1, 2 }, { 4, 4 }, { 2.5, 3 }});
			std::vector<Point const*> b({a.points[0].get(), a.points[1].get(), a.points[2].get()});
			THEN("Should be detected as COLINEAR") {
				REQUIRE(detect_rotation(a.points) == Polygon::Rotation::COLINEAR);
				REQUIRE(detect_rotation(b) == Polygon::Rotation::COLINEAR);
			}
		}

		WHEN("Two points of a triangle are at the same place") {
			Polygon a({{ 1, 2 }, { 4, 4 }, { 1, 2 }});
			std::vector<Point const*> b({a.points[0].get(), a.points[1].get(), a.points[2].get()});
			THEN("Should be detected as COLINEAR") {
				REQUIRE(detect_rotation(a.points) == Polygon::Rotation::COLINEAR);
				REQUIRE(detect_rotation(b) == Polygon::Rotation::COLINEAR);
			}
		}
	}
}

//******************************************************************************
SCENARIO("void Polygon::detect_edge_normal()", "[polygon]") {
	GIVEN("An octogon : 2 horizontal edges, 2 verticals, 4 diagonals") {
		WHEN("Points order is oriented clockwise") {
			Polygon a({
				{ 2, 1 }, { 1, 2 }, { 1, 3 }, { 2, 4 },
				{ 3, 4 }, { 4, 3 }, { 4, 2 }, { 3, 1 }});
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
			Polygon a({
				{ 2, 1 }, { 3, 1 }, { 4, 2 }, { 4, 3 },
				{ 3, 4 }, { 2, 4 }, { 1, 3 }, { 1, 2 }});
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
			Polygon a({{ 1, 2 }, { 1, 4 }, { 1, 1 }, { 1, 1 }});
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
SCENARIO("relation::PolygonPoint Polygon::relation_to(Point const* point) const", "[polygon]") {
	GIVEN("A simple polygon") {
		Polygon poly({{ 1, 1 }, { 1, 3 }, { 3, 3 }, { 3, 1 }});
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
			Polygon poly({
				{ 3, 1 }, { 3, 2 }, { 4, 3 }, { 5, 3 },
				{ 3, 5 }, { 3, 4 }, { 2, 3 }, { 1, 3 }});
			Point p(3, 3);
			THEN("Should be detected as IN") {
				REQUIRE(poly.relation_to(p) == relation::PolygonPoint::IN);
			}
		}

		WHEN("The polygon have 4 angles on the 4 first rays") {
			Polygon poly({{ 2, 1 }, { 3, 2 }, { 2, 3 }, { 1, 2 }});
			Point p(2, 2);
			THEN("Should be detected as IN") {
				REQUIRE(poly.relation_to(p) == relation::PolygonPoint::IN);
			}
		}
	}
}
