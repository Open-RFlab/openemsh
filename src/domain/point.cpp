///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include <cmath>

#include "global.hpp"

#include "point.hpp"

using namespace std;

//******************************************************************************
bool are_equal(double const a, double const b) {
	return (abs(a - b) < equality_tolerance);
}

//******************************************************************************
Point::Point(double _x, double _y)
: x(_x)
, y(_y)
{}

//******************************************************************************
void Point::print() const {
	cout << "x: " << x << "\ty: " << y << endl;
}

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
//	return (a.x == b.x && a.y == b.y);
	return (are_equal(a.x, b.x) && are_equal(a.y, b.y));
}

//******************************************************************************
bool operator!=(Point const& a, Point const& b) {
//	return (a.x != b.x || a.y != b.y);
	return (!are_equal(a.x, b.x) || !are_equal(a.y, b.y));
}

//******************************************************************************
Point mid(Point const& a, Point const& b) {
	return Point((a.x + b.x) / 2, (a.y + b.y) / 2);
}
