///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <array>

//******************************************************************************
using Bounding = std::array<double, 4>;
enum BoundingIndex { XMIN, XMAX, YMIN, YMAX };

/// Normal vector to the edge "from the inner side to the outer side"
/// of the polygon.
///*****************************************************************************
enum class Normal {
	XMIN,
	XMAX,
	YMIN,
	YMAX,
	NONE
};
