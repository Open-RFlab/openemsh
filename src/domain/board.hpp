///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <array>
//#include <initializer_list>
#include <memory>
#include <vector>

#include "conflicts/conflict.hpp"
#include "geometrics/edge.hpp"
#include "geometrics/point.hpp"
#include "geometrics/polygon.hpp"
#include "utils/entity.hpp"
#include "conflict_manager.hpp"
#include "global.hpp"
#include "meshline_policy_manager.hpp"
//#include "meshline_policy.hpp"
//#include "range.hpp"

#ifdef UNITTEST
#define private public
#endif // UNITTEST

//******************************************************************************
class Board : public Entity, public Visitable<Board, EntityVisitor> {
private:
	ConflictManager conflict_manager;
	MeshlinePolicyManager line_policy_manager;
	std::vector<std::unique_ptr<Polygon>> polygons;
	std::vector<Edge*> edges;
	Params params;

public:
//	Board(std::initializer_list<Polygon> _polygons);
	explicit Board(std::vector<std::unique_ptr<Polygon>>& polygons);
	Board(); // TODO

	void add_polygon(Polygon const polygon); // TODO

	/// Mesh resolution independant detection tasks
	///*************************************************************************
	void detect_edges_in_polygons();
	void detect_colinear_edges();
	void detect_non_conflicting_edges();

	/// Mesh resolution dependant detection tasks
	///*************************************************************************
	void detect_ranges_between_lines() {} // TODO
	void detect_too_close_lines() {} // TODO

	void auto_mesh() {} // TODO

	std::vector<std::unique_ptr<Meshline>> get_meshline_policies_meshlines(GridAxis axis) const;
	std::vector<std::unique_ptr<Meshline>> const& get_meshlines(GridAxis axis) const;
	std::vector<std::unique_ptr<MeshlinePolicy>> const& get_meshline_policies(GridAxis axis) const;
	std::vector<std::unique_ptr<Interval>> const& get_intervals(GridAxis axis) const;
	std::vector<std::unique_ptr<Polygon>> const& get_polygons() const;
	std::vector<std::unique_ptr<ConflictEdgeInPolygon>> const& get_conflicts_edge_in_polygons() const;
	std::vector<std::unique_ptr<ConflictColinearEdges>> const& get_conflicts_colinear_edges() const;
	std::vector<std::unique_ptr<ConflictTooCloseMeshlinePolicies>> const& get_conflicts_too_close_meshline_policies() const;

#ifdef DEBUG
	void print() const;
#endif // DEBUG
};

#ifdef UNITTEST
#undef private

void sort_points_by_vector_orientation(std::vector<Point>& points, Point const& vector);
#endif // UNITTEST
