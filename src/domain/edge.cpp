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

using namespace std;

//******************************************************************************
Edge::Edge(Point const* p0, Point const* p1)
: _p0(p0)
, _p1(p1)
, vec(*p1 - *p0)
, normal(Normal::NONE)
//, bounding({
//	(p0->x <= p1->x ? p0->x : p1->x),
//	(p0->x >= p1->x ? p1->x : p0->x),
//	(p0->y <= p1->y ? p0->y : p1->y),
//	(p0->y >= p1->y ? p1->y : p0->y) })
{
	if(vec.x == 0) {
		axis = Axis::Y;
		if(vec.y > 0)
			direction = Direction::YMAX;
		else if(vec.y < 0)
			direction = Direction::YMIN;
	} else if(vec.y == 0) {
		axis = Axis::X;
		if(vec.x > 0)
			direction = Direction::XMAX;
		else if(vec.x < 0)
			direction = Direction::XMIN;
	} else {
		axis = Axis::DIAGONAL;
		direction = Direction::DIAGONAL;
	}
}

//******************************************************************************
Point const& Edge::p0() const {
	return *_p0;
}

//******************************************************************************
Point const& Edge::p1() const {
	return *_p1;
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
	case Direction::DIAGONAL:
		cout << "X" << endl;
		break;
	}
}
#endif // DEBUG