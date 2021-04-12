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

#include "conflict.hpp"
#include "conflict_manager.hpp"
#include "edge.hpp"
#include "global.hpp"
#include "meshline_manager.hpp"
#include "point.hpp"
#include "polygon.hpp"
//#include "range.hpp"

#ifdef UNITTEST
#define private public
#endif // UNITTEST

//******************************************************************************
class Board {
private:
	ConflictManager conflict_manager;
	std::vector<std::unique_ptr<Polygon>> polygons;
	std::array<std::vector<std::unique_ptr<MeshlineManager>>, 2> line_managers;
	std::vector<std::unique_ptr<Conflict>> conflicts;
	std::vector<Edge*> edges;
	Params params;

public:
//	Board(std::initializer_list<Polygon> _polygons);
	Board(std::vector<std::unique_ptr<Polygon>>& _polygons);

	void detect_edges_in_polygons();
	void detect_colinear_edges();

#ifdef DEBUG
	void print();
#endif // DEBUG
};

#ifdef UNITTEST
#undef private

void sort_points_by_vector_orientation(std::vector<Point>& points, Point const& vector);
#endif // UNITTEST
