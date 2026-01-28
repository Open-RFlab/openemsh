///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "domain/conflicts/i_conflict_origin.hpp"
#include "domain/mesh/i_meshline_origin.hpp"
#include "domain/utils/entity_visitor.hpp"
#include "domain/material.hpp"
#include "utils/entity.hpp"
#include "utils/state_management.hpp"
#include "bounding.hpp"
#include "relation.hpp"
#include "space.hpp"

namespace domain {

class Conflict;
class Edge;
class Point;

//******************************************************************************
struct PolygonState final : public IConflictOriginState {
};

//******************************************************************************
class Polygon
: public Originator<PolygonState const>
, public Visitable<Polygon, EntityVisitor>
, public Entity
, public IConflictOrigin
/*, public IMeshLineOrigin*/ {
private:
	void detect_edge_normal() noexcept;

public:
	enum class Rotation {
//		UNKNOWN,
		CW,
		CCW,
		COLINEAR
	} const rotation;

	struct RangeZ {
		Coord min;
		Coord max;
		RangeZ(Coord const& a, Coord const& b);
	} const z_placement;

	std::size_t const priority; /// At 3D overlap, the higher is selected.
	std::shared_ptr<Material> material;
	Plane const plane;
	Bounding2D const bounding;
	std::string const name;

	std::vector<std::unique_ptr<Point const>> const points;

	/// edge[0] is between points[n] & points[0]
	/// edge[x] is between points[x-1] & points[x]
	///*************************************************************************
	std::vector<std::shared_ptr<Edge>> const edges;

	Polygon(Plane plane, std::shared_ptr<Material> const& material, std::string const& name, std::size_t priority, RangeZ const& z_placement, std::vector<std::unique_ptr<Point const>>&& points, Timepoint* t);
	~Polygon() override;

//	relation::PolygonEdge relation_to(Edge const* edge);
	relation::PolygonPoint relation_to(Point const& point) const noexcept;
};

/// These are the two declaration authorized.
///*****************************************************************************
template<class T> Polygon::Rotation detect_rotation(T const& points) noexcept;
extern template Polygon::Rotation detect_rotation(std::vector<std::unique_ptr<Point const>> const&) noexcept;
extern template Polygon::Rotation detect_rotation(std::vector<Point const*> const&) noexcept;

//******************************************************************************
Bounding2D detect_bounding(std::vector<std::unique_ptr<Point const>> const& points) noexcept;

//******************************************************************************
std::vector<std::shared_ptr<Edge>> detect_edges(std::vector<std::unique_ptr<Point const>> const& points, Plane plane, Timepoint* t);

//******************************************************************************
bool does_overlap(Polygon::RangeZ const& a, Polygon::RangeZ const& b) noexcept;

//******************************************************************************
std::vector<std::unique_ptr<Point const>> circle_to_points(Point const& center, double radius, std::size_t n_slices_per_quarter = 9);

} // namespace domain
