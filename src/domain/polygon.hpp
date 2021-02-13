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

#include "edge.hpp"
#include "point.hpp"

using namespace std;

//******************************************************************************
enum { XMIN, XMAX, YMIN, YMAX };

//******************************************************************************
class Polygon {
public:
	enum class Rotation {
		UNKNOWN,
		CW,
		CCW
	} rotation;

	enum class Type {
		SHAPE,
		PORT
	} type; // TODO usefull?

	std::array<double, 4> bounding;
	std::vector<std::unique_ptr<Point const>> points;
	std::vector<std::unique_ptr<Edge>> edges;
//	std::array<std::unique_ptr<Point>, 4> bounding;

	Polygon(Rotation _rotation, std::initializer_list<Point> _points);
	inline void detect_rotation();
	inline void detect_edge_normal();

	void print() const;
};
