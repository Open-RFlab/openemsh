///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/global.hpp"

#include "domain/point.hpp"

/// @test Point operator-(Point const& a, Point const& b)
/// @test Point operator+(Point const& a, Point const& b)
/// @test Point operator==(Point const& a, Point const& b)
/// @test template<typename T> Point operator*(T const n, Point const& p)
/// @test template<typename T> Point operator*(Point const& p, T const n)
///*****************************************************************************

//******************************************************************************
SCENARIO("Point operator-(Point const& a, Point const& b)", "[point]") {
	GIVEN("Two points") {
		Point a(5, 1);
		Point b(2, 3.5);

		WHEN("We soustract b to a (a-b)") {
			Point c(a - b);
			THEN("Each coord of the resulting point should be the soustraction of a and b equivalent coord") {
				REQUIRE(c.x == a.x - b.x);
				REQUIRE(c.y == a.y - b.y);
			}
		}
	}
}

//******************************************************************************
SCENARIO("Point operator+(Point const& a, Point const& b)", "[point]") {
	GIVEN("Two points") {
		Point a(5, 1);
		Point b(2, 3.5);

		WHEN("We add b to a (a+b)") {
			Point c(a + b);
			THEN("Each coord of the resulting point should be the addition of a and b equivalent coord") {
				REQUIRE(c.x == a.x + b.x);
				REQUIRE(c.y == a.y + b.y);
			}
		}
	}
}

//******************************************************************************
SCENARIO("bool operator==(Point const& a, Point const& b)", "[point]") {
	GIVEN("A point") {
		Point a(5, 1);

		WHEN("We compare it to an equivalent point") {
			Point b(5, 1);
			THEN("Should be equal") {
				REQUIRE(a == b);
			}
		}

		WHEN("We compare it to a different point") {
			Point b(1, 1);
			THEN("Should not be equal") {
				REQUIRE_FALSE(a == b);
			}
		}

		WHEN("We compare it to a point just below the equality tolerance") {
			Point b(5, 1 + equality_tolerance - equality_tolerance / 2);
			THEN("Should be equal") {
				REQUIRE(a == b);
			}
		}

		WHEN("We compare it to a point just after the equality tolerance") {
			Point b(5, 1 + equality_tolerance + equality_tolerance / 2);
			THEN("Should not be equal") {
				REQUIRE_FALSE(a == b);
			}
		}
	}
}

//******************************************************************************
SCENARIO("template<typename T> Point operator*(T const n, Point const& p)", "[point]") {
	GIVEN("A point and a number") {
		Point a(5, 1);
		double n = 3.5;

		WHEN("We multiply the point by the number (n * a)") {
			Point b(n * a);
			THEN("Each coord of the resulting point should be the multiplication of the a equivalent coord by n") {
				REQUIRE(b.x == a.x * n);
				REQUIRE(b.y == a.y * n);
			}
		}

		WHEN("We multiply the number by the point (a * n)") {
			Point b(a * n);
			THEN("Each coord of the resulting point should be the multiplication of the a equivalent coord by n") {
				REQUIRE(b.x == a.x * n);
				REQUIRE(b.y == a.y * n);
			}
		}
	}
}
