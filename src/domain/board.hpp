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
#include "edge.hpp"
#include "global.hpp"
#include "meshline_manager.hpp"
#include "point.hpp"
#include "polygon.hpp"

//******************************************************************************
class Board {
private:
	std::vector<std::unique_ptr<Polygon>> polygons;
	std::array<std::vector<std::unique_ptr<MeshlineManager>>, 2> line_managers;
	std::vector<std::unique_ptr<Conflict>> conflicts;
	std::vector<Edge*> edges;
	Params params;

public:
//	Board(std::initializer_list<Polygon> _polygons);
	Board(std::vector<unique_ptr<Polygon>>& _polygons);

	void detect_disabled_edges();
	void detect_halfs_rule_edges();
	void print();
};
