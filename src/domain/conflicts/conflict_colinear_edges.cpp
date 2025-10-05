///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "domain/geometrics/edge.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "domain/meshline_policy_manager.hpp"
#include "utils/unreachable.hpp"

#include "conflict_colinear_edges.hpp"

namespace domain {

using namespace std;

//******************************************************************************
ConflictColinearEdges::ConflictColinearEdges(Axis axis, Edge* a, Edge* b, Timepoint* t)
: Originator(t, { .edges = { a, b } })
, Conflict(Kind::COLINEAR_EDGES)
, axis(axis)
{}

//******************************************************************************
void ConflictColinearEdges::append(Edge* edge, Timepoint* t) {
	auto state = get_current_state();
	state.edges.push_back(edge);
	set_given_or_next_state(state, t);
}

//******************************************************************************
void ConflictColinearEdges::auto_solve(MeshlinePolicyManager& line_policy_manager) {
	size_t normal_min = 0;
	size_t normal_max = 0;

	auto const& s = get_current_state();

	for(Edge const* edge : s.edges) {
		if(!edge->get_current_state().to_mesh)
			continue;

		Normal const normal = edge->get_current_state().to_reverse
		                    ? reverse(edge->normal)
		                    : edge->normal;

		switch(normal) {
		case Normal::XMIN:
		case Normal::YMIN:
		case Normal::ZMIN:
			++normal_min;
			break;
		case Normal::XMAX:
		case Normal::YMAX:
		case Normal::ZMAX:
			++normal_max;
			break;
		default:
			break;
		}
	}

	optional<Coord> const coord = domain::coord(s.edges.front()->p0(), s.edges.front()->axis);

	auto const add_policy = [&, this](MeshlinePolicy::Policy policy, MeshlinePolicy::Normal normal) {
		auto [t, state] = this->make_next_state();
		state.meshline_policy = line_policy_manager.add_meshline_policy(
			{ this },
			axis,
			policy,
			normal,
			coord.value(),
			true,
			t);
		state.solution = state.meshline_policy;
		state.is_solved = true;
		this->set_state(t, state);
	};

	if(coord) {
		if(normal_min && normal_max) {
			add_policy(
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE);
		} else if(normal_min && !normal_max) {
			add_policy(
				MeshlinePolicy::Policy::THIRDS,
				MeshlinePolicy::Normal::MIN);
		} else if(!normal_min && normal_max) {
			add_policy(
				MeshlinePolicy::Policy::THIRDS,
				MeshlinePolicy::Normal::MAX);
		}
	}
}

} // namespace domain
