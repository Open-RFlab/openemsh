///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/mesh/meshline_policy.hpp"

/// @test MeshlinePolicy::Normal cast(Normal const normal) noexcept
///*****************************************************************************

using namespace domain;

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
