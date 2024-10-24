///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

namespace domain {
namespace relation {

/// Relation between edges.
///*****************************************************************************
enum class SegmentSegment {
	APART,         ///< Edges do not intersect nor colinear overlap.
	CROSSING,      ///< Edges intersect.
	COLINEAR,      ///< Edges are colinear.
	OVERLAPPING    ///< Edges are colinear and overlapping. OVERLAPPING
};

/// Relation between a point and an edge.
///*****************************************************************************
enum class SegmentPoint {
	OUT,    ///< Point not on the edge.
	ON      ///< Point on the edge.
};

/// Relation between an edge and a polygon.
///*****************************************************************************
enum class PolygonSegment {
	IN,    ///< Edge totally inside the polygon.
	ON,    ///< Edge COLINEAR_CROSSING an edge of the polygon.
	OUT    ///< Edge totally outside the polygon.
};

/// Relation between a point and a polygon.
///*****************************************************************************
enum class PolygonPoint {
	IN,    ///< Point inside the polygon.
	ON,    ///< Point on an edge or a point of the polygon.
	OUT    ///< Point outside the polygon.
};

//******************************************************************************
//PolygonPoint cast(PolygonSegment const a);

//******************************************************************************
//PolygonSegment cast(PolygonPoint const a);

//******************************************************************************
//void toggle(PolygonSegment& a);

//******************************************************************************
//void toggle(PolygonPoint& a);

} // namespace relation
} // namespace domain
