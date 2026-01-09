///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <vector>

#include "domain/global.hpp"
#include "domain/geometrics/bounding.hpp"
#include "domain/geometrics/space.hpp"
#include "utils/state_management.hpp"
#include "conflict.hpp"

namespace domain {

class Angle;
class Interval;

//******************************************************************************
struct ConflictDiagonalOrCircularZoneState final : public ConflictState {
	double dmax;
	std::size_t lmin;
	double minimal_angle;
	std::vector<Angle*> angles;
	std::vector<Interval*> intervals;
};

//******************************************************************************
class ConflictDiagonalOrCircularZone
: public Originator<ConflictDiagonalOrCircularZoneState const>
, public Visitable<ConflictDiagonalOrCircularZone, EntityVisitor>
, public Conflict {
private:
	GlobalParams* global_params;

public:
	Axis const axis;

	ConflictDiagonalOrCircularZone(Axis axis, std::vector<Angle*> const& angles, GlobalParams* global_params, Timepoint* t);

	void append(std::vector<Interval*> const& intervals, Timepoint* t = nullptr);

	Bounding1D bounding() const;

	// TODO Refactor to Avoid that
	void auto_solve(MeshlinePolicyManager& line_policy_manager) override;
	void solve_angles();
	void solve_intervals(MeshlinePolicyManager& line_policy_manager);
};

} // namespace domain
