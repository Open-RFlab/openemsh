///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

//******************************************************************************
struct Params {
	double metal_res;
	double substrate_res;
	double proximity_limit;
	double lambda = 2;
	size_t lmin = 10;
	double dmax = 2.5;
};

inline double equality_tolerance = 1e-8;
