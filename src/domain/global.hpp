///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

namespace domain {

//******************************************************************************
struct Params {
	double metal_res;
	double substrate_res;
	double proximity_limit = 1; // TODO must be linked to initial d
	double lambda = 2;
	size_t lmin = 10;
	double dmax = 2.5;
};

inline double equality_tolerance = 1e-8;

} // namespace domain
