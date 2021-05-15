///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/global.hpp"

#include "domain/coord.hpp"

/// @test Coord::operator double() const
/// @test bool Coord::operator==(Coord const& a) const
///*****************************************************************************

//******************************************************************************
SCENARIO("Coord::operator double() const", "[coord]") {
	GIVEN("A coord") {
		Coord a(5.5);
		Coord b(-5.5);
		THEN("A (double) cast should return the inner value") {
			REQUIRE((double) a == a.value);
			REQUIRE((double) b == b.value);
		}
	}
}

//******************************************************************************
SCENARIO("bool operator==(Coord const& a) const", "[coord]") {
	GIVEN("A coord") {
		Coord a(5.5);
		WHEN("We compare it to an equivalent coord") {
			Coord b(5.5);
			double c = 5.5;
			THEN("Should be equal") {
				REQUIRE(a == b);
				REQUIRE(a == c);
				REQUIRE(c == a);
			}
		}

		WHEN("We compare it to a different coord") {
			Coord b(1);
			double c = 1;
			THEN("Should not be equal") {
				REQUIRE_FALSE(a == b);
				REQUIRE_FALSE(a == c);
				REQUIRE_FALSE(c == a);
			}
		}

		WHEN("We compare it to a coord just below the equality tolerance") {
			Coord b(5.5 + equality_tolerance - equality_tolerance / 2);
			double c = 5.5 + equality_tolerance - equality_tolerance / 2;
			THEN("Should be equal") {
				REQUIRE(a == b);
				REQUIRE(a == c);
				REQUIRE(c == a);
			}
		}

		WHEN("We compare it to a coord just after the equality tolerance") {
			Coord b(5.5 + equality_tolerance + equality_tolerance / 2);
			double c = 5.5 + equality_tolerance + equality_tolerance / 2;
			THEN("Should not be equal") {
				REQUIRE_FALSE(a == b);
				REQUIRE_FALSE(a == c);
				REQUIRE_FALSE(c == a);
			}
		}
	}
}
