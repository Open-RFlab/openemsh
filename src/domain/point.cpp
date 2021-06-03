///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

#include "point.hpp"

using namespace std;

//******************************************************************************
Point::Point(Coord x, Coord y)
: x(x)
, y(y)
{}

//******************************************************************************
Point operator-(Point const& a, Point const& b) {
	return Point(a.x - b.x, a.y - b.y);
}

//******************************************************************************
Point operator+(Point const& a, Point const& b) {
	return Point(a.x + b.x, a.y + b.y);
}

//******************************************************************************
bool operator==(Point const& a, Point const& b) {
	return (a.x == b.x && a.y == b.y);
}

//******************************************************************************
/*
bool operator!=(Point const& a, Point const& b) {
//	return (a.x != b.x || a.y != b.y);
	return (!are_equal(a.x, b.x) || !are_equal(a.y, b.y));
}
*/

//******************************************************************************
Point mid(Point const& a, Point const& b) {
	return Point((a.x + b.x) / 2, (a.y + b.y) / 2);
}

#ifdef DEBUG
//******************************************************************************
void Point::print() const {
	cout << "x: " << x << "\ty: " << y << endl;
}
#endif // DEBUG
