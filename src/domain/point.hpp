///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

//******************************************************************************
class Point {
public:
	double x;
	double y;

	Point(double _x, double _y);
	void print() const;
};

//******************************************************************************
Point operator-(Point const& a, Point const& b);