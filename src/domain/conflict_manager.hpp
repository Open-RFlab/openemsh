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
#include "conflicts/conflict_colinear_edges.hpp"
#include "conflicts/conflict_edge_in_polygon.hpp"
#include "conflicts/conflict_too_close_meshline_policies.hpp"

class Edge;
class MeshlinePolicyManager;
class MeshlinePolicy;
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
	std::vector<std::unique_ptr<ConflictTooCloseMeshlinePolicies>> all_too_close_meshline_policies;

public:
	explicit ConflictManager(MeshlinePolicyManager* const line_policy_manager);

	void add_colinear_edges(Edge* a, Edge* b);

	void add_edge_in_polygon(Edge* a, Polygon* polygon, std::optional<Edge const*> b = std::nullopt);
	void add_edge_in_polygon(Edge* a, Polygon* polygon, Range const range, std::optional<Edge const*> b = std::nullopt);
	ConflictTooCloseMeshlinePolicies* add_too_close_meshline_policies(MeshlinePolicy* a, MeshlinePolicy* b) noexcept;

//	void add_user_will(Edge* a); // TODO

	void auto_solve_all_edge_in_polygon();
	void auto_solve_all_colinear_edges();
//	Conflict* find(std::vector<IConflictOrigin> const&);

	std::vector<std::unique_ptr<ConflictColinearEdges>> const& get_colinear_edges() const;
	std::vector<std::unique_ptr<ConflictEdgeInPolygon>> const& get_edge_in_polygons() const;
	std::vector<std::unique_ptr<ConflictTooCloseMeshlinePolicies>> const& get_too_close_meshline_policies() const;
};

#ifdef UNITTEST
#undef private
#endif // UNITTEST
