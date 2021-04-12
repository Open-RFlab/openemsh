///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "i_conflict_origin.hpp"
#include "i_meshline_origin.hpp"
#include "point.hpp"

//******************************************************************************
class Range : public IConflictOrigin, public IMeshLineOrigin {
public:
	enum class Axis {
		X,
		Y,
		DIAGONAL,
		POINT
	} axis;

	Point const p0;
	Point const p1;

	Range(Point const _p0, Point const _p1);
	void print() const;
};

//******************************************************************************
bool operator==(Range const& a, Range const& b);

//******************************************************************************
Point mid(Range const a);