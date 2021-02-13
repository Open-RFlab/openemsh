///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include "polygon.hpp"

using namespace std;

//******************************************************************************
Polygon::Polygon(Rotation _rotation, initializer_list<Point> _points)
: rotation(_rotation)
, bounding({ begin(_points)->x, begin(_points)->x, begin(_points)->y, begin(_points)->y }) {
	for(Point const& point : _points) {
		points.push_back(make_unique<Point>(point));

		if(point.x < bounding[XMIN]) bounding[XMIN] = point.x;
		if(point.x > bounding[XMAX]) bounding[XMAX] = point.x;
		if(point.y < bounding[YMIN]) bounding[YMIN] = point.y;
		if(point.y > bounding[YMAX]) bounding[YMAX] = point.y;
	}

	detect_rotation();

	Point const* prev = points.back().get();
	for(unique_ptr<Point const>& point : points) {
		edges.push_back(make_unique<Edge>(prev, point.get()));
		prev = point.get();
	}

	detect_edge_normal();
}

/// Cf. https://rosettacode.org/wiki/Shoelace_formula_for_polygonal_area#C.2B.2B
/// Cf. https://www.baeldung.com/cs/list-polygon-points-clockwise
///*****************************************************************************
inline void Polygon::detect_rotation() {
	double left_sum = 0.0;
	double right_sum = 0.0;
 
	for(unsigned long i = 0; i < points.size(); ++i) {
		unsigned long j = (i + 1) % points.size();
		left_sum  += points[i]->x * points[j]->y;
		right_sum += points[j]->x * points[i]->y;
	}

	double area = 0.5 * (left_sum - right_sum);

	if(area > 0)
		rotation = Rotation::CCW;
	else if(area < 0)
		rotation = Rotation::CW;
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
inline void Polygon::detect_edge_normal() {
	switch(rotation) {
	case Rotation::CW: 
		for(std::unique_ptr<Edge>& edge : edges) {
			switch(edge->direction) {
			case Edge::Direction::XMIN: edge->normal = Edge::Normal::YMIN; break;
			case Edge::Direction::XMAX: edge->normal = Edge::Normal::YMAX; break;
			case Edge::Direction::YMIN: edge->normal = Edge::Normal::XMAX; break;
			case Edge::Direction::YMAX: edge->normal = Edge::Normal::XMIN; break;
			default: break;
			}
		}
		break;
	case Rotation::CCW:
		for(std::unique_ptr<Edge>& edge : edges) {
			switch(edge->direction) {
			case Edge::Direction::XMIN: edge->normal = Edge::Normal::YMAX; break;
			case Edge::Direction::XMAX: edge->normal = Edge::Normal::YMIN; break;
			case Edge::Direction::YMIN: edge->normal = Edge::Normal::XMIN; break;
			case Edge::Direction::YMAX: edge->normal = Edge::Normal::XMAX; break;
			default: break;
			}
		}
		break;
	default: break;
	}	
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
	case Rotation::UNKNOWN: cout << "UNKNOWN" << endl; break;
	}
}

