///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "utils/unreachable.hpp"

#include "point_3d.hpp"

using namespace domain;
using namespace std;

//******************************************************************************
Point3D::Point3D(Coord x, Coord y, Coord z) noexcept
: x(x)
, y(y)
, z(z)
{}

//******************************************************************************
Point Point3D::get_projection(Plane plane) const {
	switch(plane) {
	case YZ: return { y, z };
	case ZX: return { z, x };
	case XY: return { x, y };
	default: ::unreachable();
	}
}

//******************************************************************************
Point3D operator-(Point3D const& a, Point3D const& b) noexcept {
	return Point3D(a.x - b.x, a.y - b.y, a.z - b.z);
}

//******************************************************************************
Point3D operator+(Point3D const& a, Point3D const& b) noexcept {
	return Point3D(a.x + b.x, a.y + b.y, a.z + b.z);
}

//******************************************************************************
bool operator==(Point3D const& a, Point3D const& b) noexcept {
	return (a.x == b.x && a.y == b.y && a.z == b.z);
}

//******************************************************************************
/*
bool operator!=(Point3D const& a, Point3D const& b) {
//	return (a.x != b.x || a.y != b.y);
	return (!are_equal(a.x, b.x) || !are_equal(a.y, b.y));
}
*/

//******************************************************************************
Point3D mid(Point3D const& a, Point3D const& b) noexcept {
	return Point3D(mid(a.x, b.x), mid(a.y, b.y), mid(a.z, b.z));
}
