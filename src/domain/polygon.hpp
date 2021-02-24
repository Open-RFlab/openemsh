///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <array>
#include <initializer_list>
#include <memory>
#include <vector>

//#include "conflict.hpp"
//#include "edge.hpp"
#include "i_conflict_origin.hpp"
#include "i_meshline_origin.hpp"
//#include "point.hpp"
#include "relation.hpp"

class Conflict;
class Edge;
class MeshlineManager;
class Point;

//******************************************************************************
enum { XMIN, XMAX, YMIN, YMAX };

//******************************************************************************
class Polygon : public IConflictOrigin, public IMeshLineOrigin {
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

	std::array<double, 4> bounding;
	std::vector<std::unique_ptr<Point const>> points;

	/// edge[0] is between points[n] & points[0]
	/// edge[x] is between points[x-1] & points[x]
	///*************************************************************************
	std::vector<std::unique_ptr<Edge>> edges;

	std::vector<Conflict*> conflicts;

	Polygon(std::initializer_list<Point> _points);
//	inline void detect_rotation();
	void detect_edge_normal();
//	void is_inside(Point const* point);
//	relation::PolygonEdge relation_to(Edge const* edge);
	relation::PolygonPoint relation_to(Point const* point) const;

	void print() const;
};

//******************************************************************************
bool are_possibly_overlapping(Polygon const& a, Polygon const& b);

//******************************************************************************
Polygon::Rotation detect_rotation(std::vector<std::unique_ptr<Point const>> const& points);
Polygon::Rotation detect_rotation(std::vector<Point const*> const points);
