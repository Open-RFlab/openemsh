///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "utils/unreachable.hpp"

#include "space.hpp"

namespace domain {

using namespace std;

//******************************************************************************
static_assert(Axes[Plane::YZ][0] == Axis::Y);
static_assert(Axes[Plane::YZ][1] == Axis::Z);
static_assert(Axes[Plane::ZX][0] == Axis::Z);
static_assert(Axes[Plane::ZX][1] == Axis::X);
static_assert(Axes[Plane::XY][0] == Axis::X);
static_assert(Axes[Plane::XY][1] == Axis::Y);

//******************************************************************************
optional<ViewAxis> transpose(Plane const plane, Axis const axis) noexcept {
	switch(plane) {
	case YZ:
		switch(axis) {
		case X: return nullopt;
		case Y: return H;
		case Z: return V;
		default: unreachable();
		}
	case ZX:
		switch(axis) {
		case X: return V;
		case Y: return nullopt;
		case Z: return H;
		default: unreachable();
		}
	case XY:
		switch(axis) {
		case X: return H;
		case Y: return V;
		case Z: return nullopt;
		default: unreachable();
		}
	default:
		unreachable();
	}
}

//******************************************************************************
Axis transpose(Plane const plane, ViewAxis const axis) noexcept {
	switch(axis) {
	case H:
		switch(plane) {
		case YZ: return Y;
		case ZX: return Z;
		case XY: return X;
		default: unreachable();
		}
	case V:
		switch(plane) {
		case YZ: return Z;
		case ZX: return X;
		case XY: return Y;
		default: unreachable();
		}
	default:
		unreachable();
	}
}

//******************************************************************************
ViewAxis reverse(ViewAxis const axis) noexcept {
	switch(axis) {
	case H: return V;
	case V: return H;
	default: unreachable();
	}
}

} // namespace domain
