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

//#include "conflict.hpp"
//#include "edge.hpp"
//#include "point.hpp"
#include "domain/conflicts/i_conflict_origin.hpp"
#include "domain/mesh/i_meshline_origin.hpp"
#include "utils/entity.hpp"
#include "domain/global.hpp"
#include "bounding.hpp"
#include "relation.hpp"

class Conflict;
class Edge;
class Point;

//******************************************************************************
class Polygon
: public Entity
, public Visitable<Polygon, EntityVisitor>
, public IConflictOrigin
/*, public IMeshLineOrigin*/ {
private:
	void detect_bounding(); // TODO
	void detect_edge_normal();

public:
	enum class Rotation {
//		UNKNOWN,
		CW,
		CCW,
		COLINEAR
	} rotation;

	enum class Type {
		SHAPE,
		PORT
	} type; // TODO usefull?

	Bounding2D bounding;
	std::string name; // TODO

	// TODO rm unique_ptr ?
	std::vector<std::unique_ptr<Point const>> points;

	/// edge[0] is between points[n] & points[0]
	/// edge[x] is between points[x-1] & points[x]
	///*************************************************************************
	std::vector<std::unique_ptr<Edge>> edges;

	explicit Polygon(std::initializer_list<Point> points);
	Polygon(std::string name, std::initializer_list<Point> points);

//	relation::PolygonEdge relation_to(Edge const* edge);
	relation::PolygonPoint relation_to(Point const& point) const;

#ifdef DEBUG
	void print() const;
#endif // DEBUG
};

/// These are the two declaration authorized.
///*****************************************************************************
template<class T> Polygon::Rotation detect_rotation(T const& points);
extern template Polygon::Rotation detect_rotation(std::vector<std::unique_ptr<Point const>> const&);
extern template Polygon::Rotation detect_rotation(std::vector<Point const*> const&);
