///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <vector>

#include "conflict.hpp"

class Edge;

//******************************************************************************
class ConflictColinearEdges : public Conflict {
public:
	std::vector<Edge*> edges;

	ConflictColinearEdges(Edge* a, Edge* b);

	void append(Edge* edge);

	void auto_solve(MeshlinePolicyManager& line_policy_manager) override;

#ifdef DEBUG
	void print() const override {} // TODO
#endif // DEBUG
};