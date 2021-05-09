///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/meshline_policy_manager.hpp"

/// @test GridAxis cast(MeshlinePolicy::Axis const a)
///*****************************************************************************

//******************************************************************************
SCENARIO("GridAxis cast(MeshlinePolicy::Axis const a)", "[meshline_policy_manager]") {
	GIVEN("A MeshLinePolicy::Axis enum class") {
		MeshlinePolicy::Axis a;
		WHEN("a == MeshlinePolicy::Axis::X") {
			a = MeshlinePolicy::Axis::X;
			GridAxis b = cast(a);
			THEN("Should return X") {
				REQUIRE(b == X);
			}
		}
		WHEN("a == MeshlinePolicy::Axis::Y") {
			a = MeshlinePolicy::Axis::Y;
			GridAxis b = cast(a);
			THEN("Should return Y") {
				REQUIRE(b == Y);
			}
		}
	}
}
