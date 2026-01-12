///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/geometrics/bounding.hpp"

/// @test bool does_overlap(Bounding1D const& a, Coord const& b) noexcept
/// @test bool does_overlap(Bounding1D const& a, Bounding1D const& b) noexcept
/// @test bool does_overlap(Bounding2D const& a, Bounding2D const& b) noexcept
/// @test bool does_overlap_strict(Bounding2D const& a, Bounding2D const& b) noexcept
/// @test Bounding1D cast(ViewAxis axis, Bounding2D const& a) noexcept
///*****************************************************************************

using namespace domain;

//******************************************************************************
SCENARIO("bool does_overlap(Bounding1D const& a, Coord const& b) noexcept", "[bounding]") {
	GIVEN("A bounding box") {
		Bounding1D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		WHEN("A coord is not in the bounding box") {
			Coord b(5);
			THEN("Should not be detected as overlapping") {
				REQUIRE_FALSE(does_overlap(a, b));
			}
		}
		WHEN("A coord is inside the bounding box") {
			Coord b(3);
			THEN("Should be detected as overlapping") {
				REQUIRE(does_overlap(a, b));
			}
		}
		WHEN("A coord is at the extremity of the bounding box") {
			Coord b(4);
			THEN("Should be detected as overlapping") {
				REQUIRE(does_overlap(a, b));
			}
		}
	}
}

//******************************************************************************
SCENARIO("bool does_overlap(Bounding1D const& a, Bounding1D const& b) noexcept", "[bounding]") {
	GIVEN("Two bounding boxes that do not overlap") {
		Bounding1D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		Bounding1D b;
		b[XMIN] = 5;
		b[XMAX] = 6;
		THEN("Should not be detected as overlapping") {
			REQUIRE_FALSE(does_overlap(a, b));
			REQUIRE_FALSE(does_overlap(b, a));
		}
	}

	GIVEN("Two bounding boxes that overlap") {
		Bounding1D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		Bounding1D b;
		b[XMIN] = 2;
		b[XMAX] = 5;
		THEN("Should be detected as overlapping") {
			REQUIRE(does_overlap(a, b));
			REQUIRE(does_overlap(b, a));
		}
	}

	GIVEN("A bounding box that is totally inside an other") {
		Bounding1D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		Bounding1D b;
		b[XMIN] = 2;
		b[XMAX] = 3;
		THEN("Should be detected as overlapping") {
			REQUIRE(does_overlap(a, b));
			REQUIRE(does_overlap(b, a));
		}
	}

	GIVEN("Two bounding boxes that just touch each other by an extremity") {
		Bounding1D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		Bounding1D b;
		b[XMIN] = 4;
		b[XMAX] = 5;
		THEN("Should be detected as overlapping") {
			REQUIRE(does_overlap(a, b));
			REQUIRE(does_overlap(b, a));
		}
	}
}

//******************************************************************************
SCENARIO("bool does_overlap(Bounding2D const& a, Bounding2D const& b) noexcept", "[bounding]") {
	GIVEN("Two bounding boxes that are strictly the same") {
		Bounding2D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 2;
		a[YMAX] = 4;
		Bounding2D b;
		b[XMIN] = 1;
		b[XMAX] = 4;
		b[YMIN] = 2;
		b[YMAX] = 4;
		THEN("Should be detected as overlapping") {
			REQUIRE(does_overlap(a, b));
			REQUIRE(does_overlap(b, a));
		}
	}

	GIVEN("Two bounding boxes that do not overlap") {
		Bounding2D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 2;
		a[YMAX] = 4;
		Bounding2D b;
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
		Bounding2D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 2;
		a[YMAX] = 4;
		Bounding2D b;
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
		Bounding2D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 2;
		a[YMAX] = 4;
		Bounding2D b;
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
		Bounding2D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 2;
		a[YMAX] = 4;
		Bounding2D b;
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

//******************************************************************************
SCENARIO("bool does_overlap_strict(Bounding2D const& a, Bounding2D const& b) noexcept", "[bounding]") {
	GIVEN("Two bounding boxes that are strictly the same") {
		Bounding2D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 2;
		a[YMAX] = 4;
		Bounding2D b;
		b[XMIN] = 1;
		b[XMAX] = 4;
		b[YMIN] = 2;
		b[YMAX] = 4;
		THEN("Should be detected as overlapping") {
			REQUIRE(does_overlap_strict(a, b));
			REQUIRE(does_overlap_strict(b, a));
		}
	}

	GIVEN("Two bounding boxes that do not overlap") {
		Bounding2D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 2;
		a[YMAX] = 4;
		Bounding2D b;
		b[XMIN] = 5;
		b[XMAX] = 6;
		b[YMIN] = 1;
		b[YMAX] = 3;
		THEN("Should not be detected as overlapping") {
			REQUIRE_FALSE(does_overlap_strict(a, b));
			REQUIRE_FALSE(does_overlap_strict(b, a));
		}
	}

	GIVEN("Two bounding boxes that overlap") {
		Bounding2D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 2;
		a[YMAX] = 4;
		Bounding2D b;
		b[XMIN] = 2;
		b[XMAX] = 5;
		b[YMIN] = 1;
		b[YMAX] = 3;
		THEN("Should be detected as overlapping") {
			REQUIRE(does_overlap_strict(a, b));
			REQUIRE(does_overlap_strict(b, a));
		}
	}

	GIVEN("A bounding box that is totally inside an other") {
		Bounding2D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 2;
		a[YMAX] = 4;
		Bounding2D b;
		b[XMIN] = 2;
		b[XMAX] = 3;
		b[YMIN] = 2.5;
		b[YMAX] = 3.5;
		THEN("Should be detected as overlapping") {
			REQUIRE(does_overlap_strict(a, b));
			REQUIRE(does_overlap_strict(b, a));
		}
	}

	GIVEN("Two bounding boxes that just touch each other by a corner") {
		Bounding2D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 2;
		a[YMAX] = 4;
		Bounding2D b;
		b[XMIN] = 4;
		b[XMAX] = 5;
		b[YMIN] = 4;
		b[YMAX] = 5;
		THEN("Should not be detected as overlapping") {
			REQUIRE_FALSE(does_overlap_strict(a, b));
			REQUIRE_FALSE(does_overlap_strict(b, a));
		}
	}
}

//******************************************************************************
SCENARIO("Bounding1D cast(ViewAxis axis, Bounding2D const& a) noexcept", "[bounding]") {
	GIVEN("A 2D Bounding box") {
		Bounding2D a;
		a[XMIN] = 1;
		a[XMAX] = 4;
		a[YMIN] = 3;
		a[YMAX] = 5;
		WHEN("Casting to 1D Bounding according to ViewAxis::H") {
			Bounding1D b = cast(ViewAxis::H, a);
			THEN("Should return the X part of the 2D Bounding box") {
				REQUIRE(b[XMIN] == a[XMIN]);
				REQUIRE(b[XMAX] == a[XMAX]);
			}
		}
		WHEN("Casting to 1D Bounding according to ViewAxis::V") {
			Bounding1D b = cast(ViewAxis::V, a);
			THEN("Should return the Y part of the 2D Bounding box") {
				REQUIRE(b[XMIN] == a[YMIN]);
				REQUIRE(b[XMAX] == a[YMAX]);
			}
		}
	}
}
