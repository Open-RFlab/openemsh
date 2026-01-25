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
		bool const is_enabled)
: Originator(t, {
	.policy = policy,
	.normal = normal,
	.is_enabled = is_enabled,
	.d = global_params->get_current_state().dmax / 2, // TODO this seems to help while center of intervals is buggy
	.origins = origins
})
, axis(axis)
, global_params(global_params)
, coord(coord)
{}

//******************************************************************************
optional<Meshline> MeshlinePolicy::mesh() {
	if(get_current_state().policy == Policy::ONELINE)
		return Meshline(coord, this);
	else
		return nullopt;
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
		::unreachable();
	}
}

} // namespace domain
