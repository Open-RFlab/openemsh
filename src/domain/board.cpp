///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#ifdef DEBUG
#include <iostream>
#endif // DEBUG

#include <algorithm>
#include <utility>

#include "utils/signum.hpp"
#include "conflicts/conflict_edge_in_polygon.hpp"

#include "board.hpp"

using namespace std;

/// @warning Undefined behavior if points are not all colinear.
///*****************************************************************************
void sort_points_by_vector_orientation(vector<Point>& points, Point const& vector) {
	int8_t x_sign = signum(vector.x);
	int8_t y_sign = signum(vector.y);
	if(x_sign > 0 && y_sign == 0)
		sort(begin(points), end(points),
			[](Point const& a, Point const& b) {
				return (a.x < b.x);
			});
	else if(x_sign == 0 && y_sign > 0)
		sort(begin(points), end(points),
			[](Point const& a, Point const& b) {
				return (a.y < b.y);
			});
	else if(x_sign < 0 && y_sign == 0)
		sort(begin(points), end(points),
			[](Point const& a, Point const& b) {
				return (a.x > b.x);
			});
	else if(x_sign == 0 && y_sign < 0)
		sort(begin(points), end(points),
			[](Point const& a, Point const& b) {
				return (a.y > b.y);
			});
	else if(x_sign > 0 && y_sign > 0)
		sort(begin(points), end(points),
			[](Point const& a, Point const& b) {
				return (a.x < b.x && a.y < b.y);
			});
	else if(x_sign < 0 && y_sign > 0)
		sort(begin(points), end(points),
			[](Point const& a, Point const& b) {
				return (a.x > b.x && a.y < b.y);
			});
	else if(x_sign < 0 && y_sign < 0)
		sort(begin(points), end(points),
			[](Point const& a, Point const& b) {
				return (a.x > b.x && a.y > b.y);
			});
	else if(x_sign > 0 && y_sign < 0)
		sort(begin(points), end(points),
			[](Point const& a, Point const& b) {
				return (a.x < b.x && a.y > b.y);
			});
}

//******************************************************************************
Board::Board(vector<unique_ptr<Polygon>>& polygons)
: conflict_manager(&line_policy_manager)
, line_policy_manager(params)
, polygons(move(polygons)) {
	for(unique_ptr<Polygon>& polygon : this->polygons)
		for(unique_ptr<Edge>& edge : polygon->edges)
			edges.push_back(edge.get());
	this->polygons.shrink_to_fit();
	edges.shrink_to_fit();
}

