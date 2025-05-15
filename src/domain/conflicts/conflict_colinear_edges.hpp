///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <vector>

#include "domain/geometrics/space.hpp"
#include "utils/state_management.hpp"
#include "conflict.hpp"

namespace domain {

class Edge;

//******************************************************************************
struct ConflictColinearEdgesState final : public ConflictState {
	std::vector<Edge*> edges;
};

//******************************************************************************
class ConflictColinearEdges
: public Originator<ConflictColinearEdgesState const>
, public Visitable<ConflictColinearEdges, EntityVisitor>
, public Conflict {
public:
	Axis const axis;

	ConflictColinearEdges(Axis axis, Edge* a, Edge* b, Timepoint* t);

	void append(Edge* edge, Timepoint* t = nullptr);

	void auto_solve(MeshlinePolicyManager& line_policy_manager) override;
};

} // namespace domain
