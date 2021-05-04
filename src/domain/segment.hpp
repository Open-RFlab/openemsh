///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <array>
#include <optional>

#include "relation.hpp"
#include "types.hpp"

class Point;
class Range;

//******************************************************************************
class Segment {
public:
	enum class Axis {
		X,
		Y,
		DIAGONAL,
		POINT
	} axis;

	Segment() = default;
	virtual ~Segment() = default;

	virtual Point const& p0() const = 0;
	virtual Point const& p1() const = 0;

	relation::SegmentSegment relation_to(Segment const& segment) const;
	relation::SegmentPoint relation_to(Point const& point) const;
};

//******************************************************************************
Bounding bounding(Segment const& a);

//******************************************************************************
std::optional<Point> intersection(Segment const& a, Segment const& b);

//******************************************************************************
std::optional<Range> merge(Segment const& a, Segment const& b);

//******************************************************************************
Point mid(Segment const& s);

//******************************************************************************
std::optional<Range> overlap(Segment const& a, Segment const& b);
