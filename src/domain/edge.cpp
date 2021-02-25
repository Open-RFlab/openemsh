///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include <array>

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
			return relation::EdgeEdge::OVERLAPPING;
		else
			return relation::EdgeEdge::COLINEAR;
	} else if(r1 != r2 && r3 != r4) {
		return relation::EdgeEdge::CROSSING;
	} else {
		return relation::EdgeEdge::APART;
	}
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
/// Returns nullopt if not overlapping edges of if edges overlaps in diagonal
/// TODO can easily work for diag!
///*****************************************************************************
optional<Range> overlap(Edge const* a, Edge const* b) {
//array<Point, 2>* overlap(Edge const& a, Edge const& b) {
//Edge* overlap(Edge const& a, Edge const& b) {
	if((a->direction == Edge::Direction::XMIN || a->direction == Edge::Direction::XMAX)
	&& (b->direction == Edge::Direction::XMIN || b->direction == Edge::Direction::XMAX)) {
		// Parallel |

	} else if((a->direction == Edge::Direction::YMIN || a->direction == Edge::Direction::YMAX)
	       && (b->direction == Edge::Direction::YMIN || b->direction == Edge::Direction::YMAX)) {
		// Parallel _
//		return new Range();
	}
	return nullopt;
}
