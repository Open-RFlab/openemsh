///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <optional>
#include <tuple>

#include "domain/geometrics/space.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "domain/meshline_policy_manager.hpp"

#include "conflict_too_close_meshline_policies.hpp"

namespace domain {

using namespace std;

//******************************************************************************
ConflictTooCloseMeshlinePolicies::ConflictTooCloseMeshlinePolicies(
		Axis axis,
		MeshlinePolicy* a,
		MeshlinePolicy* b)
: Conflict(Kind::TOO_CLOSE_MESHLINE_POLICIES)
, axis(axis)
, meshline_policies({ a, b })
{}

//******************************************************************************
void ConflictTooCloseMeshlinePolicies::auto_solve(MeshlinePolicyManager& line_policy_manager) {
	auto& a = meshline_policies.front();
	auto& b = meshline_policies.back();
	if(a->axis != b->axis)
		return;

	auto [policy, normal] = [&]() -> tuple<optional<MeshlinePolicy::Policy>, optional<MeshlinePolicy::Normal>> {
		if(a->policy == MeshlinePolicy::Policy::THIRDS && b->policy == MeshlinePolicy::Policy::THIRDS) {
			if(a->normal != b->normal) {
				return { MeshlinePolicy::Policy::HALFS, MeshlinePolicy::Normal::NONE };
			} else if(a->normal == MeshlinePolicy::Normal::MIN
			       && b->normal == MeshlinePolicy::Normal::MIN) {
				return { MeshlinePolicy::Policy::THIRDS, MeshlinePolicy::Normal::MIN };
			} else if(a->normal == MeshlinePolicy::Normal::MAX
			       && b->normal == MeshlinePolicy::Normal::MAX) {
				return { MeshlinePolicy::Policy::THIRDS, MeshlinePolicy::Normal::MAX };
			}
		} else if((a->policy == MeshlinePolicy::Policy::HALFS && b->policy == MeshlinePolicy::Policy::HALFS)
		       || (a->policy == MeshlinePolicy::Policy::HALFS && b->policy == MeshlinePolicy::Policy::THIRDS)
		       || (a->policy == MeshlinePolicy::Policy::THIRDS && b->policy == MeshlinePolicy::Policy::HALFS)) {
			return { MeshlinePolicy::Policy::HALFS, MeshlinePolicy::Normal::NONE };
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

} // namespace domain
