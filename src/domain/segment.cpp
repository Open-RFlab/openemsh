///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

//#include <vector>

#include "point.hpp"
#include "polygon.hpp"
#include "range.hpp"

#include "segment.hpp"

using namespace std;

//******************************************************************************
/*
Segment::Segment(Axis _axis)
: axis(_axis)
{}
*/

/// Cf. https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect
///*****************************************************************************
relation::SegmentSegment Segment::relation_to(Segment const& segment) const {
	vector<Point const*> v1({ &p0(), &p1(), &segment.p0() });
	vector<Point const*> v2({ &p0(), &p1(), &segment.p1() });
	vector<Point const*> v3({ &segment.p0(), &segment.p1(), &p0() });
	vector<Point const*> v4({ &segment.p0(), &segment.p1(), &p1() });
	Polygon::Rotation r1(detect_rotation(v1));
	Polygon::Rotation r2(detect_rotation(v2));
	Polygon::Rotation r3(detect_rotation(v3));
	Polygon::Rotation r4(detect_rotation(v4));

	if(r1 == Polygon::Rotation::COLINEAR && r2 == Polygon::Rotation::COLINEAR
	&& r3 == Polygon::Rotation::COLINEAR && r4 == Polygon::Rotation::COLINEAR) {
		Bounding a_bnd(bounding(segment));
		Bounding b_bnd(bounding(*this));

		if(((a_bnd[XMIN] >= b_bnd[XMIN] && a_bnd[XMIN] <= b_bnd[XMAX]) || (a_bnd[XMAX] >= b_bnd[XMIN] && a_bnd[XMAX] <= b_bnd[XMAX])
		||  (b_bnd[XMIN] >= a_bnd[XMIN] && b_bnd[XMIN] <= a_bnd[XMAX]) || (b_bnd[XMAX] >= a_bnd[XMIN] && b_bnd[XMAX] <= a_bnd[XMAX]))
		&& ((a_bnd[YMIN] >= b_bnd[YMIN] && a_bnd[YMIN] <= b_bnd[YMAX]) || (a_bnd[YMAX] >= b_bnd[YMIN] && a_bnd[YMAX] <= b_bnd[YMAX])
		||  (b_bnd[YMIN] >= a_bnd[YMIN] && b_bnd[YMIN] <= a_bnd[YMAX]) || (b_bnd[YMAX] >= a_bnd[YMIN] && b_bnd[YMAX] <= a_bnd[YMAX])))
			return relation::SegmentSegment::OVERLAPPING;
		else
			return relation::SegmentSegment::COLINEAR;
	} else if(r1 != r2 && r3 != r4) {
		return relation::SegmentSegment::CROSSING;
	} else {
		return relation::SegmentSegment::APART;
	}
}

//******************************************************************************
relation::SegmentPoint Segment::relation_to(Point const& point) const {
	vector<Point const*> v({ &p0(), &p1(), &point });
	return detect_rotation(v) == Polygon::Rotation::COLINEAR
		&& (point.x <= max(p0().x, p1().x) && point.x >= min(p0().x, p1().x)
		&&  point.y <= max(p0().y, p1().y) && point.y >= min(p0().y, p1().y))
		? relation::SegmentPoint::ON
		: relation::SegmentPoint::OUT;
}

//******************************************************************************
Bounding bounding(Segment const& a) {
	Bounding bounding;
	bounding[XMIN] = a.p0().x < a.p1().x ? a.p0().x : a.p1().x;
	bounding[XMAX] = a.p0().x > a.p1().x ? a.p0().x : a.p1().x;
	bounding[YMIN] = a.p0().y < a.p1().y ? a.p0().y : a.p1().y;
	bounding[YMAX] = a.p0().y > a.p1().y ? a.p0().y : a.p1().y;
	return bounding;
}

