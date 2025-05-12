///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "domain/geometrics/point.hpp"
#include "utils/unreachable.hpp"
#include "meshline.hpp"

#include "meshline_policy.hpp"

namespace domain {

using namespace std;

//******************************************************************************
MeshlinePolicy::MeshlinePolicy(
		Axis const axis,
		Policy const policy,
		Normal normal,
		GlobalParams* global_params,
		Coord const coord,
		Timepoint* t,
		vector<IMeshLineOrigin*> const& origins,
		bool const is_enabled,
		double const res_factor)
: Originator(t, {
	.is_enabled = is_enabled,
	.res_factor = res_factor,
	.d = global_params->get_current_state().dmax / res_factor,
	.origins = origins
})
, axis(axis)
, policy(policy)
, normal(normal)
, global_params(global_params)
, coord(coord)
{}

//******************************************************************************
optional<Meshline> MeshlinePolicy::mesh() {
	if(policy == Policy::ONELINE)
		return Meshline(coord, this);
	else
		return nullopt;
}

//******************************************************************************
optional<Coord> coord(Point const& point, Segment::Axis const axis) noexcept {
	switch(axis) {
	case Segment::Axis::H: return point.y;
	case Segment::Axis::V: return point.x;
	default: return nullopt;
	}
}

//******************************************************************************
MeshlinePolicy::Normal cast(Normal const normal) noexcept {
	switch(normal) {
	case Normal::XMIN:
	case Normal::YMIN:
	case Normal::ZMIN:
		return MeshlinePolicy::Normal::MIN;
	case Normal::XMAX:
	case Normal::YMAX:
	case Normal::ZMAX:
		return MeshlinePolicy::Normal::MAX;
	case Normal::NONE:
		return MeshlinePolicy::Normal::NONE;
	default:
		unreachable();
	}
}

} // namespace domain
