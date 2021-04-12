///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

struct Params {
	double metal_res;
	double substrate_res;
};

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

inline double equality_tolerance = 1e-8;
