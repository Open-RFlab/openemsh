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
class Board : public Entity, public Visitable<Board, EntityVisitor> {
private:
	ConflictManager conflict_manager;
	MeshlinePolicyManager line_policy_manager;

	PlaneSpace<std::vector<std::unique_ptr<Polygon>>> polygons;
	PlaneSpace<std::vector<Edge*>> edges;

public:
	//**************************************************************************
	class Builder {
	public:

		void add_fixed_meshline_policy(Axis axis, Coord coord);
		void add_polygon(Plane plane, Polygon::Type type, std::string const& name, std::initializer_list<Point> points);
		void add_polygon(Plane plane, Polygon::Type type, std::string const& name, std::vector<std::unique_ptr<Point const>>&& points);
		void add_polygon_from_box(Plane plane, Polygon::Type type, std::string const& name, Point const p1, Point const p3);

		[[nodiscard]] std::unique_ptr<Board> build();

	private:
		PlaneSpace<std::vector<std::unique_ptr<Polygon>>> polygons;
		AxisSpace<std::vector<std::unique_ptr<MeshlinePolicy>>> line_policies;
	};

	Params params;

	explicit Board(PlaneSpace<std::vector<std::unique_ptr<Polygon>>>&& polygons);
	Board(
		PlaneSpace<std::vector<std::unique_ptr<Polygon>>>&& polygons,
		AxisSpace<std::vector<std::unique_ptr<MeshlinePolicy>>>&& line_policies);

	/// Mesh resolution independant detection tasks
	///*************************************************************************
	void detect_edges_in_polygons(Plane const plane);
	void detect_colinear_edges(Plane plane);
	void detect_non_conflicting_edges(Plane const plane);

	void detect_edges_in_polygons() { for(auto const& plane : AllPlane) detect_edges_in_polygons(plane); };
	void detect_colinear_edges() { for(auto const& plane : AllPlane) detect_colinear_edges(plane); };
	void detect_non_conflicting_edges() { for(auto const& plane : AllPlane) detect_non_conflicting_edges(plane); };

	/// Mesh resolution dependant detection tasks
	///*************************************************************************
	void detect_too_close_lines() {} // TODO
	void detect_intervals_between_lines() {} // TODO

	void auto_mesh(); // TODO

	std::vector<std::unique_ptr<Meshline>> get_meshline_policies_meshlines(Axis axis) const;
	std::vector<std::unique_ptr<Meshline>> const& get_meshlines(Axis axis) const;
	std::vector<std::unique_ptr<MeshlinePolicy>> const& get_meshline_policies(Axis axis) const;
	std::vector<std::unique_ptr<Interval>> const& get_intervals(Axis axis) const;
	std::vector<std::unique_ptr<Polygon>> const& get_polygons(Plane plane) const;
	std::vector<std::unique_ptr<ConflictEdgeInPolygon>> const& get_conflicts_edge_in_polygons(Plane const plane) const;
	std::vector<std::unique_ptr<ConflictColinearEdges>> const& get_conflicts_colinear_edges(Axis const axis) const;
	std::vector<std::unique_ptr<ConflictTooCloseMeshlinePolicies>> const& get_conflicts_too_close_meshline_policies(Axis const axis) const;
};

#ifdef UNITTEST
#undef private
void sort_points_by_vector_orientation(std::vector<Point>& points, Point const& vector);
#endif // UNITTEST

} // namespace domain
