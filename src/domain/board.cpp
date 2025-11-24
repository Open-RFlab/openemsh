///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <algorithm>
#include <utility>

#include "infra/utils/to_string.hpp"
#include "utils/progress.hpp"
#include "utils/signum.hpp"
#include "utils/unreachable.hpp"
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
shared_ptr<Board> Board::Builder::build(Params&& params) {
	return make_shared<Board>(
		std::move(polygons),
		std::move(fixed_meshline_policy_creators),
		std::move(params),
		Caretaker::singleton().get_history_root());
}

// TODO should be in meshline manager?
//******************************************************************************
void Board::Builder::add_fixed_meshline_policy(Axis const axis, Coord const coord) {
	fixed_meshline_policy_creators[axis].emplace_back([=](Board const* board, Timepoint* t) {
		if(!contains_that(board->line_policy_manager->get_current_state().line_policies[axis],
			[&coord](shared_ptr<MeshlinePolicy> const& policy) {
				if(policy->get_current_state().policy == MeshlinePolicy::Policy::ONELINE
				&& policy->get_current_state().normal == MeshlinePolicy::Normal::NONE
				&& policy->coord == coord)
					return true;
				return false;
			}))
				board->line_policy_manager->add_meshline_policy(
					{},
					axis,
					MeshlinePolicy::Policy::ONELINE,
					MeshlinePolicy::Normal::NONE,
					coord,
					t);
	});
}

//******************************************************************************
void Board::Builder::add_polygon(Plane plane, shared_ptr<Material> const& material, string const& name, size_t priority, Polygon::RangeZ const& z_placement, initializer_list<Point> points) {
	polygons[plane].push_back(make_shared<Polygon>(plane, material, name, priority, z_placement, from_init_list(points), Caretaker::singleton().get_history_root()));
}

//******************************************************************************
void Board::Builder::add_polygon(Plane plane, shared_ptr<Material> const& material, string const& name, size_t priority, Polygon::RangeZ const& z_placement, vector<unique_ptr<Point const>>&& points) {
	polygons[plane].push_back(make_shared<Polygon>(plane, material, name, priority, z_placement, std::move(points), Caretaker::singleton().get_history_root()));
}

