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
		MeshlinePolicy* b,
		Timepoint* t)
: Originator(t)
, Conflict(Kind::TOO_CLOSE_MESHLINE_POLICIES)
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
		auto const& state_a = a->get_current_state();
		auto const& state_b = b->get_current_state();

		if(state_a.policy == MeshlinePolicy::Policy::THIRDS && state_b.policy == MeshlinePolicy::Policy::THIRDS) {
			if(state_a.normal != state_b.normal) {
				return { MeshlinePolicy::Policy::HALFS, MeshlinePolicy::Normal::NONE };
			} else if(state_a.normal == MeshlinePolicy::Normal::MIN
			       && state_b.normal == MeshlinePolicy::Normal::MIN) {
				return { MeshlinePolicy::Policy::THIRDS, MeshlinePolicy::Normal::MIN };
			} else if(state_a.normal == MeshlinePolicy::Normal::MAX
			       && state_b.normal == MeshlinePolicy::Normal::MAX) {
				return { MeshlinePolicy::Policy::THIRDS, MeshlinePolicy::Normal::MAX };
			}
		} else if((state_a.policy == MeshlinePolicy::Policy::HALFS && state_b.policy == MeshlinePolicy::Policy::HALFS)
		       || (state_a.policy == MeshlinePolicy::Policy::HALFS && state_b.policy == MeshlinePolicy::Policy::THIRDS)
		       || (state_a.policy == MeshlinePolicy::Policy::THIRDS && state_b.policy == MeshlinePolicy::Policy::HALFS)) {
			return { MeshlinePolicy::Policy::HALFS, MeshlinePolicy::Normal::NONE };
		} //else if(ONE and *) { // TODO }
		// TODO should not have been created ?
		// TODO only one, remove other?
		// TODO one and one : merge

		return { nullopt, nullopt };
	} ();

	if(policy && normal) {
		auto [t, state] = make_next_state();
		state.meshline_policy = line_policy_manager.add_meshline_policy(
			{ this },
			axis,
			policy.value(),
			normal.value(),
			mid(a->coord, b->coord),
			true,
			t);
		state.solution = state.meshline_policy;
		state.is_solved = true;
		set_state(t, state);
	}

	// TODO detect axis
	// TODO detect 2 vs 3 rule (vs 1 rule?)
	// TODO detect normal
	// TODO create MLP

}

// TODO Interval class : just mesh() from adjacent / neibourgh

} // namespace domain
