///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include <algorithm>
//#include <array>

#include "point.hpp"
#include "polygon.hpp"
#include "range.hpp"

#include "edge.hpp"

using namespace std;

//******************************************************************************
Edge::Edge(Point const* _p0, Point const* _p1)
: p0(_p0)
, p1(_p1)
, vec(make_unique<Point>(*p1 - *p0))
, normal(Normal::NONE)
//, bounding({
//	(p0->x <= p1->x ? p0->x : p1->x),
//	(p0->x >= p1->x ? p1->x : p0->x),
//	(p0->y <= p1->y ? p0->y : p1->y),
//	(p0->y >= p1->y ? p1->y : p0->y) })
{
	if(vec->x == 0) {
		axis = Axis::Y;
		if(vec->y > 0)
			direction = Direction::YMAX;
		else if(vec->y < 0)
			direction = Direction::YMIN;
	} else if(vec->y == 0) {
		axis = Axis::X;
		if(vec->x > 0)
			direction = Direction::XMAX;
		else if(vec->x < 0)
			direction = Direction::XMIN;
	} else {
		axis = Axis::DIAGONAL;
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
		array<double, 4> a_bnd;
		a_bnd[XMIN] = edge->p0->x < edge->p1->x ? edge->p0->x : edge->p1->x;
		a_bnd[XMAX] = edge->p0->x > edge->p1->x ? edge->p0->x : edge->p1->x;
		a_bnd[YMIN] = edge->p0->y < edge->p1->y ? edge->p0->y : edge->p1->y;
		a_bnd[YMAX] = edge->p0->y > edge->p1->y ? edge->p0->y : edge->p1->y;
		array<double, 4> b_bnd;
		b_bnd[XMIN] = p0->x < p1->x ? p0->x : p1->x;
		b_bnd[XMAX] = p0->x > p1->x ? p0->x : p1->x;
		b_bnd[YMIN] = p0->y < p1->y ? p0->y : p1->y;
		b_bnd[YMAX] = p0->y > p1->y ? p0->y : p1->y;
		if(((a_bnd[XMIN] >= b_bnd[XMIN] && a_bnd[XMIN] <= b_bnd[XMAX]) || (a_bnd[XMAX] >= b_bnd[XMIN] && a_bnd[XMAX] <= b_bnd[XMAX])
		||  (b_bnd[XMIN] >= a_bnd[XMIN] && b_bnd[XMIN] <= a_bnd[XMAX]) || (b_bnd[XMAX] >= a_bnd[XMIN] && b_bnd[XMAX] <= a_bnd[XMAX]))
		&& ((a_bnd[YMIN] >= b_bnd[YMIN] && a_bnd[YMIN] <= b_bnd[YMAX]) || (a_bnd[YMAX] >= b_bnd[YMIN] && a_bnd[YMAX] <= b_bnd[YMAX])
		||  (b_bnd[YMIN] >= a_bnd[YMIN] && b_bnd[YMIN] <= a_bnd[YMAX]) || (b_bnd[YMAX] >= a_bnd[YMIN] && b_bnd[YMAX] <= a_bnd[YMAX])))
			return relation::EdgeEdge::OVERLAPPING;
		else
			return relation::EdgeEdge::COLINEAR;
	} else if(r1 != r2 && r3 != r4) {
		return relation::EdgeEdge::CROSSING;
	} else {
		return relation::EdgeEdge::APART;
	}
}

//*****************************************************************************
relation::EdgePoint Edge::relation_to(Point const* point) const {
	return detect_rotation(vector<Point const*>({ p0, p1, point })) == Polygon::Rotation::COLINEAR
		&& (point->x <= max(p0->x, p1->x) && point->x >= min(p0->x, p1->x)
		&&  point->y <= max(p0->y, p1->y) && point->y >= min(p0->y, p1->y))
		? relation::EdgePoint::ON
		: relation::EdgePoint::OUT;
}

