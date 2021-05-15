///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/bounding.hpp"

/// @test bool does_overlap(Bounding const& a, Bounding const& b)
///*****************************************************************************

//******************************************************************************
SCENARIO("bool does_overlap(Bounding const& a, Bounding const& b)", "[bounding]") {
	GIVEN("Two bounding boxes that do not overlap") {
		Bounding a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 2;
		a[YMAX] = 4;
		Bounding b;
		b[XMIN] = 5;
		b[XMAX] = 6;
		b[YMIN] = 1;
		b[YMAX] = 3;
		THEN("Should not be detected as overlapping") {
			REQUIRE_FALSE(does_overlap(a, b));
			REQUIRE_FALSE(does_overlap(b, a));
		}
	}

	GIVEN("Two bounding boxes that overlap") {
		Bounding a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 2;
		a[YMAX] = 4;
		Bounding b;
		b[XMIN] = 2;
		b[XMAX] = 5;
		b[YMIN] = 1;
		b[YMAX] = 3;
		THEN("Should be detected as overlapping") {
			REQUIRE(does_overlap(a, b));
			REQUIRE(does_overlap(b, a));
		}
	}

	GIVEN("A bounding box that is totally inside an other") {
		Bounding a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 2;
		a[YMAX] = 4;
		Bounding b;
		b[XMIN] = 2;
		b[XMAX] = 3;
		b[YMIN] = 2.5;
		b[YMAX] = 3.5;
		THEN("Should be detected as overlapping") {
			REQUIRE(does_overlap(a, b));
			REQUIRE(does_overlap(b, a));
		}
	}

	GIVEN("Two bounding boxes that just touch each other by a corner") {
		Bounding a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 2;
		a[YMAX] = 4;
		Bounding b;
		b[XMIN] = 4;
		b[XMAX] = 5;
		b[YMIN] = 4;
		b[YMAX] = 5;
		THEN("Should be detected as overlapping") {
			REQUIRE(does_overlap(a, b));
			REQUIRE(does_overlap(b, a));
		}
	}
}
