///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <optional>

#include "utils/enum_utils.hpp"

namespace domain {

//******************************************************************************
ARRAY_WITH_ITERABLE_ENUM_INDEX(AxisSpace, Axis, X, Y, Z)

//******************************************************************************
ARRAY_WITH_ITERABLE_ENUM_INDEX(PlaneSpace, Plane, YZ, ZX, XY)

//******************************************************************************
ARRAY_WITH_ITERABLE_ENUM_INDEX(ViewAxisSpace, ViewAxis, H, V)

//******************************************************************************
inline static PlaneSpace<std::array<Axis const, 2>> constexpr Axes {
	Axis::Y, Axis::Z,
	Axis::Z, Axis::X,
	Axis::X, Axis::Y
};

/// ViewAxis describe the orientation of Axis itself.
///*****************************************************************************
std::optional<ViewAxis> transpose(Plane const plane, Axis const axis) noexcept;

/// ViewAxis describe the orientation of Axis itself.
///*****************************************************************************
Axis transpose(Plane const plane, ViewAxis const axis) noexcept;

//******************************************************************************
ViewAxis reverse(ViewAxis const axis) noexcept;

} // namespace domain
