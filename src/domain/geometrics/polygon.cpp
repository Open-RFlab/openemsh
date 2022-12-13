///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

#include "utils/unreachable.hpp"
#include "edge.hpp"
#include "point.hpp"

#include "polygon.hpp"

namespace domain {

using namespace std;

//******************************************************************************
Polygon::Polygon(Plane const plane, string const& name, vector<unique_ptr<Point const>>&& points)
: rotation(detect_rotation(points))
, plane(plane)
, bounding(detect_bounding(points))
, name(name)
, points(move(points))
, edges(detect_edges(this->points, plane))
{
	detect_edge_normal();
}

//******************************************************************************
vector<unique_ptr<Edge>> detect_edges(vector<unique_ptr<Point const>> const& points, Plane const plane) {
	vector<unique_ptr<Edge>> edges;

	Point const* prev = points.back().get();
	for(auto const & point : points) {
		edges.push_back(make_unique<Edge>(plane, prev, point.get()));
		prev = point.get();
	}
	edges.shrink_to_fit();

	return edges;
}

/// Cf. https://rosettacode.org/wiki/Shoelace_formula_for_polygonal_area#C.2B.2B
/// Cf. https://www.baeldung.com/cs/list-polygon-points-clockwise
///
/// Due to floating point calcul, might fail to detect colinear polygons if
/// if those are not also colinear to x or y axis? Not problematic though since
/// FDTD mesh is orthogonal.
///*****************************************************************************
template<class T>
Polygon::Rotation detect_rotation(T const& points) noexcept {
	double left_sum = 0.0;
	double right_sum = 0.0;

	for(size_t i = 0; i < points.size(); ++i) {
		size_t j = (i + 1) % points.size();
		left_sum  += double (points[i]->x * points[j]->y);
		right_sum += double (points[j]->x * points[i]->y);
	}

	double area = 0.5 * (left_sum - right_sum);

	if(Coord(area) == 0)
		return Polygon::Rotation::COLINEAR;
	else if(area > 0)
		return Polygon::Rotation::CCW;
	else if(area < 0)
		return Polygon::Rotation::CW;
	else
		unreachable();
}

//******************************************************************************
template Polygon::Rotation detect_rotation(std::vector<std::unique_ptr<Point const>> const&) noexcept;
template Polygon::Rotation detect_rotation(std::vector<Point const*> const&) noexcept;

//******************************************************************************
Bounding2D detect_bounding(vector<unique_ptr<Point const>> const& points) noexcept {
	Bounding2D bounding({
		(*begin(points))->x,
		(*begin(points))->x,
		(*begin(points))->y,
		(*begin(points))->y });

	for(auto const& point : points) {
		if(point->x < bounding[XMIN]) bounding[XMIN] = point->x;
		if(point->x > bounding[XMAX]) bounding[XMAX] = point->x;
		if(point->y < bounding[YMIN]) bounding[YMIN] = point->y;
		if(point->y > bounding[YMAX]) bounding[YMAX] = point->y;
	}

	return bounding;
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
void Polygon::detect_edge_normal() noexcept {
	switch(rotation) {
	case Rotation::CW:
		for(auto const& edge : edges) {
			switch(edge->direction) {
			case Edge::Direction::XMIN: edge->normal = Normal::YMIN; break;
			case Edge::Direction::XMAX: edge->normal = Normal::YMAX; break;
			case Edge::Direction::YMIN: edge->normal = Normal::XMAX; break;
			case Edge::Direction::YMAX: edge->normal = Normal::XMIN; break;
			default: edge->normal = Normal::NONE; break;
			}
		}
		break;
	case Rotation::CCW:
		for(auto const& edge : edges) {
			switch(edge->direction) {
			case Edge::Direction::XMIN: edge->normal = Normal::YMAX; break;
			case Edge::Direction::XMAX: edge->normal = Normal::YMIN; break;
			case Edge::Direction::YMIN: edge->normal = Normal::XMIN; break;
			case Edge::Direction::YMAX: edge->normal = Normal::XMAX; break;
			default: edge->normal = Normal::NONE; break;
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
relation::PolygonPoint Polygon::relation_to(Point const& point) const noexcept {
	vector<Point> to_try({
		{ bounding[XMIN] - 1, point.y },
		{ bounding[XMAX] + 1, point.y },
		{ point.x, bounding[YMIN] - 1 },
		{ point.x, bounding[YMAX] + 1 }});
		/* The followings are like that.
		{ bounding[XMAX] + 1, point->y + n } */

	bool need_retry;
	unsigned int count;
	unsigned int n = 0;

	do {
		need_retry = false;
		count = 0;
		if(to_try.size() <= n)
			to_try.emplace_back(bounding[XMAX] + 1, point.y + n);
		Edge ray(plane, &point, &to_try[n++]);

		// Detect vertices on the ray.
		for(auto const& p : points) {
			if(*p == point) {
				return relation::PolygonPoint::ON;
			} else if(ray.relation_to(*p) == relation::SegmentPoint::ON) {
				need_retry = true;
				break;
			}
		}
		if(need_retry)
			continue;

		for(auto const& edge : edges) {
			if(edge->relation_to(point) == relation::SegmentPoint::ON)
				return relation::PolygonPoint::ON;

			if(edge->relation_to(ray) == relation::SegmentSegment::CROSSING) {
				++count;
			}
		}
	} while(need_retry);

	if(count % 2)
		return relation::PolygonPoint::IN;
	else
		return relation::PolygonPoint::OUT;
}

#ifdef DEBUG
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
#endif // DEBUG

} // namespace domain
