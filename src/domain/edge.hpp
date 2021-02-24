///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

//#include <array>
#include <memory>
#include <vector>

//#include "conflict.hpp"
//#include "meshline_manager.hpp"
//#include "point.hpp"
#include "global.hpp"
#include "i_conflict_origin.hpp"
#include "i_meshline_origin.hpp"
#include "relation.hpp"

class Conflict;
class MeshlineManager;
class Point;
class Range;

#ifdef UNITTEST
#define private public
#endif // UNITTEST

//******************************************************************************
class Edge : public IConflictOrigin, public IMeshLineOrigin {
public:
//s	bool is_enabled;
	Point const* const p0;
	Point const* const p1;
	std::unique_ptr<Point const> const vec;

	/// Direction of the edge itself. Useful to detect vertical or horizontal
	/// edges.
	///*************************************************************************
	enum class Direction {
		XMIN,
		XMAX,
		YMIN,
		YMAX,
		DIAGONAL
//		XAXIS,
//		YAXIS,
//		POINT        // TODO is this usefull?
	} direction;

	Normal normal;

	std::vector<Conflict*> conflicts;
	MeshlineManager* meshline_manager;

	Edge(Point const* _p0, Point const* _p1);
//	Relation is_crossing(Edge const* edge) const;
	relation::EdgeEdge relation_to(Edge const* edge) const;
	relation::EdgePoint relation_to(Point const* point) const; // TODO

	void print() const;
};

#ifdef UNITTEST
#undef private
#endif // UNITTEST

bool are_colinear(Edge const& a, Edge const& b); // TODO this vs relation_to : good design?
Point* intersection(Edge const* a, Edge const* b); // TODO ?
Range* overlap(Edge const* a, Edge const* b);
//std::array<Point, 2>* overlap(Edge const& a, Edge const& b);