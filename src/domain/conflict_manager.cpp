///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "geometrics/edge.hpp"
#include "geometrics/polygon.hpp"
#include "mesh/meshline_policy.hpp"

#include "conflict_manager.hpp"

using namespace std;

//******************************************************************************
ConflictManager::ConflictManager(MeshlinePolicyManager* const line_policy_manager)
: line_policy_manager(line_policy_manager)
{}

/// @warning Allows geometrically inconsistent datas.
///*****************************************************************************
void ConflictManager::add_colinear_edges(Edge* a, Edge* b) {
	if((a->axis == Segment::Axis::H && b->axis == Segment::Axis::H)
	|| (a->axis == Segment::Axis::V && b->axis == Segment::Axis::V)) {
		bool does_conflict_exist = false;
		for(auto const& conflict : all_colinear_edges) {
			bool is_a_registered = false;
			bool is_b_registered = false;

			for(auto const* edge : conflict->edges) {
				if(edge == a) {
					is_a_registered = true;
					if(is_b_registered)
						break;
				} else if(edge == b) {
					is_b_registered = true;
					if(is_a_registered)
						break;
				}
			}

			if(is_a_registered && !is_b_registered) {
				does_conflict_exist = true;
				conflict->append(b);
				b->conflicts.push_back(conflict.get());
				break;
			} else if(!is_a_registered && is_b_registered) {
				does_conflict_exist = true;
				conflict->append(a);
				a->conflicts.push_back(conflict.get());
				break;
			} else if(is_a_registered && is_b_registered) {
				does_conflict_exist = true;
				break;
			}
		}

		if(!does_conflict_exist) {
			Conflict* conflict = all_colinear_edges.emplace_back(
				make_unique<ConflictColinearEdges>(a, b)).get();
			a->conflicts.push_back(conflict);
			b->conflicts.push_back(conflict);
		}
	}
}

/// @warning Allows geometrically inconsistent datas.
///*****************************************************************************
void ConflictManager::add_edge_in_polygon(Edge* a, Polygon* polygon, optional<Edge const*> b) {
	add_edge_in_polygon(a, polygon, Range(a->p0(), a->p1()), b);
}

/// @warning Allows geometrically inconsistent datas.
///*****************************************************************************
void ConflictManager::add_edge_in_polygon(Edge* a, Polygon* polygon, Range const range, optional<Edge const*> b) {
	bool does_conflict_exist = false;
	for(auto const& conflict : all_edge_in_polygons) {
		bool is_a_registered = false;
		bool is_polygon_registered = false;
		bool is_overlap_registered = false;

		if(conflict->edge == a) {
			is_a_registered = true;

			for(Overlap& overlap : conflict->overlaps) {
				if(get<POLYGON>(overlap) == polygon) {
					is_polygon_registered = true;
					if((get<RANGE>(overlap) == range)
					&& (!get<EDGE>(overlap) || (get<EDGE>(overlap) && get<EDGE>(overlap).value() == b))) {
						is_overlap_registered = true;
						break;
					}
				}
			}
		}

		if(is_a_registered && !is_overlap_registered) {
			does_conflict_exist = true;
			conflict->append(polygon, range, b);
//			b->conflicts.push_back(conflict.get()); // TODO needed?
			if(!is_polygon_registered)
				polygon->conflicts.push_back(conflict.get());
			break;
		} else if(is_a_registered && is_overlap_registered) {
			does_conflict_exist = true;
			break;
		}
	}

	if(!does_conflict_exist) {
		Conflict* conflict = all_edge_in_polygons.emplace_back(
			make_unique<ConflictEdgeInPolygon>(a, polygon, range, b)).get();
		a->conflicts.push_back(conflict);
//		b->conflicts.push_back(conflict); // TODO needed?
		polygon->conflicts.push_back(conflict);
	}
}

//******************************************************************************
ConflictTooCloseMeshlinePolicies* ConflictManager::add_too_close_meshline_policies(
		MeshlinePolicy* a,
		MeshlinePolicy* b) noexcept {

	if(a->axis != b->axis)
		return nullptr;

	for(auto const& conflict : all_too_close_meshline_policies)
		if(conflict->meshline_policies[0] == a
		|| conflict->meshline_policies[0] == b
		|| conflict->meshline_policies[1] == a
		|| conflict->meshline_policies[1] == b)
			return nullptr;

	auto conflict = all_too_close_meshline_policies.emplace_back(
		make_unique<ConflictTooCloseMeshlinePolicies>(a, b)).get();
	a->conflicts.push_back(conflict);
	b->conflicts.push_back(conflict);
	a->is_enabled = false;
	b->is_enabled = false;

	return conflict;
}

//******************************************************************************
void ConflictManager::auto_solve_all_edge_in_polygon() {
	for(auto const& conflict : all_edge_in_polygons)
		conflict->auto_solve(*line_policy_manager);
}

//******************************************************************************
void ConflictManager::auto_solve_all_colinear_edges() {
	for(auto const& conflict : all_colinear_edges)
		conflict->auto_solve(*line_policy_manager);
}

//******************************************************************************
vector<unique_ptr<ConflictColinearEdges>> const& ConflictManager::get_colinear_edges() const {
	return all_colinear_edges;
}

//******************************************************************************
vector<unique_ptr<ConflictEdgeInPolygon>> const& ConflictManager::get_edge_in_polygons() const {
	return all_edge_in_polygons;
}

//******************************************************************************
vector<unique_ptr<ConflictTooCloseMeshlinePolicies>> const& ConflictManager::get_too_close_meshline_policies() const {
	return all_too_close_meshline_policies;
}
