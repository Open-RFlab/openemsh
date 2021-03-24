///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

// TODO use std::complex instead of Point?

//******************************************************************************
class Point {
public:
	double const x;
	double const y;

	Point(double const _x, double const _y);
	void print() const;
};

//******************************************************************************
Point operator-(Point const& a, Point const& b);

//******************************************************************************
Point operator+(Point const& a, Point const& b);

//******************************************************************************
bool operator==(Point const& a, Point const& b);

//******************************************************************************
template<typename T>
Point operator*(T const n, Point const& p) {
	return Point(p.x * n, p.y * n);
}

//******************************************************************************
template<typename T>
Point operator*(Point const& p, T const n) {
	return Point(p.x * n, p.y * n);
}
