///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <limits>

#include "domain/geometrics/angle.hpp"
#include "domain/mesh/interval.hpp"

#include "conflict_diagonal_or_circular_zone.hpp"

namespace domain {

using namespace std;

// Assume sorted angles by axis-relevant coord.
//******************************************************************************
ConflictDiagonalOrCircularZone::ConflictDiagonalOrCircularZone(Axis axis, vector<Angle*> const& angles, GlobalParams* global_params, Timepoint* t)
: Originator(t, {
	.dmax = global_params->get_current_state().diagonal_dmax,
	.lmin = global_params->get_current_state().lmin,
	.minimal_angle = global_params->get_current_state().consecutive_diagonal_minimal_angle,
	.angles = angles })
, Conflict(Kind::DIAGONAL_OR_CIRCULAR_ZONE)
, global_params(global_params)
, axis(axis)
{}

//******************************************************************************
void ConflictDiagonalOrCircularZone::append(vector<Interval*> const& intervals, Timepoint* t) {
	auto state = get_current_state();
	state.intervals.insert(state.intervals.end(), intervals.begin(), intervals.end());
	set_given_or_next_state(state, t);
}

// Assume sorted angles.
//******************************************************************************
Bounding1D ConflictDiagonalOrCircularZone::bounding() const {
	auto state = get_current_state();

	auto const get_coord = [this](Angle const* angle) {
		if(auto view_axis = transpose(angle->plane, axis)
		; view_axis.has_value())
			return coord(angle->p, view_axis.value());
		return Coord(numeric_limits<double>::signaling_NaN());
	};

	return { get_coord(state.angles.front()), get_coord(state.angles.back()) };
}

//******************************************************************************
void ConflictDiagonalOrCircularZone::auto_solve(MeshlinePolicyManager& line_policy_manager) {
}

// Assume sorted angles
//******************************************************************************
void ConflictDiagonalOrCircularZone::solve_angles() {
	auto [t, state] = make_next_state();

	// Always enable extremities
	for(Angle* angle : { state.angles.front(), state.angles.back() }) {
		if(auto view_axis = transpose(angle->plane, axis)
		; view_axis.has_value()) {
			auto state_a = angle->get_current_state();
			state_a.to_mesh[view_axis.value()] = true;
			angle->set_state(t, state_a);
		}
	}

	// Enable inner angles if acute enough
	for(Angle* angle : state.angles | views::drop(1) | views::reverse | views::drop(1) | views::reverse) {
		if(auto view_axis = transpose(angle->plane, axis)
		; view_axis.has_value()) {
			auto state_a = angle->get_current_state();
			state_a.to_mesh[view_axis.value()] = angle->angle >= global_params->get_current_state().consecutive_diagonal_minimal_angle;
			angle->set_state(t, state_a);
		}
	}
}

//******************************************************************************
void ConflictDiagonalOrCircularZone::solve_intervals() {
	auto [t, state] = make_next_state();

	// TODO lmin for extremities
	// front()->before & back()->after

	for(Interval* interval : state.intervals) {
		auto state_i = interval->get_current_state();
		state_i.dmax = state.dmax;
		interval->set_state(t, state_i);
	}
}


} // namespace domain
