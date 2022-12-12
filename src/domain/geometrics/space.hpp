///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "utils/enum_utils.hpp"

namespace domain {

//******************************************************************************
ARRAY_WITH_ITERABLE_ENUM_INDEX(AxisSpace, Axis, X, Y, Z)

//******************************************************************************
ARRAY_WITH_ITERABLE_ENUM_INDEX(PlaneSpace, Plane, YZ, ZX, XY)

} // namespace domain
