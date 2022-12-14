///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "domain/geometrics/coord.hpp"
#include "domain/geometrics/point.hpp"
#include "domain/geometrics/space.hpp"

//******************************************************************************
class Point3D {
public:
	domain::Coord x;
	domain::Coord y;
	domain::Coord z;

	Point3D(domain::Coord x, domain::Coord y, domain::Coord z) noexcept;
	domain::Point get_projection(domain::Plane plane) const;
};

//******************************************************************************
Point3D operator-(Point3D const& a, Point3D const& b) noexcept;

//******************************************************************************
Point3D operator+(Point3D const& a, Point3D const& b) noexcept;

//******************************************************************************
bool operator==(Point3D const& a, Point3D const& b) noexcept;

//******************************************************************************
//bool operator!=(Point3D const& a, Point3D const& b);

//******************************************************************************
template<typename T>
Point3D operator*(T const& n, Point3D const& p) noexcept {
	return Point3D(p.x * n, p.y * n, p.z * n);
}

//******************************************************************************
template<typename T>
Point3D operator*(Point3D const& p, T const& n) noexcept {
	return Point3D(p.x * n, p.y * n, p.z * n);
}

//******************************************************************************
Point3D mid(Point3D const& a, Point3D const& b) noexcept;
