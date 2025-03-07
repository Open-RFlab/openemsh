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
