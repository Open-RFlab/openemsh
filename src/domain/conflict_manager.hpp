///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <memory>
#include <optional>
#include <vector>

#include "conflict.hpp"

class Edge;
class MeshlinePolicyManager;
class Polygon;
class Range;

/// Create / append conflicts regarding already existing conflicts
///*****************************************************************************
class ConflictManager {
private:
	std::vector<std::unique_ptr<Conflict>>& conflicts; // TODO multiple vectors (for each kind)
	MeshlinePolicyManager* const line_policy_manager;

public:
	ConflictManager(std::vector<std::unique_ptr<Conflict>>& _conflicts, MeshlinePolicyManager* const _line_policy_manager);

	void add_colinear_edges(Edge* a, Edge* b);

	void add_edge_in_polygon(Edge* a, Polygon* polygon, std::optional<Edge const*> b = std::nullopt);
	void add_edge_in_polygon(Edge* a, Polygon* polygon, Range const range, std::optional<Edge const*> b = std::nullopt);

//	void add_user_will(Edge* a); // TODO

	void auto_solve_all_edge_in_polygon();
	void auto_solve_all_colinear_edges();
//	Conflict* find(std::vector<IConflictOrigin> const&);
};