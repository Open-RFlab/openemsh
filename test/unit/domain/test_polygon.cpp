///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/edge.hpp"
#include "domain/point.hpp"

#include "domain/polygon.hpp"

SCENARIO("bool are_possibly_overlapping(Polygon const& a, Polygon const& b)", "[polygon]") {
	GIVEN("Two polygons") {
		WHEN("The bounding boxes do not overlap") {
			Polygon a(Polygon::Rotation::UNKNOWN, {{ 1, 2 }, { 1, 4 }, { 4, 4 }});
			Polygon b(Polygon::Rotation::UNKNOWN, {{ 5, 3 }, { 5, 1 }, { 6, 2 }});
			THEN("Polygons should not possibly overlap") {
				REQUIRE_FALSE(are_possibly_overlapping(a, b));
				REQUIRE_FALSE(are_possibly_overlapping(b, a));
			}
		}

		WHEN("The bounding boxes overlap but not the polygons") {
			Polygon a(Polygon::Rotation::UNKNOWN, {{ 1, 2 }, { 1, 4 }, { 4, 4 }});
			Polygon b(Polygon::Rotation::UNKNOWN, {{ 3, 3 }, { 5, 3 }, { 4, 1 }});
			THEN("Polygons should possibly overlap") {
				REQUIRE(are_possibly_overlapping(a, b));
				REQUIRE(are_possibly_overlapping(b, a));
			}
		}

		WHEN("The bounding boxes and the polygons overlap") {
			Polygon a(Polygon::Rotation::UNKNOWN, {{ 1, 2 }, { 1, 4 }, { 4, 4 }});
			Polygon b(Polygon::Rotation::UNKNOWN, {{ 2, 1 }, { 2, 3 }, { 5, 3 }});
			THEN("Polygons should possibly overlap") {
				REQUIRE(are_possibly_overlapping(a, b));
				REQUIRE(are_possibly_overlapping(b, a));
			}
		}


		WHEN("The bounding boxes of a polygons is totally inside the other") {
			Polygon a(Polygon::Rotation::UNKNOWN, {{ 1, 2 }, { 1, 4 }, { 4, 4 }});
			Polygon b(Polygon::Rotation::UNKNOWN, {{ 2, 2.5 }, { 2, 3.5 }, { 3, 3 }});
			THEN("Polygons should possibly overlap") {
				REQUIRE(are_possibly_overlapping(a, b));
				REQUIRE(are_possibly_overlapping(b, a));
			}
		}

		WHEN("The bounding boxes just touch each other by a corner") {
			Polygon a(Polygon::Rotation::UNKNOWN, {{ 1, 2 }, { 1, 4 }, { 4, 4 }});
			Polygon b(Polygon::Rotation::UNKNOWN, {{ 4, 4 }, { 5, 4 }, { 5, 5 }});
			THEN("Polygons should possibly overlap") {
				REQUIRE(are_possibly_overlapping(a, b));
				REQUIRE(are_possibly_overlapping(b, a));
			}
		}
	}
}

SCENARIO("Polygon::Rotation detect_rotation(T& points)", "[polygon]") {
	GIVEN("A polygon or a bunch of points") {
		WHEN("Points order is oriented clockwise") {
			Polygon a(Polygon::Rotation::UNKNOWN, {{ 1, 2 }, { 1, 4 }, { 4, 4 }});
			std::vector<Point const*> b({a.points[0].get(), a.points[1].get(), a.points[2].get()});
			THEN("Should be detected as CW") {
				REQUIRE(detect_rotation(a.points) == Polygon::Rotation::CW);
				REQUIRE(detect_rotation(b) == Polygon::Rotation::CW);
			}
		}

		WHEN("Points order is oriented counter clockwise") {
			Polygon a(Polygon::Rotation::UNKNOWN, {{ 1, 2 }, { 4, 4 }, { 1, 4 }});
			std::vector<Point const*> b({a.points[0].get(), a.points[1].get(), a.points[2].get()});
			THEN("Should be detected as CCW") {
				REQUIRE(detect_rotation(a.points) == Polygon::Rotation::CCW);
				REQUIRE(detect_rotation(b) == Polygon::Rotation::CCW);
			}
		}

		WHEN("Points are all aligned in an axis") {
			Polygon a(Polygon::Rotation::UNKNOWN, {{ 1, 2 }, { 1, 4 }, { 1, 1 }});
			std::vector<Point const*> b({a.points[0].get(), a.points[1].get(), a.points[2].get()});
			THEN("Should be detected as COLINEAR") {
				REQUIRE(detect_rotation(a.points) == Polygon::Rotation::COLINEAR);
				REQUIRE(detect_rotation(b) == Polygon::Rotation::COLINEAR);
			}
		}

		WHEN("Points are all aligned in diagonal") {
			Polygon a(Polygon::Rotation::UNKNOWN, {{ 1, 2 }, { 4, 4 }, { 2.5, 3 }});
			std::vector<Point const*> b({a.points[0].get(), a.points[1].get(), a.points[2].get()});
			THEN("Should be detected as COLINEAR") {
				REQUIRE(detect_rotation(a.points) == Polygon::Rotation::COLINEAR);
				REQUIRE(detect_rotation(b) == Polygon::Rotation::COLINEAR);
			}
		}
	}
}
