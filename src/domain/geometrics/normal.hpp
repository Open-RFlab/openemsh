///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

namespace domain {

/// Normal vector to the edge "from the inner side to the outer side"
/// of the polygon.
///*****************************************************************************
enum class Normal {
	XMIN,
	XMAX,
	YMIN,
	YMAX,
	ZMIN,
	ZMAX,
	NONE
};

//******************************************************************************
Normal reverse(Normal normal) noexcept;

} // namespace domain
