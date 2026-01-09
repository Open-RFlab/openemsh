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
#include "conflicts/conflict_diagonal_or_circular_zone.hpp"
#include "geometrics/space.hpp"
#include "utils/state_management.hpp"

namespace domain {

class Angle;
class Edge;
class MeshlinePolicyManager;
class MeshlinePolicy;
class Polygon;
class Range;
class GlobalParams;

#ifdef UNITTEST
#define private public
#endif // UNITTEST

//******************************************************************************
struct ConflictManagerState final {
	PlaneSpace<std::vector<std::shared_ptr<ConflictEdgeInPolygon>>> all_edge_in_polygons;
	AxisSpace<std::vector<std::shared_ptr<ConflictColinearEdges>>> all_colinear_edges;
	AxisSpace<std::vector<std::shared_ptr<ConflictTooCloseMeshlinePolicies>>> all_too_close_meshline_policies;
	AxisSpace<std::vector<std::shared_ptr<ConflictDiagonalOrCircularZone>>> all_diagonal_or_circular_zone;
};

/// Create / append conflicts regarding already existing conflicts
///*****************************************************************************
class ConflictManager
: public Originator<ConflictManagerState const> {
private:
	MeshlinePolicyManager* line_policy_manager;

public:
	explicit ConflictManager(Timepoint* t);

	void init(MeshlinePolicyManager* _line_policy_manager);

	void add_colinear_edges(Edge* a, Edge* b);

	void add_edge_in_polygon(Edge* a, Polygon* polygon, std::optional<Edge const*> b = std::nullopt);
	void add_edge_in_polygon(Edge* a, Polygon* polygon, Range const range, std::optional<Edge const*> b = std::nullopt);
	ConflictTooCloseMeshlinePolicies* add_too_close_meshline_policies(MeshlinePolicy* a, MeshlinePolicy* b) noexcept;

	void add_diagonal_or_circular_zone(Axis axis, std::vector<Angle*> const& angles, std::vector<Edge*> const& edges, GlobalParams* global_params);

//	void add_user_will(Edge* a); // TODO

	void auto_solve_all_diagonal_angles(Axis const axis);
	void auto_solve_all_diagonal_zones(Axis const axis);
	void auto_solve_all_edge_in_polygon(Plane const plane);
	void auto_solve_all_colinear_edges(Axis const axis);
//	Conflict* find(std::vector<IConflictOrigin> const&);

	std::vector<std::shared_ptr<ConflictColinearEdges>> const& get_colinear_edges(Axis const axis) const;
	std::vector<std::shared_ptr<ConflictEdgeInPolygon>> const& get_edge_in_polygons(Plane const plane) const;
	std::vector<std::shared_ptr<ConflictTooCloseMeshlinePolicies>> const& get_too_close_meshline_policies(Axis const axis) const;
	std::vector<std::shared_ptr<ConflictDiagonalOrCircularZone>> const& get_diagonal_or_circular_zones(Axis const axis) const;
};

#ifdef UNITTEST
#undef private
#endif // UNITTEST

} // namespace domain