/// Returns the intersection point between two edges.
/// @warning Assume both edges are CROSSING.
///
/// TODO should assume a relation vs check : assume
///
/// Returns nullopt if not crossing edges TODO apart edges
/// Cf. https://openclassrooms.com/forum/sujet/calcul-du-point-d-intersection-de-deux-segments-21661
///*****************************************************************************
optional<Point> intersection(Segment const& a, Segment const& b) {
	if(a.axis == Segment::Axis::X && b.axis == Segment::Axis::Y) {
		// Horizontal & vertical
		return Point(b.p0().x, a.p0().y);
	} else if(a.axis == Segment::Axis::Y && b.axis == Segment::Axis::X) {
		// Vertical & horizontal
		return Point(a.p0().x, b.p0().y);
	} else if(a.axis == Segment::Axis::DIAGONAL || b.axis == Segment::Axis::DIAGONAL) {
		// Diagonal
		Point a_vec(a.p1() - a.p0());
		Point b_vec(b.p1() - b.p0());
		double div = a_vec.x * b_vec.y - a_vec.y * b_vec.x;

		if(div != 0) {
			double m = (a_vec.x * a.p0().y
			         - a_vec.x * b.p0().y
			         - a_vec.y * a.p0().x
			         + a_vec.y * b.p0().x)
			         / div;
			if(m >= 0 && m <= 1) // When we are not sure edges are crossing.
				return Point(b.p0() + m * b_vec);
		}
	}
	return nullopt;
}

/// @warning Assume segments are OVERLAPPING.
///*****************************************************************************
optional<Range> merge(Segment const& a, Segment const& b) {
	Point a_vec(a.p1() - a.p0());
	Point b_vec(b.p1() - b.p0());
	if((a.axis == Segment::Axis::X && b.axis == Segment::Axis::X)
	|| (a.axis == Segment::Axis::Y && b.axis == Segment::Axis::Y)
	|| (a_vec.y / a_vec.x == b_vec.y / b_vec.x)) {
		Bounding a_bnd(bounding(a));
		Bounding b_bnd(bounding(b));

		double xmin = 0;
		double xmax = 0;
		if(a_bnd[XMIN] >= b_bnd[XMIN] && a_bnd[XMIN] <= b_bnd[XMAX])
			xmin = b_bnd[XMIN];
		else if(b_bnd[XMIN] >= a_bnd[XMIN] && b_bnd[XMIN] <= a_bnd[XMAX])
			xmin = a_bnd[XMIN];
		else
			return nullopt;

		if(a_bnd[XMAX] >= b_bnd[XMIN] && a_bnd[XMAX] <= b_bnd[XMAX])
			xmax = b_bnd[XMAX];
		else if(b_bnd[XMAX] >= a_bnd[XMIN] && b_bnd[XMAX] <= a_bnd[XMAX])
			xmax = a_bnd[XMAX];

		double ymin = 0;
		double ymax = 0;
		if(a_bnd[YMIN] >= b_bnd[YMIN] && a_bnd[YMIN] <= b_bnd[YMAX])
			ymin = b_bnd[YMIN];
		else if(b_bnd[YMIN] >= a_bnd[YMIN] && b_bnd[YMIN] <= a_bnd[YMAX])
			ymin = a_bnd[YMIN];
		else
			return nullopt;

		if(a_bnd[YMAX] >= b_bnd[YMIN] && a_bnd[YMAX] <= b_bnd[YMAX])
			ymax = b_bnd[YMAX];
		else if(b_bnd[YMAX] >= a_bnd[YMIN] && b_bnd[YMAX] <= a_bnd[YMAX])
			ymax = a_bnd[YMAX];

		return Range(Point(xmin, ymin), Point(xmax, ymax));
	}
	return nullopt;
}

//******************************************************************************
Point mid(Segment const& a) {
	return mid(a.p0(), a.p1());
}

/// Returns the overlap range between two edges.
/// @warning Assume both edges are OVERLAPPING.
/// Returns nullopt if edges are not prallels or if boundings do not overlap.
///*****************************************************************************
optional<Range> overlap(Segment const& a, Segment const& b) {
	Point a_vec(a.p1() - a.p0());
	Point b_vec(b.p1() - b.p0());
	if((a.axis == Segment::Axis::X && b.axis == Segment::Axis::X)
	|| (a.axis == Segment::Axis::Y && b.axis == Segment::Axis::Y)
	|| (a_vec.y / a_vec.x == b_vec.y / b_vec.x)) {
		Bounding a_bnd(bounding(a));
		Bounding b_bnd(bounding(b));

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
