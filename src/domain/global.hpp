///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <cstddef>

#include "utils/state_management.hpp"

namespace domain {

//******************************************************************************
struct Params {
	double metal_res;
	double substrate_res;
	double proximity_limit = 1; // TODO must be linked to initial d
	double lambda = 2;
	std::size_t lmin = 10;
	double dmax = 2.5;
};

//******************************************************************************
class GlobalParams : public Originator<Params> {
public:
	explicit GlobalParams(Timepoint* t) : Originator(t) {}
	GlobalParams(Params params, Timepoint* t) : Originator(t, std::move(params)) {}
};

inline double equality_tolerance = 1e-8;

} // namespace domain
