///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "meshline_policy_manager.hpp"

using namespace std;

//******************************************************************************
MeshlinePolicyManager::MeshlinePolicyManager(Params& _params)
: params(_params)
{}

//******************************************************************************
MeshlinePolicy* MeshlinePolicyManager::add_meshline_policy(
		IMeshLineOrigin* origin,
		MeshlinePolicy::Axis const axis,
		MeshlinePolicy::Policy const policy,
		Normal const normal,
		double const coord,
		bool const is_enabled) {

	if((axis == MeshlinePolicy::Axis::X && (normal == Normal::YMIN || normal == Normal::YMAX))
	|| (axis == MeshlinePolicy::Axis::Y && (normal == Normal::XMIN || normal == Normal::XMAX))
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
GridAxis cast(MeshlinePolicy::Axis const a) {
	switch(a) {
	case MeshlinePolicy::Axis::X: return X;
	case MeshlinePolicy::Axis::Y: return Y;
	}
}
