///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/point.hpp"
#include "domain/segment.hpp"

#include "domain/meshline_policy.hpp"

/// @test std::optional<MeshlinePolicy::Axis> cast(Segment::Axis const a)
/// @test double coord(Point const& point, MeshlinePolicy::Axis const axis)
///*****************************************************************************

SCENARIO("std::optional<MeshlinePolicy::Axis> cast(Segment::Axis const a)", "[meshline_policy]") {
	GIVEN("A Segment::Axis enum class") {
		WHEN("Segment::Axis::X") {
			THEN("Should return MeshlinePolicy::Axis::X") {
				std::optional<MeshlinePolicy::Axis> a(cast(Segment::Axis::X));
				REQUIRE(a.has_value());
				REQUIRE(a.value() == MeshlinePolicy::Axis::X);
			}
		}

		WHEN("Segment::Axis::Y") {
			THEN("Should return MeshlinePolicy::Axis::Y") {
				std::optional<MeshlinePolicy::Axis> a(cast(Segment::Axis::Y));
				REQUIRE(a.has_value());
				REQUIRE(a.value() == MeshlinePolicy::Axis::Y);
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

SCENARIO("double coord(Point const& point, MeshlinePolicy::Axis const axis)", "[meshline_policy]") {
	GIVEN("A point") {
		Point p(1, 2);
		THEN("Should select the correct coordinate") {
			REQUIRE(coord(p, MeshlinePolicy::Axis::X) == p.x);
			REQUIRE(coord(p, MeshlinePolicy::Axis::Y) == p.y);
		}
	}
}
