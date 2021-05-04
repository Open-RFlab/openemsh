///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <initializer_list>
#include <memory>
#include <vector>

#include "bounding.hpp"
//#include "conflict.hpp"
//#include "edge.hpp"
#include "global.hpp"
#include "i_conflict_origin.hpp"
#include "i_meshline_origin.hpp"
//#include "point.hpp"
#include "relation.hpp"

class Conflict;
class Edge;
class Point;

//******************************************************************************
class Polygon : public IConflictOrigin, public IMeshLineOrigin {
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

	Bounding bounding;

	// TODO rm unique_ptr ?
	std::vector<std::unique_ptr<Point const>> points;

	/// edge[0] is between points[n] & points[0]
	/// edge[x] is between points[x-1] & points[x]
	///*************************************************************************
	std::vector<std::unique_ptr<Edge>> edges;

	std::vector<Conflict*> conflicts;

	Polygon(std::initializer_list<Point> _points);

//	relation::PolygonEdge relation_to(Edge const* edge);
	relation::PolygonPoint relation_to(Point const& point) const;

#ifdef DEBUG
	void print() const;
#endif // DEBUG
};

//******************************************************************************
Polygon::Rotation detect_rotation(std::vector<std::unique_ptr<Point const>> const& points);
Polygon::Rotation detect_rotation(std::vector<Point const*> const points);