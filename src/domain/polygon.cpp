///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include "edge.hpp"
#include "point.hpp"

#include "polygon.hpp"

using namespace std;

//******************************************************************************
Polygon::Polygon(initializer_list<Point> _points)
: bounding({ begin(_points)->x, begin(_points)->x, begin(_points)->y, begin(_points)->y }) {
	for(Point const& point : _points) {
		points.push_back(make_unique<Point>(point)); // TODO do not use initializer_list because of copies

		if(point.x < bounding[XMIN]) bounding[XMIN] = point.x;
		if(point.x > bounding[XMAX]) bounding[XMAX] = point.x;
		if(point.y < bounding[YMIN]) bounding[YMIN] = point.y;
		if(point.y > bounding[YMAX]) bounding[YMAX] = point.y;
	}
	points.shrink_to_fit();

	rotation = detect_rotation(points);

	Point const* prev = points.back().get();
	for(unique_ptr<Point const>& point : points) {
		edges.push_back(make_unique<Edge>(prev, point.get()));
		prev = point.get();
	}
	edges.shrink_to_fit();

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

/// Based on the ray casting method.
/// Vertices on the chosen ray may false the result so multiple retrys may be
/// needed. The first rays tried are the 4 orthogonal ones because detecting a
/// crossing relation with an edge will be faster with these ones. Then
/// diagonals will be tried.
///*****************************************************************************
relation::PolygonPoint Polygon::relation_to(Point const* point) const {
	vector<Point> to_try({
		{ bounding[XMIN] - 1, point->y },
		{ bounding[XMAX] + 1, point->y },
		{ point->x, bounding[YMIN] - 1 },
		{ point->x, bounding[YMAX] + 1 }});
		/* The followings are like that.
		{ bounding[XMAX] + 1, point->y + n } */

	bool need_retry;
	unsigned int count;
	unsigned int n = 0;

	do {
		need_retry = false;
		count = 0;
		if(to_try.size() <= n)
			to_try.emplace_back(bounding[XMAX] + 1, point->y + n);
		Edge ray(point, &to_try[n++]);

		// Detect vertices on the ray.
		for(unique_ptr<Point const> const& p : points) {
			if(*p == *point) {
				return relation::PolygonPoint::ON;
			} else if(ray.relation_to(p.get()) == relation::EdgePoint::ON) {
				need_retry = true;
				break;
			}
		}
		if(need_retry)
			continue;

		for(unique_ptr<Edge> const& edge : edges) {
			if(edge->relation_to(point) == relation::EdgePoint::ON)
				return relation::PolygonPoint::ON;

			if(edge->relation_to(&ray) == relation::EdgeEdge::CROSSING) {
				++count;
			}
		}
	} while(need_retry);

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
	}
}

/// Check if the boundings of two polygons overlap or just touch each other.
///*****************************************************************************
bool are_possibly_overlapping(Polygon const& a, Polygon const& b) {
	return((b.bounding[XMIN] >= a.bounding[XMIN] && b.bounding[XMIN] <= a.bounding[XMAX])
		|| (b.bounding[XMAX] >= a.bounding[XMIN] && b.bounding[XMAX] <= a.bounding[XMAX])
		|| (a.bounding[XMIN] >= b.bounding[XMIN] && a.bounding[XMIN] <= b.bounding[XMAX])
		|| (a.bounding[XMAX] >= b.bounding[XMIN] && a.bounding[XMAX] <= b.bounding[XMAX]))
		&&((b.bounding[YMIN] >= a.bounding[YMIN] && b.bounding[YMIN] <= a.bounding[YMAX])
		|| (b.bounding[YMAX] >= a.bounding[YMIN] && b.bounding[YMAX] <= a.bounding[YMAX])
		|| (a.bounding[YMIN] >= b.bounding[YMIN] && a.bounding[YMIN] <= b.bounding[YMAX])
		|| (a.bounding[YMAX] >= b.bounding[YMIN] && a.bounding[YMAX] <= b.bounding[YMAX]));
}