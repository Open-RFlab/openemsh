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
/// @test std::optional<ViewAxis> cast(Segment::Axis const axis) noexcept
/// @test Segment::Axis cast(ViewAxis const axis) noexcept
/// @test std::optional<Coord> coord(Point const& point, Segment::Axis const axis) noexcept
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

//******************************************************************************
SCENARIO("std::optional<ViewAxis> cast(Segment::Axis const axis) noexcept", "[segment]") {
	GIVEN("A Segment::Axis enum class") {
		WHEN("Segment::Axis::H") {
			THEN("Should return ViewAxis::H") {
				std::optional<ViewAxis> a(cast(Segment::Axis::H));
				REQUIRE(a.has_value());
				REQUIRE(a.value() == ViewAxis::H);
			}
		}
		WHEN("Segment::Axis::V") {
			THEN("Should return ViewAxis::V") {
				std::optional<ViewAxis> a(cast(Segment::Axis::V));
				REQUIRE(a.has_value());
				REQUIRE(a.value() == ViewAxis::V);
			}
		}
		WHEN("Segment::Axis::DIAGONAL") {
			THEN("Should not return anything") {
				std::optional<ViewAxis> a(cast(Segment::Axis::DIAGONAL));
				REQUIRE_FALSE(a.has_value());
			}
		}
		WHEN("Segment::Axis::H") {
			THEN("Should return ViewAxis::H") {
				std::optional<ViewAxis> a(cast(Segment::Axis::POINT));
				REQUIRE_FALSE(a.has_value());
			}
		}
	}
}

//******************************************************************************
SCENARIO("Segment::Axis cast(ViewAxis const axis) noexcept", "[segment]") {
	GIVEN("A ViewAxis enum") {
		WHEN("ViewAxis::H") {
			THEN("Should return Segment::Axis::H") {
				REQUIRE(cast(ViewAxis::H) == Segment::Axis::H);
			}
		}
		WHEN("ViewAxis::V") {
			THEN("Should return Segment::Axis::V") {
				REQUIRE(cast(ViewAxis::V) == Segment::Axis::V);
			}
		}
	}
}

//******************************************************************************
SCENARIO("std::optional<Coord> coord(Point const& point, Segment::Axis const axis) noexcept", "[segment]") {
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
