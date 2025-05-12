///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "geometrics/edge.hpp"
#include "geometrics/polygon.hpp"
#include "mesh/meshline_policy.hpp"

#include "conflict_manager.hpp"

namespace domain {

using namespace std;

//******************************************************************************
ConflictManager::ConflictManager(Timepoint* t)
: Originator(t)
, line_policy_manager(nullptr)
{}

//******************************************************************************
void ConflictManager::init(MeshlinePolicyManager* line_policy_manager) {
	this->line_policy_manager = line_policy_manager;
}

/// @warning Allows geometrically inconsistent datas.
///*****************************************************************************
void ConflictManager::add_colinear_edges(Edge* a, Edge* b) {
	if(a->plane == b->plane && a->axis == b->axis) {
		auto const axis = transpose(a->plane, a->axis);
		if(!axis.has_value())
			return;

		bool does_conflict_exist = false;
		for(auto const& conflict : get_current_state().all_colinear_edges[axis.value()]) {
			bool is_a_registered = false;
			bool is_b_registered = false;

			for(auto const* edge : conflict->get_current_state().edges) {
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
				auto* t = next_timepoint();
				conflict->append(b, t);
				auto state_b = b->get_current_state();
				state_b.conflicts.push_back(conflict.get());
				b->set_state(t, state_b);
				break;
			} else if(!is_a_registered && is_b_registered) {
				does_conflict_exist = true;
				auto* t = next_timepoint();
				conflict->append(a, t);
				auto state_a = a->get_current_state();
				state_a.conflicts.push_back(conflict.get());
				a->set_state(t, state_a);
				break;
			} else if(is_a_registered && is_b_registered) {
				does_conflict_exist = true;
				break;
			}
		}

		if(!does_conflict_exist) {
			auto [t, state] = make_next_state();
			auto state_a = a->get_current_state();
			auto state_b = b->get_current_state();

			auto const& conflict = state.all_colinear_edges[axis.value()].emplace_back(
				make_shared<ConflictColinearEdges>(axis.value(), a, b, t));
			get_caretaker().take_care_of(conflict);
			state_a.conflicts.push_back(conflict.get());
			state_b.conflicts.push_back(conflict.get());

			a->set_state(t, state_a);
			b->set_state(t, state_b);
			set_state(t, state);
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
	if(a->plane != polygon->plane
	&& (!b.has_value() || (*b)->plane != a->plane))
		return;

	Plane const plane = a->plane;

	bool does_conflict_exist = false;
	for(auto const& conflict : get_current_state().all_edge_in_polygons[plane]) {
		bool is_a_registered = false;
		bool is_polygon_registered = false;
		bool is_overlap_registered = false;

		if(conflict->edge == a) {
			is_a_registered = true;

			for(Overlap const& overlap : conflict->get_current_state().overlaps) {
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
			auto* t = next_timepoint();
			conflict->append(polygon, range, b, t);
//			b->conflicts.push_back(conflict.get()); // TODO needed?
			if(!is_polygon_registered) {
				auto state_p = polygon->get_current_state();
				state_p.conflicts.push_back(conflict.get());
				polygon->set_state(t, state_p);
			}
			break;
		} else if(is_a_registered && is_overlap_registered) {
			does_conflict_exist = true;
			break;
		}
	}

	if(!does_conflict_exist) {
		auto [t, state] = make_next_state();
		auto state_a = a->get_current_state();
		auto state_p = polygon->get_current_state();

		auto const& conflict = state.all_edge_in_polygons[plane].emplace_back(
			make_shared<ConflictEdgeInPolygon>(plane, a, polygon, range, b, t));
		get_caretaker().take_care_of(conflict);
		state_a.conflicts.push_back(conflict.get());
//		b->conflicts.push_back(conflict); // TODO needed?
		state_p.conflicts.push_back(conflict.get());

		a->set_state(t, state_a);
		polygon->set_state(t, state_p);
		set_state(t, state);
	}
}

//******************************************************************************
ConflictTooCloseMeshlinePolicies* ConflictManager::add_too_close_meshline_policies(
		MeshlinePolicy* a,
		MeshlinePolicy* b) noexcept {

	if(a->axis != b->axis)
		return nullptr;

	Axis const axis = a->axis;

	for(auto const& conflict : get_current_state().all_too_close_meshline_policies[axis])
		if(conflict->meshline_policies[0] == a
		|| conflict->meshline_policies[0] == b
		|| conflict->meshline_policies[1] == a
		|| conflict->meshline_policies[1] == b)
			return nullptr;

	auto [t, state] = make_next_state();
	auto state_a = a->get_current_state();
	auto state_b = b->get_current_state();

	auto const& conflict = state.all_too_close_meshline_policies[axis].emplace_back(
		make_shared<ConflictTooCloseMeshlinePolicies>(axis, a, b, t));
	get_caretaker().take_care_of(conflict);
	state_a.conflicts.push_back(conflict.get());
	state_b.conflicts.push_back(conflict.get());
	state_a.is_enabled = false;
	state_b.is_enabled = false;

	a->set_state(t, state_a);
	b->set_state(t, state_b);
	set_state(t, state);

	return conflict.get();
}

//******************************************************************************
void ConflictManager::auto_solve_all_edge_in_polygon(Plane const plane) {
	for(auto const& conflict : get_current_state().all_edge_in_polygons[plane])
		conflict->auto_solve(*line_policy_manager);
}

//******************************************************************************
void ConflictManager::auto_solve_all_colinear_edges(Axis const axis) {
	for(auto const& conflict : get_current_state().all_colinear_edges[axis])
		conflict->auto_solve(*line_policy_manager);
}

//******************************************************************************
vector<shared_ptr<ConflictColinearEdges>> const& ConflictManager::get_colinear_edges(Axis const axis) const {
	return get_current_state().all_colinear_edges[axis];
}

//******************************************************************************
vector<shared_ptr<ConflictEdgeInPolygon>> const& ConflictManager::get_edge_in_polygons(Plane const plane) const {
	return get_current_state().all_edge_in_polygons[plane];
}

//******************************************************************************
vector<shared_ptr<ConflictTooCloseMeshlinePolicies>> const& ConflictManager::get_too_close_meshline_policies(Axis const axis) const {
	return get_current_state().all_too_close_meshline_policies[axis];
}

} // namespace domain
