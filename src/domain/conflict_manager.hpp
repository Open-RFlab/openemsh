///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <memory>
#include <vector>

#include "conflict.hpp"

class Edge;
class Range;
class Polygon;

/// Create / append conflicts regarding already existing conflicts
///*****************************************************************************
class ConflictManager {
private:
	std::vector<std::unique_ptr<Conflict>>& conflicts;
public:
	ConflictManager(std::vector<std::unique_ptr<Conflict>>& _conflicts);
	void add_colinear_edges(Edge* a, Edge* b);
	void add_edge_in_polygon(Edge* _edge, Polygon* _polygon, Range const* _range); // TODO
};