//******************************************************************************
void Board::Builder::add_polygon_from_box(Plane plane, shared_ptr<Material> const& material, string const& name, size_t priority, Polygon::RangeZ const& z_placement, Point const p1, Point const p3) {
	vector<unique_ptr<Point const>> points(4);
	points[0] = make_unique<Point const>(p1.x, p1.y);
	points[1] = make_unique<Point const>(p1.x, p3.y);
	points[2] = make_unique<Point const>(p3.x, p3.y);
	points[3] = make_unique<Point const>(p3.x, p1.y);

	polygons[plane].push_back(make_shared<Polygon>(plane, material, name, priority, z_placement, std::move(points), Caretaker::singleton().get_history_root()));
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
Board::Board(PlaneSpace<vector<shared_ptr<Polygon>>>&& polygons, Params&& params, Timepoint* t)
: Originator(t, BoardState(std::move(polygons)))
, global_params(make_shared<GlobalParams>(std::move(params), t))
, conflict_manager(make_shared<ConflictManager>(t))
, line_policy_manager(make_shared<MeshlinePolicyManager>(global_params.get(), t)) {

	conflict_manager->init(line_policy_manager.get());
	line_policy_manager->init(conflict_manager.get());
	get_caretaker().take_care_of(conflict_manager);
	get_caretaker().take_care_of(line_policy_manager);
	get_caretaker().take_care_of(global_params);
	for(auto const& plane : AllPlane)
		for(auto const& polygon : get_current_state().polygons[plane])
			get_caretaker().take_care_of(polygon);
}

//******************************************************************************
Board::Board(
	PlaneSpace<std::vector<std::shared_ptr<Polygon>>>&& polygons,
	AxisSpace<std::vector<std::function<void (Board*, Timepoint*)>>>&& fixed_meshline_policy_creators,
	Params&& params,
	Timepoint* t)
: Originator(t, BoardState(std::move(polygons)))
, global_params(make_shared<GlobalParams>(std::move(params), t))
, conflict_manager(make_shared<ConflictManager>(t))
, line_policy_manager(make_shared<MeshlinePolicyManager>(global_params.get(), t))
, fixed_meshline_policy_creators(std::move(fixed_meshline_policy_creators)) {

	conflict_manager->init(line_policy_manager.get());
	line_policy_manager->init(conflict_manager.get());
	get_caretaker().take_care_of(conflict_manager);
	get_caretaker().take_care_of(line_policy_manager);
	get_caretaker().take_care_of(global_params);
	for(auto const& plane : AllPlane)
		for(auto const& polygon : get_current_state().polygons[plane])
			get_caretaker().take_care_of(polygon);
}

//******************************************************************************
shared_ptr<Material> Board::find_ambient_material(Plane plane, Segment const& segment) const {
	return find_ambient_material(plane, segment, nullptr).first;
}

/// Check among all Polygons that match segment and current_polygon->z_placement.
/// Choose Material following this rule: CONDUCTOR>DIELECTRIC>AIR
///*****************************************************************************
pair<shared_ptr<Material>, remove_const_t<decltype(Polygon::priority)>> Board::find_ambient_material(Plane plane, Segment const& segment, shared_ptr<Polygon> const& current_polygon) const {
	Bounding2D const segment_bounding = bounding(segment);

	vector<pair<shared_ptr<Material>, remove_const_t<decltype(Polygon::priority)>>> materials;
	for(shared_ptr<Polygon> const& polygon : get_current_state().polygons[plane]) {
		if(polygon->material
		&& (!current_polygon // Bypass self & z_overlap checks if not relevant.
		   || (polygon != current_polygon
		   && does_overlap(polygon->z_placement, current_polygon->z_placement)))
		&& does_overlap(polygon->bounding, segment_bounding))
			materials.emplace_back(shared_ptr<Material>(polygon->material), polygon->priority);
	}

	ranges::sort(materials, [](auto const& a, auto const& b) {
		auto const& [material_a, priority_a] = a;
		auto const& [material_b, priority_b] = b;
		return priority_a != priority_b
		     ? priority_a < priority_b
		     : *material_a < *material_b;
	});

	if(!materials.empty())
		return materials.back();
	else
		// TODO if no material return Board::background_material
		return {};
}

//******************************************************************************
void Board::adjust_edges_to_materials(Plane const plane) {
	auto [bar, found, i] = Progress::Bar::build(
		get_current_state().edges[plane].size(),
		"["s + to_string(plane) + "] Adjusting Edges to Materials ");

	for(shared_ptr<Polygon> const& polygon : get_current_state().polygons[plane]) {
		for(shared_ptr<Edge> const& edge : polygon->edges) {
			auto const& inner_material = polygon->material;
			auto const& [immediate_ambient_outer_material, outer_priority] = [&]() -> pair<shared_ptr<Material>, remove_const_t<decltype(Polygon::priority)>> {
				Point const translate_x(2 * equality_tolerance, 0);
				Point const translate_y(0, 2 * equality_tolerance);
				switch(edge->normal) {
				case Normal::NONE:
					return {};
				case Normal::XMIN:
					return find_ambient_material(plane, Range(edge->p0() - translate_x, edge->p1() - translate_x), polygon);
				case Normal::XMAX:
					return find_ambient_material(plane, Range(edge->p0() + translate_x, edge->p1() + translate_x), polygon);
				case Normal::YMIN:
					return find_ambient_material(plane, Range(edge->p0() - translate_y, edge->p1() - translate_y), polygon);
				case Normal::YMAX:
					return find_ambient_material(plane, Range(edge->p0() + translate_y, edge->p1() + translate_y), polygon);
				default:
					::unreachable();
				}
			} ();

			if(immediate_ambient_outer_material
			&& *immediate_ambient_outer_material > *inner_material) {
				++found;
				auto state = edge->get_current_state();
				state.to_reverse = true;
				edge->set_next_state(state);
			}
			bar.tick(found, ++i);
		}
	}
	bar.complete();
}

/// Detect all EDGE_IN_POLYGON. Will also detect some COLINEAR_EDGES.
/// Overlapping edges should be EDGE_IN_POLYGON and not COLINEAR_EDGES.
///*****************************************************************************
void Board::detect_edges_in_polygons(Plane const plane) {
	auto [bar, found, k] = Progress::Bar::build(
		get_current_state().polygons[plane].size() * get_current_state().polygons[plane].size(),
		"["s + to_string(plane) + "] Detecting EDGES_IN_POLYGON conflicts ");

	for(auto const& poly_a : get_current_state().polygons[plane]) {
		for(auto const& poly_b : get_current_state().polygons[plane]) {
			++k;

			if(poly_b == poly_a)
				continue;

			// Same priority : no reason to dismiss one.
			if(poly_a->priority > poly_b->priority)
				continue;

			if(!does_overlap(poly_b->z_placement, poly_a->z_placement))
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
							++found;
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
					++found;
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
*/							++found;
							conflict_manager->add_edge_in_polygon(edge_a.get(), poly_b.get(), range.range);
//							range.rel_to_poly_b = poly_b->relation_to(&range.mid.value()); //TODO useless
						} else if(range.rel_to_poly_b == relation::PolygonPoint::IN) {
							++found;
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
		bar.tick(found, k);
	}
	bar.complete();
}

//******************************************************************************
void Board::detect_colinear_edges(Plane const plane) {
	auto const& s = get_current_state();

	auto [bar, found, k] = Progress::Bar::build(
		((s.edges[plane].size() * s.edges[plane].size()) - s.edges[plane].size()) / 2, // AB/BA deduplicated, AA dismissed.
		"["s + to_string(plane) + "] Detecting COLINEAR_EDGES conflicts ");

	for(size_t i = 0; i < s.edges[plane].size(); ++i) {
		if(s.edges[plane][i]->axis == Segment::Axis::DIAGONAL)
//		|| !edges[i]->to_mesh)
			continue;

		for(size_t j = i + 1; j < s.edges[plane].size(); ++j, ++k) {
			if(s.edges[plane][j]->axis != s.edges[plane][i]->axis)
				continue;

			optional<Axis> const axis = transpose(plane, s.edges[plane][i]->axis);

			if(axis) {
				if(s.edges[plane][i]->axis == Segment::Axis::H && s.edges[plane][i]->p0().y == s.edges[plane][j]->p0().y) {
					conflict_manager->add_colinear_edges(s.edges[plane][i], s.edges[plane][j]);
					++found;
				} else if(s.edges[plane][i]->axis == Segment::Axis::V && s.edges[plane][i]->p0().x == s.edges[plane][j]->p0().x) {
					conflict_manager->add_colinear_edges(s.edges[plane][i], s.edges[plane][j]);
					++found;
				}
			}
		}
		bar.tick(found, k);
	}
	bar.complete();
}

//******************************************************************************
void Board::detect_individual_edges(Plane const plane) {
	auto [bar, found, i] = Progress::Bar::build(
		get_current_state().edges[plane].size(),
		"["s + to_string(plane) + "] Adding fixed meshline policies ");

	for(Edge* edge : get_current_state().edges[plane]) {
		optional<Coord> const coord = domain::coord(edge->p0(), edge->axis);
		optional<Axis> const axis = transpose(plane, edge->axis);
		Normal const normal = edge->get_current_state().to_reverse
		                    ? reverse(edge->normal)
		                    : edge->normal;
		if(coord && axis
		&& (edge->get_current_state().conflicts.empty()
		   || ranges::none_of(edge->get_current_state().conflicts, [](auto const& conflict) { return conflict ? conflict->kind == Conflict::Kind::COLINEAR_EDGES : false; }))
		&& edge->get_current_state().to_mesh) {
			++found;
			auto [t, state_e] = edge->make_next_state();
			state_e.meshline_policy = line_policy_manager->add_meshline_policy(
				{ edge },
				axis.value(),
				MeshlinePolicy::Policy::THIRDS,
				cast(normal),
				coord.value(),
				state_e.to_mesh,
				t);
			edge->set_state(t, state_e);
		}
		bar.tick(found, ++i);
	}
	bar.complete();
}

//******************************************************************************
void Board::add_fixed_meshline_policies(Axis axis) {
	auto [bar, i, _] = Progress::Bar::build(
		fixed_meshline_policy_creators[axis].size(),
		"["s + to_string(axis) + "] Adding fixed meshline policies ");

	auto* t = next_timepoint();
	for(auto const& create_meshline_policy : fixed_meshline_policy_creators[axis]) {
		create_meshline_policy(this, t);
		bar.tick(++i);
	}
	bar.complete();
}

//******************************************************************************
void Board::adjust_edges_to_materials() {
	for(auto const& plane : AllPlane)
		adjust_edges_to_materials(plane);
}

//******************************************************************************
void Board::detect_edges_in_polygons() {
	for(auto const& plane : AllPlane)
		detect_edges_in_polygons(plane);
}

//******************************************************************************
void Board::detect_colinear_edges() {
	for(auto const& plane : AllPlane)
		detect_colinear_edges(plane);
}

//******************************************************************************
void Board::detect_individual_edges() {
	for(auto const& plane : AllPlane)
		detect_individual_edges(plane);
}

//******************************************************************************
void Board::add_fixed_meshline_policies() {
#warning TODO
//	for(auto const& axis : AllAxis)
	for(auto const& axis : { X, Y })
		add_fixed_meshline_policies(axis);
}

//******************************************************************************
void Board::auto_solve_all_edge_in_polygon() {
	for(auto const& plane : AllPlane)
		conflict_manager->auto_solve_all_edge_in_polygon(plane);
}

//******************************************************************************
void Board::auto_solve_all_colinear_edges() {
	for(auto const& axis : AllAxis)
		conflict_manager->auto_solve_all_colinear_edges(axis);
}

//******************************************************************************
void Board::detect_and_solve_too_close_meshline_policies() {
	for(auto const& axis : AllAxis)
		line_policy_manager->detect_and_solve_too_close_meshline_policies(axis);
}

//******************************************************************************
void Board::detect_intervals() {
	for(auto const& axis : AllAxis)
		line_policy_manager->detect_intervals(axis);
}

//******************************************************************************
void Board::mesh() {
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

//******************************************************************************
size_t Board::get_mesh_cell_number() const {
	return line_policy_manager->get_mesh_cell_number();
}


} // namespace domain
