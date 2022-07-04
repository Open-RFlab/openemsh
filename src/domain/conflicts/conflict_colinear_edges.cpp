///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "domain/geometrics/edge.hpp"
#include "domain/meshline_policy_manager.hpp"

#include "conflict_colinear_edges.hpp"

using namespace std;

//******************************************************************************
ConflictColinearEdges::ConflictColinearEdges(Edge* a, Edge* b)
: Conflict(Kind::COLINEAR_EDGES)
, edges({ a, b })
{}

//******************************************************************************
void ConflictColinearEdges::append(Edge* edge) {
	edges.push_back(edge);
}

//******************************************************************************
void ConflictColinearEdges::auto_solve(MeshlinePolicyManager& line_policy_manager) {
	if(optional<MeshlinePolicy::Axis> axis = cast(edges.front()->axis)) {
		size_t normal_min = 0;
		size_t normal_max = 0;

		for(Edge* edge : edges) {
			if(!edge->to_mesh)
				continue;

			switch(axis.value()) {
			case MeshlinePolicy::Axis::H:
				if(edge->normal == Normal::YMIN)
					++normal_min;
				else if(edge->normal == Normal::YMAX)
					++normal_max;
				break;
			case MeshlinePolicy::Axis::V:
				if(edge->normal == Normal::XMIN)
					++normal_min;
				else if(edge->normal == Normal::XMAX)
					++normal_max;
				break;
			}
		}

		if(normal_min && normal_max) {
			meshline_policy = line_policy_manager.add_meshline_policy(
				this,
				axis.value(),
				MeshlinePolicy::Policy::HALFS,
				Normal::NONE,
				coord(edges.front()->p0(), axis.value()));
			solution = meshline_policy;
			is_solved = true;
		} else if(normal_min && !normal_max) {
			Normal normal = [&] {
				switch(axis.value()) {
				case MeshlinePolicy::Axis::H: return Normal::YMIN;
				case MeshlinePolicy::Axis::V: return Normal::XMIN;
				}
			} ();
			meshline_policy = line_policy_manager.add_meshline_policy(
				this,
				axis.value(),
				MeshlinePolicy::Policy::THIRDS,
				normal,
				coord(edges.front()->p0(), axis.value()));
			solution = meshline_policy;
			is_solved = true;
		} else if(!normal_min && normal_max) {
			Normal normal = [&] {
				switch(axis.value()) {
				case MeshlinePolicy::Axis::H: return Normal::YMAX;
				case MeshlinePolicy::Axis::V: return Normal::XMAX;
				}
			} ();
			meshline_policy = line_policy_manager.add_meshline_policy(
				this,
				axis.value(),
				MeshlinePolicy::Policy::THIRDS,
				normal,
				coord(edges.front()->p0(), axis.value()));
			solution = meshline_policy;
			is_solved = true;
		}
	}
}
