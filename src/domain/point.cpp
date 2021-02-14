///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include "point.hpp"

using namespace std;

//******************************************************************************
Point::Point(double const _x, double const _y)
: x(_x)
, y(_y)
{}

//******************************************************************************
void Point::print() const {
	cout << "x: " << x << "\ty: " << y << endl;
}

//******************************************************************************
Point operator-(Point const& a, Point const& b) {
	return move(Point(a.x - b.x, a.y - b.y));
}
