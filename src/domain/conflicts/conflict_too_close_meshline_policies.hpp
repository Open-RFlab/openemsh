///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <array>

#include "domain/geometrics/space.hpp"
#include "conflict.hpp"

//******************************************************************************
class ConflictTooCloseMeshlinePolicies
: public Conflict
, public Visitable<ConflictTooCloseMeshlinePolicies, EntityVisitor> {
public:
	Axis const axis;
	std::array<MeshlinePolicy* const, 2> const meshline_policies;

	ConflictTooCloseMeshlinePolicies(Axis axis, MeshlinePolicy* a, MeshlinePolicy* b);

	void auto_solve(MeshlinePolicyManager& line_policy_manager) override;
};
