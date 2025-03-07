///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "point.hpp"

namespace domain {

//******************************************************************************
Point::Point(Coord x, Coord y) noexcept
: x(x)
, y(y)
{}

//******************************************************************************
Point operator-(Point const& a, Point const& b) noexcept {
	return Point(a.x - b.x, a.y - b.y);
}

//******************************************************************************
Point operator+(Point const& a, Point const& b) noexcept {
	return Point(a.x + b.x, a.y + b.y);
}

//******************************************************************************
bool operator==(Point const& a, Point const& b) noexcept {
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
Point mid(Point const& a, Point const& b) noexcept {
	return Point(mid(a.x, b.x), mid(a.y, b.y));
}

} // namespace domain
