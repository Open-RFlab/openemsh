///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <memory>
#include <optional>
#include <vector>

//#include "conflict.hpp"
#include "conflict_colinear_edges.hpp"
#include "conflict_edge_in_polygon.hpp"

class Edge;
class MeshlinePolicyManager;
class Polygon;
class Range;

#ifdef UNITTEST
#define private public
#endif // UNITTEST

/// Create / append conflicts regarding already existing conflicts
///*****************************************************************************
class ConflictManager {
private:
	MeshlinePolicyManager* const line_policy_manager;

	std::vector<std::unique_ptr<ConflictEdgeInPolygon>> all_edge_in_polygons;
	std::vector<std::unique_ptr<ConflictColinearEdges>> all_colinear_edges;

public:
	explicit ConflictManager(MeshlinePolicyManager* const line_policy_manager);

	void add_colinear_edges(Edge* a, Edge* b);

	void add_edge_in_polygon(Edge* a, Polygon* polygon, std::optional<Edge const*> b = std::nullopt);
	void add_edge_in_polygon(Edge* a, Polygon* polygon, Range const range, std::optional<Edge const*> b = std::nullopt);

//	void add_user_will(Edge* a); // TODO

	void auto_solve_all_edge_in_polygon();
	void auto_solve_all_colinear_edges();
//	Conflict* find(std::vector<IConflictOrigin> const&);
};

#ifdef UNITTEST
#undef private
#endif // UNITTEST
