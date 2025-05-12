///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <array>

#include "domain/geometrics/space.hpp"
#include "utils/state_management.hpp"
#include "conflict.hpp"

namespace domain {

//******************************************************************************
struct ConflictTooCloseMeshlinePoliciesState final : public ConflictState {
	bool is_enabled;
};

//******************************************************************************
class ConflictTooCloseMeshlinePolicies
: public Originator<ConflictTooCloseMeshlinePoliciesState>
, public Visitable<ConflictTooCloseMeshlinePolicies, EntityVisitor>
, public Conflict {
public:
	Axis const axis;
	std::array<MeshlinePolicy* const, 2> const meshline_policies;

	ConflictTooCloseMeshlinePolicies(Axis axis, MeshlinePolicy* a, MeshlinePolicy* b, Timepoint* t);

	void auto_solve(MeshlinePolicyManager& line_policy_manager) override;
};

} // namespace domain
