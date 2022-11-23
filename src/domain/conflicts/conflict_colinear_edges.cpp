///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "domain/geometrics/edge.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "domain/meshline_policy_manager.hpp"
#include "utils/entity.hpp"
#include "utils/unreachable.hpp"

#include "conflict_colinear_edges.hpp"

using namespace std;

//******************************************************************************
ConflictColinearEdges::ConflictColinearEdges(Axis axis, Edge* a, Edge* b)
: Conflict(Kind::COLINEAR_EDGES)
, axis(axis)
, edges({ a, b })
{}

//******************************************************************************
void ConflictColinearEdges::append(Edge* edge) {
	edges.push_back(edge);
}

//******************************************************************************
void ConflictColinearEdges::auto_solve(MeshlinePolicyManager& line_policy_manager) {
	size_t normal_min = 0;
	size_t normal_max = 0;

	for(Edge* edge : edges) {
		if(!edge->to_mesh)
			continue;

		switch(edge->normal) {
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

	optional<Coord> const coord = ::coord(edges.front()->p0(), edges.front()->axis);

	if(coord) {
		if(normal_min && normal_max) {
			meshline_policy = line_policy_manager.add_meshline_policy(
				this,
				axis,
				MeshlinePolicy::Policy::HALFS,
				MeshlinePolicy::Normal::NONE,
				coord.value());
			solution = meshline_policy;
			is_solved = true;
		} else if(normal_min && !normal_max) {
			meshline_policy = line_policy_manager.add_meshline_policy(
				this,
				axis,
				MeshlinePolicy::Policy::THIRDS,
				MeshlinePolicy::Normal::MIN,
				coord.value());
			solution = meshline_policy;
			is_solved = true;
		} else if(!normal_min && normal_max) {
			meshline_policy = line_policy_manager.add_meshline_policy(
				this,
				axis,
				MeshlinePolicy::Policy::THIRDS,
				MeshlinePolicy::Normal::MAX,
				coord.value());
			solution = meshline_policy;
			is_solved = true;
		}
	}
}
