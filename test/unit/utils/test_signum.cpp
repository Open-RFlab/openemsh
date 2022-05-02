///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/geometrics/coord.hpp"

#include "utils/signum.hpp"

/// @test template <typename T> int8_t signum(T const& a)
///*****************************************************************************

//******************************************************************************
SCENARIO("template <typename T> int8_t signum(T const& a)", "[utils][signum]") {
	GIVEN("A double number") {
		WHEN("Positive") {
			double a = 2;
			THEN("Should return 1") {
				REQUIRE(signum(a) == 1);
			}
		}

		WHEN("Zero") {
			double a = 0;
			THEN("Should return 0") {
				REQUIRE(signum(a) == 0);
			}
		}

		WHEN("Negative") {
			double a = -2;
			THEN("Should return -1") {
				REQUIRE(signum(a) == -1);
			}
		}
	}

	GIVEN("A Coord number") {
		WHEN("Positive") {
			Coord a(2);
			THEN("Should return 1") {
				REQUIRE(signum(a) == 1);
			}
		}

		WHEN("Zero") {
			Coord a(0);
			THEN("Should return 0") {
				REQUIRE(signum(a) == 0);
			}
		}

		WHEN("Negative") {
			Coord a(-2);
			THEN("Should return -1") {
				REQUIRE(signum(a) == -1);
			}
		}
	}
}
