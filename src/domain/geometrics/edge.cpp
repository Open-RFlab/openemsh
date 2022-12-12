///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

#include <algorithm>
//#include <array>

#include "point.hpp"
#include "polygon.hpp"
#include "range.hpp"

#include "edge.hpp"

namespace domain {

using namespace std;

//******************************************************************************
Edge::Edge(Plane const plane, Point const* p0, Point const* p1)
: Segment(domain::axis(*p0, *p1))
, IMeshLineOrigin()
, _p0(p0)
, _p1(p1)
, vec(*p1 - *p0)
, plane(plane)
, normal(Normal::NONE)
, to_mesh(true)
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

#ifdef DEBUG
//******************************************************************************
void Edge::print() const {
	cout << "x0: " << p0().x << "\ty0: " << p0().y << "\t\tx1: " << p1().x << "\ty1: " << p1().y << "\t\t";
	switch(direction) {
	case Direction::XMIN:
	case Direction::XMAX:
		cout << "_" << endl;
		break;
	case Direction::YMIN:
	case Direction::YMAX:
		cout << "|" << endl;
		break;
	case Direction::NONE:
		cout << "X" << endl;
		break;
	}
}
#endif // DEBUG

} // namespace domain
