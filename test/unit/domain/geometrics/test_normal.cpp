///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/geometrics/normal.hpp"

/// @test Normal reverse(Normal normal) noexcept
///*****************************************************************************

using namespace domain;

//******************************************************************************
SCENARIO("Normal reverse(Normal normal) noexcept", "[domain][normal]") {
	WHEN("Reversing XMIN") {
		THEN("Should return XMAX") {
			REQUIRE(reverse(Normal::XMIN) == Normal::XMAX);
		}
	}
	WHEN("Reversing XMAX") {
		THEN("Should return XMIN") {
			REQUIRE(reverse(Normal::XMAX) == Normal::XMIN);
		}
	}
	WHEN("Reversing YMIN") {
		THEN("Should return YMAX") {
			REQUIRE(reverse(Normal::YMIN) == Normal::YMAX);
		}
	}
	WHEN("Reversing YMAX") {
		THEN("Should return YMIN") {
			REQUIRE(reverse(Normal::YMAX) == Normal::YMIN);
		}
	}
	WHEN("Reversing ZMIN") {
		THEN("Should return ZMAX") {
			REQUIRE(reverse(Normal::ZMIN) == Normal::ZMAX);
		}
	}
	WHEN("Reversing ZMAX") {
		THEN("Should return ZMIN") {
			REQUIRE(reverse(Normal::ZMAX) == Normal::ZMIN);
		}
	}
	WHEN("Reversing NONE") {
		THEN("Should return NONE") {
			REQUIRE(reverse(Normal::NONE) == Normal::NONE);
		}
	}
}
