///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "i_conflict_origin.hpp"
#include "i_meshline_origin.hpp"

class Point;

//******************************************************************************
class Range : public IConflictOrigin, public IMeshLineOrigin {
public:
//	Point const* const p0;
//	Point const* const p1;

//	Range(Point const* _p0, Point const* _p1);
	enum class Axis {
		X,
		Y
	} axis;

	double beg;
	double end;

	Range(Axis _axis, double _beg, double _end);
	void print();
};