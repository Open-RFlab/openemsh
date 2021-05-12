///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

#include "range.hpp"

using namespace std;

//******************************************************************************
Range::Range(Point const p0, Point const p1)
: _p0(p0)
, _p1(p1) {
	if(p0 == p1) {
		axis = Axis::POINT;
	} else if(p0.x == p1.x) {
		axis = Axis::V;
	} else if(p0.y == p1.y) {
		axis = Axis::H;
	} else {
		axis = Axis::DIAGONAL;
	}
}

//******************************************************************************
Point const& Range::p0() const {
	return _p0;
}

//******************************************************************************
Point const& Range::p1() const {
	return _p1;
}

//******************************************************************************
bool operator==(Range const& a, Range const& b) {
	return (a.p0() == b.p0() && a.p1() == b.p1()) || (a.p0() == b.p1() && a.p1() == b.p0());
}

#ifdef DEBUG
//******************************************************************************
void Range::print() const {
	cout << "range:" << endl;
	p0().print();
	p1().print();
}
#endif // DEBUG
