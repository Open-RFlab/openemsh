///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <optional>
#include <tuple>

#include "domain/mesh/meshline_policy.hpp"
#include "domain/meshline_policy_manager.hpp"

#include "conflict_too_close_meshline_policies.hpp"

using namespace std;

//******************************************************************************
ConflictTooCloseMeshlinePolicies::ConflictTooCloseMeshlinePolicies(
		MeshlinePolicy* a,
		MeshlinePolicy* b)
: Conflict(Kind::TOO_CLOSE_MESHLINE_POLICIES)
, meshline_policies({ a, b })
{}

//******************************************************************************
void ConflictTooCloseMeshlinePolicies::auto_solve(MeshlinePolicyManager& line_policy_manager) {
	auto& a = meshline_policies.front();
	auto& b = meshline_policies.back();
	auto axis = a->axis;
	if(axis != b->axis)
		return;

	auto [policy, normal] = [&]() -> tuple<optional<MeshlinePolicy::Policy>, optional<Normal>> {
		if(a->policy == MeshlinePolicy::Policy::THIRDS && b->policy == MeshlinePolicy::Policy::THIRDS) {
			if(a->normal != b->normal) {
				return { MeshlinePolicy::Policy::HALFS, Normal::NONE };
			} else if(a->normal == Normal::YMIN
			       && b->normal == Normal::YMIN) {
				return { MeshlinePolicy::Policy::THIRDS, Normal::YMIN };
			} else if(a->normal == Normal::YMAX
			       && b->normal == Normal::YMAX) {
				return { MeshlinePolicy::Policy::THIRDS, Normal::YMAX };
			} else if(a->normal == Normal::XMIN
			       && b->normal == Normal::XMIN) {
				return { MeshlinePolicy::Policy::THIRDS, Normal::XMIN };
			} else if(a->normal == Normal::XMAX
			       && b->normal == Normal::XMAX) {
				return { MeshlinePolicy::Policy::THIRDS, Normal::XMAX };
			}
		} else if((a->policy == MeshlinePolicy::Policy::HALFS && b->policy == MeshlinePolicy::Policy::HALFS)
		       || (a->policy == MeshlinePolicy::Policy::HALFS && b->policy == MeshlinePolicy::Policy::THIRDS)
		       || (a->policy == MeshlinePolicy::Policy::THIRDS && b->policy == MeshlinePolicy::Policy::HALFS)) {
			return { MeshlinePolicy::Policy::HALFS, Normal::NONE };
		} //else if(ONE and *) { // TODO }
		// TODO should not have been created ?
		// TODO only one, remove other?
		// TODO one and one : merge

		return { nullopt, nullopt };
	} ();

	if(policy && normal) {
		meshline_policy = line_policy_manager.add_meshline_policy(
			this,
			axis,
			policy.value(),
			normal.value(),
			mid(a->coord, b->coord));
		solution = meshline_policy;
		is_solved = true;
	}

	// TODO detect axis
	// TODO detect 2 vs 3 rule (vs 1 rule?)
	// TODO detect normal
	// TODO create MLP

}


// TODO Interval class : just mesh() from adjacent / neibourgh
