///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <algorithm>
#include <utility>

#include "utils/unreachable.hpp"
#include "utils/signum.hpp"
#include "utils/vector_utils.hpp"

#include "board.hpp"

namespace domain {

using namespace std;

/// @warning Undefined behavior if points are not all colinear.
///*****************************************************************************
void sort_points_by_vector_orientation(vector<Point>& points, Point const& vector) {
	int8_t const x_sign = signum(vector.x);
	int8_t const y_sign = signum(vector.y);
	if(x_sign > 0 && y_sign == 0)
		ranges::sort(points, [](Point const& a, Point const& b) {
			return (a.x < b.x);
		});
	else if(x_sign == 0 && y_sign > 0)
		ranges::sort(points, [](Point const& a, Point const& b) {
			return (a.y < b.y);
		});
	else if(x_sign < 0 && y_sign == 0)
		ranges::sort(points, [](Point const& a, Point const& b) {
			return (a.x > b.x);
		});
	else if(x_sign == 0 && y_sign < 0)
		ranges::sort(points, [](Point const& a, Point const& b) {
			return (a.y > b.y);
		});
	else if(x_sign > 0 && y_sign > 0)
		ranges::sort(points, [](Point const& a, Point const& b) {
			return (a.x < b.x && a.y < b.y);
		});
	else if(x_sign < 0 && y_sign > 0)
		ranges::sort(points, [](Point const& a, Point const& b) {
			return (a.x > b.x && a.y < b.y);
		});
	else if(x_sign < 0 && y_sign < 0)
		ranges::sort(points, [](Point const& a, Point const& b) {
			return (a.x > b.x && a.y > b.y);
		});
	else if(x_sign > 0 && y_sign < 0)
		ranges::sort(points, [](Point const& a, Point const& b) {
			return (a.x < b.x && a.y > b.y);
		});
}

//******************************************************************************
shared_ptr<Board> Board::Builder::build() {
	return make_shared<Board>(std::move(polygons), std::move(line_policies), Caretaker::singleton().get_history_root());
}

// TODO should be in meshline manager?
//******************************************************************************
void Board::Builder::add_fixed_meshline_policy(Axis const axis, Coord const coord) {
	if(!contains_that(line_policies[axis],
		[&coord](shared_ptr<MeshlinePolicy> const& policy) {
			if(policy->policy == MeshlinePolicy::Policy::ONELINE
			&& policy->normal == MeshlinePolicy::Normal::NONE
			&& policy->coord == coord)
				return true;
			return false;
		}))
			line_policies[axis].emplace_back(make_shared<MeshlinePolicy>(
				axis,
				MeshlinePolicy::Policy::ONELINE,
				MeshlinePolicy::Normal::NONE,
				params,
				coord,
				Caretaker::singleton().get_history_root()));
}

//******************************************************************************
void Board::Builder::add_polygon(Plane const plane, Polygon::Type const type, string const& name, initializer_list<Point> points) {
	polygons[plane].push_back(make_shared<Polygon>(plane, type, name, from_init_list(points), Caretaker::singleton().get_history_root()));
}

//******************************************************************************
void Board::Builder::add_polygon(Plane const plane, Polygon::Type const type, string const& name, vector<unique_ptr<Point const>>&& points) {
	polygons[plane].push_back(make_shared<Polygon>(plane, type, name, std::move(points), Caretaker::singleton().get_history_root()));
}

//******************************************************************************
void Board::Builder::add_polygon_from_box(Plane const plane, Polygon::Type const type, string const& name, Point const p1, Point const p3) {
	vector<unique_ptr<Point const>> points(4);
	points[0] = make_unique<Point const>(p1.x, p1.y);
	points[1] = make_unique<Point const>(p1.x, p3.y);
	points[2] = make_unique<Point const>(p3.x, p3.y);
	points[3] = make_unique<Point const>(p3.x, p1.y);

	polygons[plane].push_back(make_shared<Polygon>(plane, type, name, std::move(points), Caretaker::singleton().get_history_root()));
}

//******************************************************************************
BoardState::BoardState(PlaneSpace<vector<shared_ptr<Polygon>>>&& polygons)
: polygons(std::move(polygons)) {
	for(auto const& plane : AllPlane) {
		for(auto const& polygon : this->polygons[plane])
			for(auto const& edge : polygon->edges)
				edges[plane].push_back(edge.get());

		this->polygons[plane].shrink_to_fit();
		edges[plane].shrink_to_fit();
	}
}

//******************************************************************************
Board::Board(PlaneSpace<vector<shared_ptr<Polygon>>>&& polygons, Timepoint* t)
: Originator(t, BoardState(std::move(polygons)))
, conflict_manager(make_shared<ConflictManager>(t))
, line_policy_manager(make_shared<MeshlinePolicyManager>(params, t)) {

	conflict_manager->init(line_policy_manager.get());
	line_policy_manager->init(conflict_manager.get());
	Caretaker::singleton().take_care_of(conflict_manager);
	Caretaker::singleton().take_care_of(line_policy_manager);
	for(auto const& plane : AllPlane)
		for(auto const& polygon : get_current_state().polygons[plane])
			Caretaker::singleton().take_care_of(polygon);
}

//******************************************************************************
Board::Board(
	PlaneSpace<std::vector<std::shared_ptr<Polygon>>>&& polygons,
	AxisSpace<std::vector<std::shared_ptr<MeshlinePolicy>>>&& line_policies,
	Timepoint* t)
: Originator(t, BoardState(std::move(polygons)))
, conflict_manager(make_shared<ConflictManager>(t))
, line_policy_manager(make_shared<MeshlinePolicyManager>(params, std::move(line_policies), t)) {

	conflict_manager->init(line_policy_manager.get());
	line_policy_manager->init(conflict_manager.get());
	Caretaker::singleton().take_care_of(conflict_manager);
	Caretaker::singleton().take_care_of(line_policy_manager);
}

/// Detect all EDGE_IN_POLYGON. Will also detect some COLINEAR_EDGES.
/// Overlapping edges should be EDGE_IN_POLYGON and not COLINEAR_EDGES.
///*****************************************************************************
void Board::detect_edges_in_polygons(Plane const plane) {
	for(auto const& poly_a : get_current_state().polygons[plane]) {
		for(auto const& poly_b : get_current_state().polygons[plane]) {
			if(poly_b == poly_a)
				continue;

			for(auto const& edge_a : poly_a->edges) {

				struct RangeBtwIntersections {
					Range const range;
					optional<relation::PolygonPoint> rel_to_poly_b;
					optional<Point> const mid;
					explicit RangeBtwIntersections(Range const& _range)
					: range(_range)
					, rel_to_poly_b(nullopt)
					, mid(domain::mid(_range))
					{}
					RangeBtwIntersections(Range const& _range, relation::PolygonPoint _rel_to_poly_b)
					: range(_range)
					, rel_to_poly_b(_rel_to_poly_b)
					, mid(nullopt)
					{}
				};

				vector<Point> intersections;
				vector<RangeBtwIntersections> ranges;

				for(auto const& edge_b : poly_b->edges) {
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
							conflict_manager->add_edge_in_polygon(edge_a.get(), poly_b.get(), r.value(), edge_b.get());
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
					conflict_manager->add_edge_in_polygon(edge_a.get(), poly_b.get());
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

						ranges.emplace_back(std::move(current_range));
					}

					for(RangeBtwIntersections const& range : ranges) {
						if(range.mid.has_value()
						&& poly_b->relation_to(range.mid.value()) == relation::PolygonPoint::IN) {
/*						|| poly_b->relation_to(&range.mid.value()) == relation::PolygonPoint::ON))
*/							conflict_manager->add_edge_in_polygon(edge_a.get(), poly_b.get(), range.range);
//							range.rel_to_poly_b = poly_b->relation_to(&range.mid.value()); //TODO useless
						} else if(range.rel_to_poly_b == relation::PolygonPoint::IN) {
							conflict_manager->add_edge_in_polygon(edge_a.get(), poly_b.get(), range.range);
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
void Board::detect_colinear_edges(Plane const plane) {
	auto const& s = get_current_state();

	for(size_t i = 0; i < s.edges[plane].size(); ++i) {
		if(s.edges[plane][i]->axis == Segment::Axis::DIAGONAL)
//		|| !edges[i]->to_mesh)
			continue;

		for(size_t j = i + 1; j < s.edges[plane].size(); ++j) {
			if(s.edges[plane][j]->axis != s.edges[plane][i]->axis)
				continue;

			optional<Axis> const axis = transpose(plane, s.edges[plane][i]->axis);

			if(axis) {
				if(s.edges[plane][i]->axis == Segment::Axis::H && s.edges[plane][i]->p0().y == s.edges[plane][j]->p0().y)
					conflict_manager->add_colinear_edges(s.edges[plane][i], s.edges[plane][j]);
				else if(s.edges[plane][i]->axis == Segment::Axis::V && s.edges[plane][i]->p0().x == s.edges[plane][j]->p0().x)
					conflict_manager->add_colinear_edges(s.edges[plane][i], s.edges[plane][j]);
			}
		}
	}
}

//******************************************************************************
void Board::detect_non_conflicting_edges(Plane const plane) {
	for(Edge* edge : get_current_state().edges[plane]) {
		optional<Coord> const coord = domain::coord(edge->p0(), edge->axis);
		optional<Axis> const axis = transpose(plane, edge->axis);
		optional<MeshlinePolicy::Normal> const normal = cast(edge->normal);
		if(coord && axis && normal && !edge->get_current_state().conflicts.size()) {
			auto [t, state_e] = edge->make_next_state();
			state_e.meshline_policy = line_policy_manager->add_meshline_policy(
				edge,
				axis.value(),
				MeshlinePolicy::Policy::THIRDS,
				normal.value(),
				coord.value(),
				true,
				t);
			edge->set_state(t, state_e);
		}
	}
}

//******************************************************************************
void Board::auto_mesh() {
	for(auto const& plane : AllPlane)
		detect_edges_in_polygons(plane);

	for(auto const& plane : AllPlane)
		detect_colinear_edges(plane);

	for(auto const& plane : AllPlane)
		detect_non_conflicting_edges(plane);

	for(auto const& plane : AllPlane)
		conflict_manager->auto_solve_all_edge_in_polygon(plane);

	for(auto const& axis : AllAxis)
		conflict_manager->auto_solve_all_colinear_edges(axis);

	for(auto const& axis : AllAxis)
		line_policy_manager->detect_and_solve_too_close_meshline_policies(axis);

	for(auto const& axis : AllAxis)
		line_policy_manager->detect_intervals(axis);

	for(auto const& axis : AllAxis)
		line_policy_manager->mesh(axis);
}


//******************************************************************************
vector<shared_ptr<Meshline>> Board::get_meshline_policies_meshlines(Axis axis) const {
	return line_policy_manager->get_meshline_policies_meshlines(axis);
}

//******************************************************************************
vector<shared_ptr<Meshline>> const& Board::get_meshlines(Axis axis) const {
	return line_policy_manager->get_meshlines(axis);
}

//******************************************************************************
vector<shared_ptr<MeshlinePolicy>> const& Board::get_meshline_policies(Axis axis) const {
	return line_policy_manager->get_meshline_policies(axis);
}

//******************************************************************************
vector<shared_ptr<Interval>> const& Board::get_intervals(Axis axis) const {
	return line_policy_manager->get_intervals(axis);
}

//******************************************************************************
vector<shared_ptr<Polygon>> const& Board::get_polygons(Plane plane) const {
	return get_current_state().polygons[plane];
}

//******************************************************************************
vector<shared_ptr<ConflictEdgeInPolygon>> const& Board::get_conflicts_edge_in_polygons(Plane const plane) const {
	return conflict_manager->get_edge_in_polygons(plane);
}

//******************************************************************************
vector<shared_ptr<ConflictColinearEdges>> const& Board::get_conflicts_colinear_edges(Axis const axis) const {
	return conflict_manager->get_colinear_edges(axis);
}

//******************************************************************************
vector<shared_ptr<ConflictTooCloseMeshlinePolicies>> const& Board::get_conflicts_too_close_meshline_policies(Axis const axis) const {
	return conflict_manager->get_too_close_meshline_policies(axis);
}

} // namespace domain
