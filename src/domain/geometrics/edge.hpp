///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <array>
#include <optional>
#include <vector>

//#include "conflict.hpp"
//#include "meshline_manager.hpp"
//#include "point.hpp"
#include "domain/conflicts/i_conflict_origin.hpp"
#include "domain/conflicts/i_conflict_solution.hpp"
#include "domain/mesh/i_meshline_origin.hpp"
#include "bounding.hpp"
#include "range.hpp"
#include "relation.hpp"
#include "segment.hpp"
#include "types.hpp"

class Conflict;
class Point;
//class Range;

#ifdef UNITTEST
#define private public
#endif // UNITTEST

//******************************************************************************
class Edge : public Segment, public IConflictOrigin, public IConflictSolution, public IMeshLineOrigin {
private:
	Point const* const _p0;
	Point const* const _p1;

public:
//	bool is_enabled;

	/// Vector from p0 to p1.
	///*************************************************************************
	Point const vec;
	// TODO vec & normal -> std::complex
	// enums -> funcs? vars? enums?

	/// Direction from p0 to p1.
	///*************************************************************************
	enum class Direction {
		XMIN,
		XMAX,
		YMIN,
		YMAX,
		NONE
	} direction;

	Normal normal;
	bool to_mesh;

//	Bounding2D bounding;

	Edge(Point const* p0, Point const* p1);

	Point const& p0() const noexcept override;
	Point const& p1() const noexcept override;

#ifdef DEBUG
	void print() const;
#endif // DEBUG
};

#ifdef UNITTEST
#undef private
#endif // UNITTEST

//******************************************************************************
std::optional<Range> merge(Edge const& a, Edge const& b) = delete;

//******************************************************************************
bool operator==(Range const& a, Edge const& b) noexcept;
bool operator==(Edge const& a, Range const& b) noexcept;
