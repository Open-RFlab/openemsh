///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <array>

#include "conflict.hpp"

//******************************************************************************
class ConflictTooCloseMeshlinePolicies : public Conflict {
public:
	std::array<MeshlinePolicy* const, 2> const meshline_policies;

	ConflictTooCloseMeshlinePolicies(MeshlinePolicy* a, MeshlinePolicy* b);

	void auto_solve(MeshlinePolicyManager& line_policy_manager) override;
};
