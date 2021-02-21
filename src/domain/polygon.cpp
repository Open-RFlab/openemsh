///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include <limits>

#include "edge.hpp"
#include "point.hpp"

#include "polygon.hpp"

using namespace std;

//******************************************************************************
Polygon::Polygon(Rotation _rotation, initializer_list<Point> _points)
: rotation(_rotation)
, bounding({ begin(_points)->x, begin(_points)->x, begin(_points)->y, begin(_points)->y }) {
	for(Point const& point : _points) {
		points.push_back(make_unique<Point>(point)); // TODO do not use initializer_list because of copies

		if(point.x < bounding[XMIN]) bounding[XMIN] = point.x;
		if(point.x > bounding[XMAX]) bounding[XMAX] = point.x;
		if(point.y < bounding[YMIN]) bounding[YMIN] = point.y;
		if(point.y > bounding[YMAX]) bounding[YMAX] = point.y;
	}

	rotation = detect_rotation(points);

	Point const* prev = points.back().get();
	for(unique_ptr<Point const>& point : points) {
		edges.push_back(make_unique<Edge>(prev, point.get()));
		prev = point.get();
	}

	detect_edge_normal();
}

/// Cf. https://rosettacode.org/wiki/Shoelace_formula_for_polygonal_area#C.2B.2B
/// Cf. https://www.baeldung.com/cs/list-polygon-points-clockwise
///
/// Due to floating point calcul, might fail to detect colinear polygons if
/// if those are not also colinear to x or y axis? Not problematic though since
/// FDTD mesh is orthogonal.
///*****************************************************************************
template<class T>
inline Polygon::Rotation detect_rotation(T& points) {
	double left_sum = 0.0;
	double right_sum = 0.0;
 
	for(size_t i = 0; i < points.size(); ++i) {
		size_t j = (i + 1) % points.size();
		left_sum  += points[i]->x * points[j]->y;
		right_sum += points[j]->x * points[i]->y;
	}

	double area = 0.5 * (left_sum - right_sum);

	if(area > 0)
		return Polygon::Rotation::CCW;
	else if(area < 0)
		return Polygon::Rotation::CW;
	else
		return Polygon::Rotation::COLINEAR;
}

//******************************************************************************
Polygon::Rotation detect_rotation(vector<unique_ptr<Point const>> const& points) {
	return detect_rotation<vector<unique_ptr<Point const>> const&>(points);
}

//******************************************************************************
Polygon::Rotation detect_rotation(vector<Point const*> const points) {
	return detect_rotation<vector<Point const*> const>(points);
}

///           YMAX          |           YMAX
///             ^           |             ^
///             |           |             |
///    X   ^------>|   X    |    X   |<------^   X
///    M<--|       |   M    |    M<--|       |   M
///    I   |       |-->A    |    I   |       |-->A
///    N   |<------v   X    |    N   v------>|   X
///           |             |           |
///           v             |           v
///          YMIN           |          YMIN
///           CW            |           CCW
///*****************************************************************************
void Polygon::detect_edge_normal() {
	switch(rotation) {
	case Rotation::CW: 
		for(std::unique_ptr<Edge>& edge : edges) {
			switch(edge->direction) {
			case Edge::Direction::XMIN: edge->normal = Normal::YMIN; break;
			case Edge::Direction::XMAX: edge->normal = Normal::YMAX; break;
			case Edge::Direction::YMIN: edge->normal = Normal::XMAX; break;
			case Edge::Direction::YMAX: edge->normal = Normal::XMIN; break;
			default: break;
			}
		}
		break;
	case Rotation::CCW:
		for(std::unique_ptr<Edge>& edge : edges) {
			switch(edge->direction) {
			case Edge::Direction::XMIN: edge->normal = Normal::YMAX; break;
			case Edge::Direction::XMAX: edge->normal = Normal::YMIN; break;
			case Edge::Direction::YMIN: edge->normal = Normal::XMIN; break;
			case Edge::Direction::YMAX: edge->normal = Normal::XMAX; break;
			default: break;
			}
		}
		break;
	default: break;
	}	
}

// TODO handle polygon edges colinear to ray.
//******************************************************************************
relation::PolygonPoint Polygon::relation_to(Point const* point) const {
	Point p(numeric_limits<double>::infinity(), point->y);
	Edge ray(point, &p);

	unsigned int count = 0;
	for(unique_ptr<Edge> const& edge : edges) {
		if(edge->relation_to(&ray) == relation::EdgeEdge::CROSSING)
			++count;
	}

	if(count % 2)
		return relation::PolygonPoint::IN;
	else
		return relation::PolygonPoint::OUT;
}


//******************************************************************************
void Polygon::print() const {
	for(unique_ptr<Point const> const& point : points)
		point->print();
	for(unique_ptr<Edge> const& edge : edges)
		edge->print();
	cout << "x: " << bounding[XMIN] << " <-> " << bounding[XMAX] << endl;
	cout << "y: " << bounding[YMIN] << " <-> " << bounding[YMAX] << endl;
	switch(rotation) {
	case Rotation::CW: cout << "CW" << endl; break;
	case Rotation::CCW: cout << "CCW" << endl; break;
	case Rotation::COLINEAR: cout << "COLINEAR" << endl; break;
	case Rotation::UNKNOWN: cout << "UNKNOWN" << endl; break;
	}
}

/// Check if the boundings of two polygons overlap or just touch each other.
///*****************************************************************************
bool are_possibly_overlapping(Polygon const& a, Polygon const& b) {
	return (b.bounding[XMIN] >= a.bounding[XMIN] && b.bounding[XMIN] <= a.bounding[XMAX])
		|| (b.bounding[XMAX] >= a.bounding[XMIN] && b.bounding[XMAX] <= a.bounding[XMAX])
		|| (b.bounding[YMIN] >= a.bounding[YMIN] && b.bounding[YMIN] <= a.bounding[YMAX])
		|| (b.bounding[YMAX] >= a.bounding[YMIN] && b.bounding[YMAX] <= a.bounding[YMAX]);
}