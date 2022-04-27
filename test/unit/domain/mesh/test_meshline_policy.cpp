///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/geometrics/point.hpp"
#include "domain/geometrics/segment.hpp"

#include "domain/mesh/meshline_policy.hpp"

/// @test std::optional<MeshlinePolicy::Axis> cast(Segment::Axis const a)
/// @test Coord coord(Point const& point, MeshlinePolicy::Axis const axis)
///*****************************************************************************

SCENARIO("std::optional<MeshlinePolicy::Axis> cast(Segment::Axis const a)", "[meshline_policy]") {
	GIVEN("A Segment::Axis enum class") {
		WHEN("Segment::Axis::H") {
			THEN("Should return MeshlinePolicy::Axis::H") {
				std::optional<MeshlinePolicy::Axis> a(cast(Segment::Axis::H));
				REQUIRE(a.has_value());
				REQUIRE(a.value() == MeshlinePolicy::Axis::H);
			}
		}

		WHEN("Segment::Axis::V") {
			THEN("Should return MeshlinePolicy::Axis::V") {
				std::optional<MeshlinePolicy::Axis> a(cast(Segment::Axis::V));
				REQUIRE(a.has_value());
				REQUIRE(a.value() == MeshlinePolicy::Axis::V);
			}
		}

		WHEN("Segment::Axis::DIAGONAL") {
			THEN("Should not return anything") {
				std::optional<MeshlinePolicy::Axis> a(cast(Segment::Axis::DIAGONAL));
				REQUIRE_FALSE(a.has_value());
			}
		}

		WHEN("Segment::Axis::POINT") {
			THEN("Should not return anything") {
				std::optional<MeshlinePolicy::Axis> a(cast(Segment::Axis::POINT));
				REQUIRE_FALSE(a.has_value());
			}
		}
	}
}

SCENARIO("Coord coord(Point const& point, MeshlinePolicy::Axis const axis)", "[meshline_policy]") {
	GIVEN("A point") {
		Point p(1, 2);
		THEN("Should select the correct coordinate") {
			REQUIRE(coord(p, MeshlinePolicy::Axis::H) == p.y);
			REQUIRE(coord(p, MeshlinePolicy::Axis::V) == p.x);
		}
	}
}
