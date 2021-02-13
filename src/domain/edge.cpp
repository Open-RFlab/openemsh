///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include "edge.hpp"

using namespace std;

//******************************************************************************
Edge::Edge(Point const* _p0, Point const* _p1, Status _status)
: p0(_p0)
, p1(_p1)
, vec(make_unique<Point>(*p1 - *p0))
, status(_status)
, res_factor(1) {
	if(vec->x == 0) {
		if(vec->y > 0)
			direction = Direction::YMAX;
		else if(vec->y < 0)
			direction = Direction::YMIN;
	} else if(vec->y == 0) {
		if(vec->x > 0)
			direction = Direction::XMAX;
		else if(vec->x < 0)
			direction = Direction::XMIN;
	} else {
		direction = Direction::DIAGONAL;
	}
}

//******************************************************************************
void Edge::print() const {
	cout << "x0: " << p0->x << "\ty0: " << p0->y << "\t\tx1: " << p1->x << "\ty1: " << p1->y << "\t\t";
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

