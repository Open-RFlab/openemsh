///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "i_conflict_origin.hpp"
#include "i_meshline_origin.hpp"
#include "point.hpp"

//class Point;

//******************************************************************************
class Range : public IConflictOrigin, public IMeshLineOrigin {
public:
	enum class Axis {
		X,
		Y,
		DIAGONAL
	} axis;

	Point const p0;
	Point const p1;
	Point const vec;

	Range(Point _p0, Point _p1);
};

//******************************************************************************
bool operator==(Range const& a, Range const& b);