/// Detect all EDGE_IN_POLYGON. Will also detect some COLINEAR_EDGES.
/// Overlapping edges should be EDGE_IN_POLYGON and not COLINEAR_EDGES.
///*****************************************************************************
void Board::detect_edges_in_polygons() {
	for(unique_ptr<Polygon>& poly_a : polygons) {
		for(unique_ptr<Polygon>& poly_b : polygons) {
			if(poly_b == poly_a)
				continue;

			for(unique_ptr<Edge>& edge_a : poly_a->edges) {

				struct RangeBtwIntersections {
					Range const range;
					optional<relation::PolygonPoint> rel_to_poly_b;
					optional<Point> const mid;
					explicit RangeBtwIntersections(Range const& _range)
					: range(_range)
					, rel_to_poly_b(nullopt)
					, mid(::mid(_range))
					{}
					RangeBtwIntersections(Range const& _range, relation::PolygonPoint _rel_to_poly_b)
					: range(_range)
					, rel_to_poly_b(_rel_to_poly_b)
					, mid(nullopt)
					{}
#ifdef DEBUG
					void print() {
						cout << "range : " << endl;
						range.p0().print();
						range.p1().print();
						cout << "mid : " << endl;
						if(mid) mid->print();
						if(rel_to_poly_b) {
							cout << "rel : ";
							switch(rel_to_poly_b.value()) {
							case relation::PolygonPoint::IN: cout << "IN"; break;
							case relation::PolygonPoint::ON: cout << "ON"; break;
							case relation::PolygonPoint::OUT: cout << "OUT"; break;
							}
							cout << endl;
						}
					}
#endif // DEBUG
				};

				vector<Point> intersections;
				vector<RangeBtwIntersections> ranges;

				for(unique_ptr<Edge>& edge_b : poly_b->edges) {
					relation::SegmentSegment rel = edge_a->relation_to(*edge_b);
					switch(rel) {
					case relation::SegmentSegment::CROSSING:
						if(optional<Point> p = intersection(*edge_a, *edge_b))
							intersections.push_back(p.value());
						break;
					case relation::SegmentSegment::OVERLAPPING:
						if(optional<Range> r = overlap(*edge_a, *edge_b)) {
							intersections.push_back(r->p0());
							intersections.push_back(r->p1());
							if(r->axis == Segment::Axis::POINT)
								break;
							ranges.emplace_back(r.value(), relation::PolygonPoint::ON);
							conflict_manager.add_edge_in_polygon(edge_a.get(), poly_b.get(), r.value(), edge_b.get());
						}
						break;
					default:
						break;
					}
				}

				relation::PolygonPoint rel_p0 = poly_b->relation_to(edge_a->p0());
				relation::PolygonPoint rel_p1 = poly_b->relation_to(edge_a->p1());

				if(!intersections.size()
//				&& !ranges.size()
				&& rel_p0 == relation::PolygonPoint::IN
				&& rel_p1 == relation::PolygonPoint::IN) {
					conflict_manager.add_edge_in_polygon(edge_a.get(), poly_b.get());
				} else if(intersections.size()) {
					intersections.push_back(edge_a->p0());
					intersections.push_back(edge_a->p1());

					sort_points_by_vector_orientation(intersections, edge_a->vec);
					auto last = unique(begin(intersections), end(intersections));
					intersections.erase(last, end(intersections));

					// TODO if p0, p1 IN || OUT -> add Range(... IN || OUT)
					if(rel_p0 == relation::PolygonPoint::IN
					|| rel_p0 == relation::PolygonPoint::OUT)
						ranges.emplace_back(Range(intersections[0], intersections[1]), rel_p0);
					if(rel_p1 == relation::PolygonPoint::IN
					|| rel_p1 == relation::PolygonPoint::OUT)
						ranges.emplace_back(Range(intersections[intersections.size()-2], intersections.back()), rel_p1);

					size_t overlapping_ranges = ranges.size();
					for(size_t i = 1; i < intersections.size(); ++i) {
						Range current_range(intersections[i - 1], intersections[i]);

						bool is_already_there = false;
						for(size_t j = 0; j < overlapping_ranges; ++j) {
							if(ranges[j].range == current_range) {
								is_already_there = true;
								break;
							}
						}
						if(is_already_there)
							continue;

						ranges.emplace_back(move(current_range));
					}

					for(RangeBtwIntersections& range : ranges) {
						if(range.mid.has_value()
						&& poly_b->relation_to(range.mid.value()) == relation::PolygonPoint::IN) {
/*						|| poly_b->relation_to(&range.mid.value()) == relation::PolygonPoint::ON))
*/							conflict_manager.add_edge_in_polygon(edge_a.get(), poly_b.get(), range.range);
//							range.rel_to_poly_b = poly_b->relation_to(&range.mid.value()); //TODO useless
						} else if(range.rel_to_poly_b == relation::PolygonPoint::IN) {
							conflict_manager.add_edge_in_polygon(edge_a.get(), poly_b.get(), range.range);
						}
					}
/*
					for(RangeBtwIntersections& range : ranges) {
						range.print();
						cout << endl;
					}

					cout << "ranges : " << ranges.size() << endl;
					cout << "intersections :" << endl;
					for(Point& intersection : intersections)
						intersection.print();
					cout << endl;
*/
				}
			}
		}
	}
}

//******************************************************************************
void Board::detect_colinear_edges() {
	for(size_t i = 0; i < edges.size(); ++i) {
		if(edges[i]->axis == Segment::Axis::DIAGONAL)
			continue;

		for(size_t j = i + 1; j < edges.size(); ++j) {
			if(edges[j]->axis != edges[i]->axis)
				continue;

			if(edges[i]->axis == Segment::Axis::H && edges[i]->p0().y == edges[j]->p0().y)
				conflict_manager.add_colinear_edges(edges[i], edges[j]);
			else if(edges[i]->axis == Segment::Axis::V && edges[i]->p0().x == edges[j]->p0().x)
				conflict_manager.add_colinear_edges(edges[i], edges[j]);
		}
	}
}

//******************************************************************************
void Board::detect_non_conflicting_edges() {
	for(Edge* edge : edges) {
		optional<MeshlinePolicy::Axis> axis = cast(edge->axis);
		if(axis && !edge->conflicts.size()) {
			edge->meshline_policy = line_policy_manager.add_meshline_policy(
				edge,
				axis.value(),
				MeshlinePolicy::Policy::THIRDS,
				edge->normal,
				coord(edge->p0(), axis.value()));
		}
	}
}

#ifdef DEBUG
//******************************************************************************
void Board::print() const {
	for(unique_ptr<Polygon> const& polygon : polygons)
		polygon->print();
}
#endif // DEBUG
