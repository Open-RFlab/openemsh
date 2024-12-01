///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "domain/geometrics/space.hpp"

/// @test std::optional<ViewAxis> transpose(Plane const plane, Axis const axis) noexcept
/// @test Axis transpose(Plane const plane, ViewAxis const axis) noexcept
/// @test ViewAxis reverse(ViewAxis const axis) noexcept
///*****************************************************************************

using namespace domain;

//******************************************************************************
SCENARIO("std::optional<ViewAxis> transpose(Plane const plane, Axis const axis) noexcept", "[space]") {
	GIVEN("A Plane and an Axis") {
		WHEN("Plane is YZ") {
			AND_WHEN("Axis is X") {
				THEN("Should not return a value") {
					std::optional<ViewAxis> axis(transpose(YZ, X));
					REQUIRE_FALSE(axis.has_value());
				}
			}
			AND_WHEN("Axis is Y") {
				THEN("Should return H") {
					std::optional<ViewAxis> axis(transpose(YZ, Y));
					REQUIRE(axis.has_value());
					REQUIRE(axis.value() == H);
				}
			}
			AND_WHEN("Axis is Z") {
				THEN("Should return V") {
					std::optional<ViewAxis> axis(transpose(YZ, Z));
					REQUIRE(axis.has_value());
					REQUIRE(axis.value() == V);
				}
			}
		}

		WHEN("Plane is ZX") {
			AND_WHEN("Axis is X") {
				THEN("Should return V") {
					std::optional<ViewAxis> axis(transpose(ZX, X));
					REQUIRE(axis.has_value());
					REQUIRE(axis.value() == V);
				}
			}
			AND_WHEN("Axis is Y") {
				THEN("Should not return a value") {
					std::optional<ViewAxis> axis(transpose(ZX, Y));
					REQUIRE_FALSE(axis.has_value());
				}
			}
			AND_WHEN("Axis is Z") {
				THEN("Should return H") {
					std::optional<ViewAxis> axis(transpose(ZX, Z));
					REQUIRE(axis.has_value());
					REQUIRE(axis.value() == H);
				}
			}
		}

		WHEN("Plane is XY") {
			AND_WHEN("Axis is X") {
				THEN("Should return H") {
					std::optional<ViewAxis> axis(transpose(XY, X));
					REQUIRE(axis.has_value());
					REQUIRE(axis.value() == H);
				}
			}
			AND_WHEN("Axis is Y") {
				THEN("Should return V") {
					std::optional<ViewAxis> axis(transpose(XY, Y));
					REQUIRE(axis.has_value());
					REQUIRE(axis.value() == V);
				}
			}
			AND_WHEN("Axis is Z") {
				THEN("Should not return a value") {
					std::optional<ViewAxis> axis(transpose(XY, Z));
					REQUIRE_FALSE(axis.has_value());
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("Axis transpose(Plane const plane, ViewAxis const axis) noexcept", "[space]") {
	GIVEN("A Plane and a ViewAxis") {
		WHEN("Plane is YZ") {
			AND_WHEN("ViewAxis is H") {
				THEN("Should return Y") {
					REQUIRE(transpose(YZ, H) == Y);
				}
			}
			AND_WHEN("ViewAxis is V") {
				THEN("Should return Z") {
					REQUIRE(transpose(YZ, V) == Z);
				}
			}
		}

		WHEN("Plane is ZX") {
			AND_WHEN("ViewAxis is H") {
				THEN("Should return Z") {
					REQUIRE(transpose(ZX, H) == Z);
				}
			}
			AND_WHEN("ViewAxis is V") {
				THEN("Should return X") {
					REQUIRE(transpose(ZX, V) == X);
				}
			}
		}

		WHEN("Plane is XY") {
			AND_WHEN("ViewAxis is H") {
				THEN("Should return X") {
					REQUIRE(transpose(XY, H) == X);
				}
			}
			AND_WHEN("ViewAxis is V") {
				THEN("Should return Y") {
					REQUIRE(transpose(XY, V) == Y);
				}
			}
		}
	}
}

//******************************************************************************
SCENARIO("ViewAxis reverse(ViewAxis const axis) noexcept", "[space]") {
	GIVEN("A ViewAxis") {
		WHEN("ViewAxis is H") {
			THEN("Should return V") {
				REQUIRE(reverse(H) == V);
			}
		}
		WHEN("ViewAxis is V") {
			THEN("Should return H") {
				REQUIRE(reverse(V) == H);
			}
		}
	}
}
