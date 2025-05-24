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
#include "geometrics/space.hpp"
#include "utils/entity.hpp"
#include "utils/entity_visitor.hpp"
#include "utils/state_management.hpp"
#include "conflict_manager.hpp"
#include "global.hpp"
#include "meshline_policy_manager.hpp"
//#include "meshline_policy.hpp"
//#include "range.hpp"

namespace domain {

#ifdef UNITTEST
#define private public
#endif // UNITTEST

//******************************************************************************
struct BoardState final {
	PlaneSpace<std::vector<std::shared_ptr<Polygon>>> polygons;
	PlaneSpace<std::vector<Edge*>> edges;

	explicit BoardState(PlaneSpace<std::vector<std::shared_ptr<Polygon>>>&& polygons);
};

//******************************************************************************
class Board
: public Originator<BoardState const>
, public Visitable<Board, EntityVisitor>
, public Entity {
public:
	std::shared_ptr<GlobalParams> global_params;
private:
	std::shared_ptr<ConflictManager> conflict_manager;
	std::shared_ptr<MeshlinePolicyManager> line_policy_manager;

public:
	//**************************************************************************
	class Builder {
	public:

		void add_fixed_meshline_policy(Axis axis, Coord coord);
		void add_polygon(Plane plane, Polygon::Type type, std::string const& name, std::initializer_list<Point> points);
		void add_polygon(Plane plane, Polygon::Type type, std::string const& name, std::vector<std::unique_ptr<Point const>>&& points);
		void add_polygon_from_box(Plane plane, Polygon::Type type, std::string const& name, Point const p1, Point const p3);

		[[nodiscard]] std::shared_ptr<Board> build(Params&& params = Params());

	private:
		PlaneSpace<std::vector<std::shared_ptr<Polygon>>> polygons;
		AxisSpace<std::vector<std::function<void (Board*, Timepoint*)>>> fixed_meshline_policy_creators;
	};

	AxisSpace<std::vector<std::function<void (Board*, Timepoint*)>>> const fixed_meshline_policy_creators; // Meant to delay MeshlinePolicies creation at Step time instead of Parse time.

	Board(PlaneSpace<std::vector<std::shared_ptr<Polygon>>>&& polygons, Params&& params, Timepoint* t);
	Board(
		PlaneSpace<std::vector<std::shared_ptr<Polygon>>>&& polygons,
		AxisSpace<std::vector<std::function<void (Board*, Timepoint*)>>>&& fixed_meshline_policy_creators,
		Params&& params,
		Timepoint* t);

	/// Mesh resolution independant detection tasks
	///*************************************************************************
	void detect_edges_in_polygons(Plane const plane);
	void detect_colinear_edges(Plane plane);
	void detect_non_conflicting_edges(Plane const plane);
	void add_fixed_meshline_policies(Axis axis);

	void detect_edges_in_polygons();
	void detect_colinear_edges();
	void detect_non_conflicting_edges();
	void add_fixed_meshline_policies();

	/// Mesh resolution dependant detection tasks
	///*************************************************************************
	void auto_solve_all_edge_in_polygon();
	void auto_solve_all_colinear_edges();
	void detect_and_solve_too_close_meshline_policies();
	void detect_intervals();
	void mesh();

	std::vector<std::shared_ptr<Meshline>> get_meshline_policies_meshlines(Axis axis) const;
	std::vector<std::shared_ptr<Meshline>> const& get_meshlines(Axis axis) const;
	std::vector<std::shared_ptr<MeshlinePolicy>> const& get_meshline_policies(Axis axis) const;
	std::vector<std::shared_ptr<Interval>> const& get_intervals(Axis axis) const;
	std::vector<std::shared_ptr<Polygon>> const& get_polygons(Plane plane) const;
	std::vector<std::shared_ptr<ConflictEdgeInPolygon>> const& get_conflicts_edge_in_polygons(Plane const plane) const;
	std::vector<std::shared_ptr<ConflictColinearEdges>> const& get_conflicts_colinear_edges(Axis const axis) const;
	std::vector<std::shared_ptr<ConflictTooCloseMeshlinePolicies>> const& get_conflicts_too_close_meshline_policies(Axis const axis) const;
};

#ifdef UNITTEST
#undef private
void sort_points_by_vector_orientation(std::vector<Point>& points, Point const& vector);
#endif // UNITTEST

} // namespace domain
