///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/geometrics/point.hpp"
#include "domain/geometrics/segment.hpp"

#include "domain/mesh/meshline_policy.hpp"

/// @test std::optional<Coord> coord(Point const& point, Segment::Axis const axis) noexcept
/// @test MeshlinePolicy::Normal cast(Normal const normal) noexcept
///*****************************************************************************

using namespace domain;

//******************************************************************************
SCENARIO("std::optional<Coord> coord(Point const& point, Segment::Axis const axis) noexcept", "[meshline_policy]") {
	GIVEN("A Point and a Segment::Axis enum class") {
		Point p(1, 3);
		WHEN("Segment::Axis::H") {
			THEN("Should return Y coord") {
				std::optional<Coord> a(coord(p, Segment::Axis::H));
				REQUIRE(a.has_value());
				REQUIRE(a.value() == p.y);
			}
		}

		WHEN("Segment::Axis::V") {
			THEN("Should return X coord") {
				std::optional<Coord> a(coord(p, Segment::Axis::V));
				REQUIRE(a.has_value());
				REQUIRE(a.value() == p.x);
			}
		}

		WHEN("Segment::Axis::DIAGONAL") {
			THEN("Should not return anything") {
				std::optional<Coord> a(coord(p, Segment::Axis::DIAGONAL));
				REQUIRE_FALSE(a.has_value());
			}
		}

		WHEN("Segment::Axis::POINT") {
			THEN("Should not return anything") {
				std::optional<Coord> a(coord(p, Segment::Axis::POINT));
				REQUIRE_FALSE(a.has_value());
			}
		}
	}
}

//******************************************************************************
SCENARIO("MeshlinePolicy::Normal cast(Normal const normal) noexcept", "[meshline_policy]") {
	GIVEN("A Normal enum class") {
		WHEN("Normal::XMIN") {
			THEN("Should return MeshlinePolicy::Normal::MIN") {
				REQUIRE(cast(Normal::XMIN) == MeshlinePolicy::Normal::MIN);
			}
		}

		WHEN("Normal::YMIN") {
			THEN("Should return MeshlinePolicy::Normal::MIN") {
				REQUIRE(cast(Normal::YMIN) == MeshlinePolicy::Normal::MIN);
			}
		}

		WHEN("Normal::ZMIN") {
			THEN("Should return MeshlinePolicy::Normal::MIN") {
				REQUIRE(cast(Normal::ZMIN) == MeshlinePolicy::Normal::MIN);
			}
		}

		WHEN("Normal::XMAX") {
			THEN("Should return MeshlinePolicy::Normal::MAX") {
				REQUIRE(cast(Normal::XMAX) == MeshlinePolicy::Normal::MAX);
			}
		}

		WHEN("Normal::YMAX") {
			THEN("Should return MeshlinePolicy::Normal::MAX") {
				REQUIRE(cast(Normal::YMAX) == MeshlinePolicy::Normal::MAX);
			}
		}

		WHEN("Normal::ZMAX") {
			THEN("Should return MeshlinePolicy::Normal::MAX") {
				REQUIRE(cast(Normal::ZMAX) == MeshlinePolicy::Normal::MAX);
			}
		}

		WHEN("Normal::NONE") {
			THEN("Should return MeshlinePolicy::Normal::NONE") {
				REQUIRE(cast(Normal::NONE) == MeshlinePolicy::Normal::NONE);
			}
		}
	}
}
