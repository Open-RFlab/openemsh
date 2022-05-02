///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <array>

#include "coord.hpp"

//******************************************************************************
using Bounding2D = std::array<Coord, 4>;
enum BoundingIndex { XMIN, XMAX, YMIN, YMAX };

//******************************************************************************
bool does_overlap(Bounding2D const& a, Bounding2D const& b) noexcept;
