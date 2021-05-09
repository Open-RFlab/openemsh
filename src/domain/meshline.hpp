///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

class MeshlinePolicy;

//******************************************************************************
class Meshline {
public:
	double coord;
	MeshlinePolicy* const policy;

	Meshline(double _coord, MeshlinePolicy* const _policy);
};

//******************************************************************************
bool operator<(Meshline const& a, Meshline const& b);
