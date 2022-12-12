///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "coord.hpp"

// TODO use std::complex instead of Point?

namespace domain {

//******************************************************************************
class Point {
public:
	Coord x;
	Coord y;

	Point(Coord x, Coord y) noexcept;

#ifdef DEBUG
	void print() const;
#endif // DEBUG
};

//******************************************************************************
Point operator-(Point const& a, Point const& b) noexcept;

//******************************************************************************
Point operator+(Point const& a, Point const& b) noexcept;

//******************************************************************************
bool operator==(Point const& a, Point const& b) noexcept;

//******************************************************************************
//bool operator!=(Point const& a, Point const& b);

//******************************************************************************
template<typename T>
Point operator*(T const& n, Point const& p) noexcept {
	return Point(p.x * n, p.y * n);
}

//******************************************************************************
template<typename T>
Point operator*(Point const& p, T const& n) noexcept {
	return Point(p.x * n, p.y * n);
}

//******************************************************************************
Point mid(Point const& a, Point const& b) noexcept;

} // namespace domain
