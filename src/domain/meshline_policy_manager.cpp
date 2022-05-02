///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "meshline_policy_manager.hpp"

using namespace std;

//******************************************************************************
MeshlinePolicyManager::MeshlinePolicyManager(Params& params)
: params(params)
{}

//******************************************************************************
MeshlinePolicy* MeshlinePolicyManager::add_meshline_policy(
		IMeshLineOrigin* origin,
		MeshlinePolicy::Axis const axis,
		MeshlinePolicy::Policy const policy,
		Normal const normal,
		Coord const coord,
		bool const is_enabled) {

	// TODO fix
	if((axis == MeshlinePolicy::Axis::H && (normal == Normal::XMIN || normal == Normal::XMAX))
	|| (axis == MeshlinePolicy::Axis::V && (normal == Normal::YMIN || normal == Normal::YMAX))
	|| (policy == MeshlinePolicy::Policy::THIRDS && normal == Normal::NONE)
	|| (policy != MeshlinePolicy::Policy::THIRDS && normal != Normal::NONE))
		return nullptr;

	unique_ptr<MeshlinePolicy>& line_policy = line_policies[cast(axis)].emplace_back(make_unique<MeshlinePolicy>(
		axis, policy, normal, params, coord, is_enabled));

	line_policy->origins.emplace_back(origin);

	return line_policy.get();
}

//******************************************************************************
void MeshlinePolicyManager::mesh(MeshlinePolicy& policy) {
	unique_ptr<Meshline>& line = meshlines[cast(policy.axis)].emplace_back(
		make_unique<Meshline>(policy.mesh()));
	policy.meshlines.push_back(line.get());
}

//******************************************************************************
GridAxis cast(MeshlinePolicy::Axis const a) noexcept {
	switch(a) {
	case MeshlinePolicy::Axis::H: return H;
	case MeshlinePolicy::Axis::V: return V;
	}
}
