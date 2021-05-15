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
		Coord const _coord,
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
	case Segment::Axis::H: return MeshlinePolicy::Axis::H;
	case Segment::Axis::V: return MeshlinePolicy::Axis::V;
	default: return nullopt;
	}
}

//******************************************************************************
Coord coord(Point const& point, MeshlinePolicy::Axis const axis) {
	switch(axis) {
	case MeshlinePolicy::Axis::H: return point.y;
	case MeshlinePolicy::Axis::V: return point.x;
	}
}
