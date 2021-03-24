///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include "range.hpp"

using namespace std;

//******************************************************************************
Range::Range(Point const _p0, Point const _p1)
: p0(_p0)
, p1(_p1)
, vec(p1 - p0) {
	if(vec.x == 0) {
		axis = Axis::Y;
	} else if(vec.y == 0) {
		axis = Axis::X;
	} else {
		axis = Axis::DIAGONAL;
	}
}

//******************************************************************************
bool operator==(Range const& a, Range const& b) {
	return (a.p0 == b.p0 && a.p1 == b.p1);
}
