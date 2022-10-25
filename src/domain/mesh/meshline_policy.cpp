///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <cstdlib>

#include "domain/geometrics/point.hpp"
#include "meshline.hpp"

#include "meshline_policy.hpp"

using namespace std;

//******************************************************************************
MeshlinePolicy::MeshlinePolicy(
		Axis const axis,
		Policy const policy,
		Normal normal,
		Params& params,
		Coord const coord,
		bool const is_enabled,
		double const res_factor)
: axis(axis)
, policy(policy)
, normal(normal)
, params(params)
, coord(coord)
, is_enabled(is_enabled)
, res_factor(res_factor)
, d(params.dmax / res_factor)
{}

//******************************************************************************
optional<Meshline> MeshlinePolicy::mesh() {
	if(policy == Policy::ONELINE)
		return Meshline(coord, this);
	else
		return nullopt;
}

//******************************************************************************
optional<MeshlinePolicy::Axis> cast(Segment::Axis const a) noexcept {
	switch(a) {
	case Segment::Axis::H: return MeshlinePolicy::Axis::H;
	case Segment::Axis::V: return MeshlinePolicy::Axis::V;
	default: return nullopt;
	}
}

//******************************************************************************
Coord coord(Point const& point, MeshlinePolicy::Axis const axis) noexcept {
	switch(axis) {
	case MeshlinePolicy::Axis::H: return point.y;
	case MeshlinePolicy::Axis::V: return point.x;
	default: abort();
	}
}
