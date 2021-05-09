///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "meshline.hpp"
#include "point.hpp"

#include "meshline_policy.hpp"

using namespace std;

//******************************************************************************
MeshlinePolicy::MeshlinePolicy(
		Axis const _axis,
		Policy const _policy,
		Normal _normal,
		Params& _params,
		double const _coord,
		bool const _is_enabled,
		double const _res_factor)
: axis(_axis)
, policy(_policy)
, normal(_normal)
, params(_params)
, coord(_coord)
, is_enabled(_is_enabled)
, res_factor(_res_factor)
{}

//******************************************************************************
Meshline MeshlinePolicy::mesh() {
	return Meshline(coord, this);
}

//******************************************************************************
optional<MeshlinePolicy::Axis> cast(Segment::Axis const a) {
	switch(a) {
	case Segment::Axis::X: return MeshlinePolicy::Axis::X;
	case Segment::Axis::Y: return MeshlinePolicy::Axis::Y;
	default: return nullopt;
	}
}

//******************************************************************************
double coord(Point const& point, MeshlinePolicy::Axis const axis) {
	switch(axis) {
	case MeshlinePolicy::Axis::X: return point.x;
	case MeshlinePolicy::Axis::Y: return point.y;
	}
}