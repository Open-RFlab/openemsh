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
, p1(_p1) {
	if(p0 == p1) {
		axis = Axis::POINT;
	} else if(p0.x == p1.x) {
		axis = Axis::Y;
	} else if(p0.y == p1.y) {
		axis = Axis::X;
	} else {
		axis = Axis::DIAGONAL;
	}
}

//******************************************************************************
void Range::print() const {
	cout << "range:" << endl;
	p0.print();
	p1.print();
}

//******************************************************************************
bool operator==(Range const& a, Range const& b) {
	return (a.p0 == b.p0 && a.p1 == b.p1) || (a.p0 == b.p1 && a.p1 == b.p0);
}

//******************************************************************************
Point mid(Range const a) {
	return mid(a.p0, a.p1);
}