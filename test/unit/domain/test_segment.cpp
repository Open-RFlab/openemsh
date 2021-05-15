///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/point.hpp"

#include "domain/segment.hpp"

/// @test Segment::Axis axis(Point const& vector)
/// @test Segment::Axis axis(Point const& p0, Point const& p1)
///*****************************************************************************

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
