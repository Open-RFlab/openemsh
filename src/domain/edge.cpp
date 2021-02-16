///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include <array>

#include "point.hpp"
#include "polygon.hpp"

#include "edge.hpp"

using namespace std;

//******************************************************************************
Edge::Edge(Point const* _p0, Point const* _p1)
: p0(_p0)
, p1(_p1)
, vec(make_unique<Point>(*p1 - *p0))
{
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

/// Cf. https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect
///*****************************************************************************
relation::EdgeEdge Edge::relation_to(Edge const* edge) const {
	Polygon::Rotation r1 = detect_rotation(vector<Point const*>({ p0, p1, edge->p0 }));
	Polygon::Rotation r2 = detect_rotation(vector<Point const*>({ p0, p1, edge->p1 }));
	Polygon::Rotation r3 = detect_rotation(vector<Point const*>({ edge->p0, edge->p1, p0 }));
	Polygon::Rotation r4 = detect_rotation(vector<Point const*>({ edge->p0, edge->p1, p1 }));

	if(r1 == Polygon::Rotation::COLINEAR && r2 == Polygon::Rotation::COLINEAR
	&& r3 == Polygon::Rotation::COLINEAR && r4 == Polygon::Rotation::COLINEAR) {
		array<double, 4> e;
		e[XMIN] = edge->p0->x <= edge->p1->x ? edge->p0->x : edge->p1->x;
		e[XMAX] = edge->p0->x > edge->p1->x ? edge->p0->x : edge->p1->x;
		e[YMIN] = edge->p0->y <= edge->p1->y ? edge->p0->y : edge->p1->y;
		e[YMAX] = edge->p0->y > edge->p1->y ? edge->p0->y : edge->p1->y;
		if(((p0->x >= e[XMIN] && p0->x <= e[XMAX]) || (p1->x >= e[XMIN] && p1->x <= e[XMAX]))
		&& ((p0->y >= e[YMIN] && p0->y <= e[YMAX]) || (p1->y >= e[YMIN] && p1->y <= e[YMAX])))
			return relation::EdgeEdge::COLINEAR_CROSSING;
		else
			return relation::EdgeEdge::COLINEAR;
	} else if(r1 != r2 && r3 != r4) {
		return relation::EdgeEdge::CROSSING;
	} else {
		return relation::EdgeEdge::APART;
	}
}
