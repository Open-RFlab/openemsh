///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <initializer_list>
#include <memory>
#include <vector>

#include "edge.hpp"
#include "polygon.hpp"

//******************************************************************************
class Board {
private:
	std::vector<std::unique_ptr<Polygon>> polygons;
//	std::vector<std::unique_ptr<Conflict> conflicts;
	std::vector<Edge*> edges;

	struct Params {
		double metal_res;
		double substrate_res;
	} params;

public:
//	Board(std::initializer_list<Polygon> _polygons);
	Board(std::vector<unique_ptr<Polygon>>& _polygons);

	void detect_disabled_edges(); // TODO
	void detect_halfs_rule_edges();
	void print();
};