/// Returns the intersection point between two edges.
/// @warning Assume both edges are CROSSING.
///
/// TODO should assume a relation vs check : assume
/// TODO avoid return new! : ret Point, ret std::optional?
///
/// Returns nullopt if not crossing edges TODO apart edges
/// Cf. https://openclassrooms.com/forum/sujet/calcul-du-point-d-intersection-de-deux-segments-21661
///*****************************************************************************
optional<Point> intersection(Edge const* a, Edge const* b) {
	if((a->direction == Edge::Direction::XMIN || a->direction == Edge::Direction::XMAX)
	&& (b->direction == Edge::Direction::YMIN || b->direction == Edge::Direction::YMAX)) {
		// Horizontal & vertical
		return Point(b->p0->x, a->p0->y);
	} else if((a->direction == Edge::Direction::YMIN || a->direction == Edge::Direction::YMAX)
	       && (b->direction == Edge::Direction::XMIN || b->direction == Edge::Direction::XMAX)) {
		// Vertical & horizontal
		return Point(a->p0->x, b->p0->y);
	} else if(a->direction == Edge::Direction::DIAGONAL || b->direction == Edge::Direction::DIAGONAL) {
		// Diagonal
		double m = 0;
		double div = a->vec->x * b->vec->y - a->vec->y * b->vec->x;

		if(div != 0) {
			m = (a->vec->x * a->p0->y
			  - a->vec->x * b->p0->y
			  - a->vec->y * a->p0->x
			  + a->vec->y * b->p0->x)
			  / div;
			if(m >= 0 && m <= 1) // When we are not sure edges are crossing.
				return Point(*(b->p0) + m * *(b->vec));
		}
	}
	return nullopt;
}

/// Returns the overlap range between two edges only if both are horizontal or
/// if both are vertical. Diagonal overlaps are not handled for now.
/// @warning Assume both edges are OVERLAPPING and horizontal or vertical.
/// Returns nullopt if not overlapping edges.
///*****************************************************************************
optional<Range> overlap(Edge const* a, Edge const* b) {
//optional<pair<Point>> overlap(Edge const* a, Edge const* b) { // TODO ?
	if((a->axis == Edge::Axis::X && b->axis == Edge::Axis::X)
	|| (a->axis == Edge::Axis::Y && b->axis == Edge::Axis::Y)
	|| (a->vec->y / a->vec->x == b->vec->y / b->vec->x)) {
		array<double, 4> a_bnd;
		a_bnd[XMIN] = a->p0->x < a->p1->x ? a->p0->x : a->p1->x;
		a_bnd[XMAX] = a->p0->x > a->p1->x ? a->p0->x : a->p1->x;
		a_bnd[YMIN] = a->p0->y < a->p1->y ? a->p0->y : a->p1->y;
		a_bnd[YMAX] = a->p0->y > a->p1->y ? a->p0->y : a->p1->y;
		array<double, 4> b_bnd;
		b_bnd[XMIN] = b->p0->x < b->p1->x ? b->p0->x : b->p1->x;
		b_bnd[XMAX] = b->p0->x > b->p1->x ? b->p0->x : b->p1->x;
		b_bnd[YMIN] = b->p0->y < b->p1->y ? b->p0->y : b->p1->y;
		b_bnd[YMAX] = b->p0->y > b->p1->y ? b->p0->y : b->p1->y;

		double xmin = 0;
		double xmax = 0;
		if(a_bnd[XMIN] >= b_bnd[XMIN] && a_bnd[XMIN] <= b_bnd[XMAX])
			xmin = a_bnd[XMIN];
		else if(b_bnd[XMIN] >= a_bnd[XMIN] && b_bnd[XMIN] <= a_bnd[XMAX])
			xmin = b_bnd[XMIN];
		else
			return nullopt;

		if(a_bnd[XMAX] >= b_bnd[XMIN] && a_bnd[XMAX] <= b_bnd[XMAX])
			xmax = a_bnd[XMAX];
		else if(b_bnd[XMAX] >= a_bnd[XMIN] && b_bnd[XMAX] <= a_bnd[XMAX])
			xmax = b_bnd[XMAX];

		double ymin = 0;
		double ymax = 0;
		if(a_bnd[YMIN] >= b_bnd[YMIN] && a_bnd[YMIN] <= b_bnd[YMAX])
			ymin = a_bnd[YMIN];
		else if(b_bnd[YMIN] >= a_bnd[YMIN] && b_bnd[YMIN] <= a_bnd[YMAX])
			ymin = b_bnd[YMIN];
		else
			return nullopt;

		if(a_bnd[YMAX] >= b_bnd[YMIN] && a_bnd[YMAX] <= b_bnd[YMAX])
			ymax = a_bnd[YMAX];
		else if(b_bnd[YMAX] >= a_bnd[YMIN] && b_bnd[YMAX] <= a_bnd[YMAX])
			ymax = b_bnd[YMAX];

		return Range(Point(xmin, ymin), Point(xmax, ymax));
	}
	return nullopt;
}

//******************************************************************************
Range::Axis cast(Edge::Axis a) {
	switch(a) {
	case Edge::Axis::X: return Range::Axis::X;
	case Edge::Axis::Y: return Range::Axis::Y;
	case Edge::Axis::DIAGONAL: return Range::Axis::DIAGONAL;
	}
}