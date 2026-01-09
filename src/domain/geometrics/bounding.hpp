///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <array>

#include "space.hpp"
#include "coord.hpp"

namespace domain {

//******************************************************************************
using Bounding1D = std::array<Coord, 2>;
using Bounding2D = std::array<Coord, 4>;
enum BoundingIndex { XMIN, XMAX, YMIN, YMAX };

//******************************************************************************
bool does_overlap(Bounding1D const& a, Coord const& b) noexcept;

//******************************************************************************
bool does_overlap(Bounding1D const& a, Bounding1D const& b) noexcept;

//******************************************************************************
bool does_overlap(Bounding2D const& a, Bounding2D const& b) noexcept;

//******************************************************************************
Bounding1D cast(ViewAxis axis, Bounding2D const& a) noexcept;

} // namespace domain
