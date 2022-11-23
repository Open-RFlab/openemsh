///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <catch2/catch_all.hpp>

#include "infra/utils/to_string.hpp"

/// @test std::string to_string(Edge::Direction const direction) noexcept
/// @test std::string to_string(Normal const normal) noexcept
/// @test std::string to_string(MeshlinePolicy::Normal const normal) noexcept
/// @test std::string to_string(MeshlinePolicy::Policy const policy) noexcept
/// @test std::string to_string(Polygon::Rotation const rotation) noexcept
/// @test std::string to_string(Segment::Axis const axis) noexcept
/// @test std::string to_string(Axis const axis) noexcept
/// @test std::string to_string(Plane const plane) noexcept
///*****************************************************************************

//******************************************************************************
SCENARIO("std::string to_string(Edge::Direction const direction) noexcept", "[to_string]") {
	REQUIRE(to_string(Edge::Direction::XMIN) == "XMIN");
	REQUIRE(to_string(Edge::Direction::XMAX) == "XMAX");
	REQUIRE(to_string(Edge::Direction::YMIN) == "YMIN");
	REQUIRE(to_string(Edge::Direction::YMAX) == "YMAX");
	REQUIRE(to_string(Edge::Direction::NONE) == "NONE");
}

//******************************************************************************
SCENARIO("std::string to_string(Normal const normal) noexcept", "[to_string]") {
	REQUIRE(to_string(Normal::XMIN) == "XMIN");
	REQUIRE(to_string(Normal::XMAX) == "XMAX");
	REQUIRE(to_string(Normal::YMIN) == "YMIN");
	REQUIRE(to_string(Normal::YMAX) == "YMAX");
	REQUIRE(to_string(Normal::ZMIN) == "ZMIN");
	REQUIRE(to_string(Normal::ZMAX) == "ZMAX");
	REQUIRE(to_string(Normal::NONE) == "NONE");
}

//******************************************************************************
SCENARIO("std::string to_string(MeshlinePolicy::Normal const normal) noexcept", "[to_string]") {
	REQUIRE(to_string(MeshlinePolicy::Normal::MIN) == "MIN");
	REQUIRE(to_string(MeshlinePolicy::Normal::MAX) == "MAX");
	REQUIRE(to_string(MeshlinePolicy::Normal::NONE) == "NONE");
}

//******************************************************************************
SCENARIO("std::string to_string(MeshlinePolicy::Policy const policy) noexcept", "[to_string]") {
	REQUIRE(to_string(MeshlinePolicy::Policy::ONELINE) == "ONELINE");
	REQUIRE(to_string(MeshlinePolicy::Policy::HALFS) == "HALFS");
	REQUIRE(to_string(MeshlinePolicy::Policy::THIRDS) == "THIRDS");
}

//******************************************************************************
SCENARIO("std::string to_string(Polygon::Rotation const rotation) noexcept", "[to_string]") {
	REQUIRE(to_string(Polygon::Rotation::CW) == "CW");
	REQUIRE(to_string(Polygon::Rotation::CCW) == "CCW");
	REQUIRE(to_string(Polygon::Rotation::COLINEAR) == "COLINEAR");
}

//******************************************************************************
SCENARIO("std::string to_string(Segment::Axis const axis) noexcept", "[to_string]") {
	REQUIRE(to_string(Segment::Axis::H) == "H");
	REQUIRE(to_string(Segment::Axis::V) == "V");
	REQUIRE(to_string(Segment::Axis::DIAGONAL) == "DIAGONAL");
	REQUIRE(to_string(Segment::Axis::POINT) == "POINT");
}

//******************************************************************************
SCENARIO("std::string to_string(Axis const axis) noexcept", "[to_string]") {
	REQUIRE(to_string(X) == "X");
	REQUIRE(to_string(Y) == "Y");
	REQUIRE(to_string(Z) == "Z");
}

//******************************************************************************
SCENARIO("std::string to_string(Plane const plane) noexcept", "[to_string]") {
	REQUIRE(to_string(YZ) == "YZ");
	REQUIRE(to_string(ZX) == "ZX");
	REQUIRE(to_string(XY) == "XY");
}
