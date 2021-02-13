///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <array>
#include <memory>

#include "point.hpp"

//******************************************************************************
class Edge {
public:
	Point const* p0;
	Point const* p1;
	std::unique_ptr<Point const> vec;
//	std::array<MeshLine*, 2> meshlines;

	/// Describe meshing policy to apply to the edge.
	///*************************************************************************
	enum class Status {
		DISABLED,    ///< Do not mesh this edge : if the edge is totally overlapping another
		             /// or if the edge is inside a polygon.
		HALFS,       ///< Apply halfs rule while meshing : when edges conflict on the direction.
		THIRDS       ///< Apply thirds rule while meshing : normal case.
//		ONELINE,     ///
	} status; // TODO rename meshing_rule

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

	/// Normal vector to the edge "from the inner side to the outer side"
	/// of the polygon.
	///*************************************************************************
	enum class Normal {
		XMIN,
		XMAX,
		YMIN,
		YMAX,
		NONE
	} normal;

	double res_factor;

//	std::vector<Conflict*> conflicts;

	Edge(Point const* _p0, Point const* _p1, Status _status = Status::THIRDS);
	void print() const;
};
