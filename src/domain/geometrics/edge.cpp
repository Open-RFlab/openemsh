///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <algorithm>
#include <cmath>
#include <numbers>

#include "point.hpp"
#include "polygon.hpp"
#include "range.hpp"

#include "edge.hpp"

namespace domain {

using namespace std;

//******************************************************************************
Edge::Edge(Plane const plane, Point const* p0, Point const* p1, Timepoint* t)
: Originator(t)
, Segment(domain::axis(*p0, *p1))
, IMeshLineOrigin()
, _p0(p0)
, _p1(p1)
, vec(*p1 - *p0)
, plane(plane)
, normal(Normal::NONE)
//, bounding(bounding(*this))
{
	if(axis == Axis::H) {
		if(vec.x > 0)
			direction = Direction::XMAX;
		else if(vec.x < 0)
			direction = Direction::XMIN;
	} else if(axis == Axis::V) {
		if(vec.y > 0)
			direction = Direction::YMAX;
		else if(vec.y < 0)
			direction = Direction::YMIN;
	} else {
		direction = Direction::NONE;
	}
}

//******************************************************************************
Point const& Edge::p0() const noexcept {
	return *_p0;
}

//******************************************************************************
Point const& Edge::p1() const noexcept {
	return *_p1;
}

//******************************************************************************
bool operator==(Range const& a, Edge const& b) noexcept {
	return (b == a);
}

//******************************************************************************
bool operator==(Edge const& a, Range const& b) noexcept {
	return ((a.p0() == b.p0() && a.p1() == b.p1())
	    ||  (a.p0() == b.p1() && a.p1() == b.p0()));
}

// Returns the acute side of the angle.
//******************************************************************************
Coord angle(Point const& vec_a, Point const& vec_b) noexcept {
	// Cross and dot products.
	auto cross = double (vec_a.x * vec_b.y - vec_a.y * vec_b.x);
	auto dot   = double (vec_a.x * vec_b.x + vec_a.y * vec_b.y);

	// In radians between [-pi, +pi]
	double angle_rad = atan2(fabs(cross), dot);

	// Smaller angle in degree between [0, 180]
	double angle_deg = fabs(angle_rad) * 180.0 / numbers::pi;

	if (angle_deg > 180.0)
		angle_deg = 360.0 - angle_deg;

	return angle_deg;
}

} // namespace domain
