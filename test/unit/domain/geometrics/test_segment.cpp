///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include "domain/geometrics/point.hpp"

#include "domain/geometrics/segment.hpp"

/// @test Segment::Axis axis(Point const& vector) noexcept
/// @test Segment::Axis axis(Point const& p0, Point const& p1) noexcept
/// @test std::optional<Axis> transpose(Plane const plane, Segment::Axis const axis) noexcept
///*****************************************************************************

using namespace domain;

//******************************************************************************
SCENARIO("Segment::Axis axis(Point const& vector)", "[segment]") {
	GIVEN("A point describing a vertical vector") {
		Point a(0, 2);
		Point b(0, -2);
		THEN("Axis should be Segment::Axis::V") {
			REQUIRE(axis(a) == Segment::Axis::V);
			REQUIRE(axis(b) == Segment::Axis::V);
		}
	}

	GIVEN("A point describing an horizontal vector") {
		Point a(2, 0);
		Point b(-2, 0);
		THEN("Axis should be Segment::Axis::H") {
			REQUIRE(axis(a) == Segment::Axis::H);
			REQUIRE(axis(b) == Segment::Axis::H);
		}
	}

	GIVEN("A point describing a point vector") {
		Point a(0, 0);
		THEN("Axis should be Segment::Axis::POINT") {
			REQUIRE(axis(a) == Segment::Axis::POINT);
		}
	}

	GIVEN("A point describing a diagonal vector") {
		Point a(2, 2);
		Point b(-2, 2);
		Point c(-2, -2);
		Point d(2, -2);
		THEN("Axis should be Segment::Axis::DIAGONAL") {
			REQUIRE(axis(a) == Segment::Axis::DIAGONAL);
			REQUIRE(axis(b) == Segment::Axis::DIAGONAL);
			REQUIRE(axis(c) == Segment::Axis::DIAGONAL);
			REQUIRE(axis(d) == Segment::Axis::DIAGONAL);
		}
	}
}

//******************************************************************************
SCENARIO("Segment::Axis axis(Point const& p0, Point const& p1)", "[segment]") {
	GIVEN("A vertical segment") {
		Point a0(0, 0), a1(0, 2);
		THEN("Axis should be Segment::Axis::V") {
			REQUIRE(axis(a0, a1) == Segment::Axis::V);
			REQUIRE(axis(a1, a0) == Segment::Axis::V);
		}
	}

	GIVEN("An horizontal segment") {
		Point a0(0, 0), a1(2, 0);
		THEN("Axis should be Segment::Axis::H") {
			REQUIRE(axis(a0, a1) == Segment::Axis::H);
			REQUIRE(axis(a1, a0) == Segment::Axis::H);
		}
	}

	GIVEN("A point segment") {
		Point a0(0, 0), a1(0, 0);
		THEN("Axis should be Segment::Axis::POINT") {
			REQUIRE(axis(a0, a1) == Segment::Axis::POINT);
			REQUIRE(axis(a1, a0) == Segment::Axis::POINT);
		}
	}

	GIVEN("A diagonal segment") {
		Point a0(0, 0), a1(2, 2);
		Point b0(0, 2), b1(2, 0);
		THEN("Axis should be Segment::Axis::DIAGONAL") {
			REQUIRE(axis(a0, a1) == Segment::Axis::DIAGONAL);
			REQUIRE(axis(a1, a0) == Segment::Axis::DIAGONAL);
			REQUIRE(axis(b0, b1) == Segment::Axis::DIAGONAL);
			REQUIRE(axis(b1, b0) == Segment::Axis::DIAGONAL);
		}
	}
}

//******************************************************************************
SCENARIO("std::optional<Axis> transpose(Plane const plane, Segment::Axis const axis) noexcept", "[segment]") {
	GIVEN("A Plane and a Segment::Axis") {
		WHEN("Axis is Segment::Axis::H") {
			AND_WHEN("Plane is YZ") {
				THEN("Should return Z") {
					std::optional<Axis> axis(transpose(YZ, Segment::Axis::H));
					REQUIRE(axis.has_value());
					REQUIRE(axis.value() == Z);
				}
			}

			AND_WHEN("Plane is ZX") {
				THEN("Should return X") {
					std::optional<Axis> axis(transpose(ZX, Segment::Axis::H));
					REQUIRE(axis.has_value());
					REQUIRE(axis.value() == X);
				}
			}

			AND_WHEN("Plane is XY") {
				THEN("Should return Y") {
					std::optional<Axis> axis(transpose(XY, Segment::Axis::H));
					REQUIRE(axis.has_value());
					REQUIRE(axis.value() == Y);
				}
			}
		}

		WHEN("Axis is Segment::Axis::V") {
			AND_WHEN("Plane is YZ") {
				THEN("Should return Y") {
					std::optional<Axis> axis(transpose(YZ, Segment::Axis::V));
					REQUIRE(axis.has_value());
					REQUIRE(axis.value() == Y);
				}
			}

			AND_WHEN("Plane is ZX") {
				THEN("Should return Z") {
					std::optional<Axis> axis(transpose(ZX, Segment::Axis::V));
					REQUIRE(axis.has_value());
					REQUIRE(axis.value() == Z);
				}
			}

			AND_WHEN("Plane is XY") {
				THEN("Should return X") {
					std::optional<Axis> axis(transpose(XY, Segment::Axis::V));
					REQUIRE(axis.has_value());
					REQUIRE(axis.value() == X);
				}
			}
		}

		WHEN("Axis is Segment::Axis::DIAGONAL") {
			THEN("Should not return a value") {
				std::optional<Axis> a(transpose(YZ, Segment::Axis::DIAGONAL));
				std::optional<Axis> b(transpose(ZX, Segment::Axis::DIAGONAL));
				std::optional<Axis> c(transpose(XY, Segment::Axis::DIAGONAL));
				REQUIRE_FALSE(a.has_value());
				REQUIRE_FALSE(b.has_value());
				REQUIRE_FALSE(c.has_value());
			}
		}

		WHEN("Axis is Segment::Axis::POINT") {
			THEN("Should not return a value") {
				std::optional<Axis> a(transpose(YZ, Segment::Axis::POINT));
				std::optional<Axis> b(transpose(ZX, Segment::Axis::POINT));
				std::optional<Axis> c(transpose(XY, Segment::Axis::POINT));
				REQUIRE_FALSE(a.has_value());
				REQUIRE_FALSE(b.has_value());
				REQUIRE_FALSE(c.has_value());
			}
		}
	}
}
