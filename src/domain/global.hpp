///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <cstddef>
#include <utility>
#include <vector>

#include "geometrics/space.hpp"
#include "utils/state_management.hpp"

namespace domain {

//******************************************************************************
struct Params {
	bool has_grid_already = false; // TODO would better fit in infra layer?
	double proximity_limit = 1; // TODO must be linked to initial d
	double smoothness = 2;
	std::size_t lmin = 2;
	double dmax = 2.5;

	std::size_t diagonal_lmin = 2;
	double diagonal_dmax = 0.2;
	double consecutive_diagonal_minimal_angle = 20; // Limite between acute / obtuse angles.

	std::vector<std::pair<Axis, double>> input_fixed_meshlines;
};

//******************************************************************************
class GlobalParams : public Originator<Params const> {
public:
	explicit GlobalParams(Timepoint* t) : Originator(t) {}
	GlobalParams(Params params, Timepoint* t) : Originator(t, std::move(params)) {}
};

inline double equality_tolerance = 1e-8;

} // namespace domain
