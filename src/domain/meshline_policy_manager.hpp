///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <array>
#include <memory>
#include <optional>
#include <vector>

#include "geometrics/space.hpp"
#include "mesh/interval.hpp"
#include "mesh/meshline.hpp"
#include "mesh/meshline_policy.hpp"
#include "utils/state_management.hpp"

namespace domain {

class ConflictManager;

#ifdef UNITTEST
#define private public
#endif // UNITTEST

//******************************************************************************
struct MeshlinePolicyManagerState final {
	AxisSpace<std::vector<std::shared_ptr<MeshlinePolicy>>> line_policies;
	AxisSpace<std::vector<std::shared_ptr<Meshline>>> meshlines;
	AxisSpace<std::vector<std::shared_ptr<Interval>>> intervals;
};

//******************************************************************************
class MeshlinePolicyManager // TODO MeshlineManager
: public Originator<MeshlinePolicyManagerState> {
private:
	GlobalParams* global_params;
	ConflictManager* conflict_manager;

public:
	MeshlinePolicyManager(GlobalParams* global_params, Timepoint* t);
	MeshlinePolicyManager(
		GlobalParams* global_params,
		AxisSpace<std::vector<std::shared_ptr<MeshlinePolicy>>>&& line_policies,
		Timepoint* t);

	void init(ConflictManager* conflict_manager);

	MeshlinePolicy* add_meshline_policy(
		std::vector<IMeshLineOrigin*> origins,
		Axis const axis,
		MeshlinePolicy::Policy const policy,
		MeshlinePolicy::Normal const normal,
		Coord const coord,
		bool const is_enabled = true,
		Timepoint* t = nullptr);

	void detect_and_solve_too_close_meshline_policies(Axis const axis);
	void detect_intervals(Axis const axis);
	void mesh(Axis const axis);

	void detect_and_solve_too_close_meshline_policies() { for(auto const& axis : AllAxis) detect_and_solve_too_close_meshline_policies(axis); };
	void detect_intervals() { for(auto const& axis : AllAxis) detect_intervals(axis); };
	void mesh() { for(auto const& axis : AllAxis) mesh(axis); };

	std::vector<std::shared_ptr<Meshline>> get_meshline_policies_meshlines(Axis axis) const;
	std::vector<std::shared_ptr<Meshline>> const& get_meshlines(Axis axis) const;
	std::vector<std::shared_ptr<MeshlinePolicy>> const& get_meshline_policies(Axis axis) const;
	std::vector<std::shared_ptr<Interval>> const& get_intervals(Axis axis) const;
};

#ifdef UNITTEST
#undef private
#endif // UNITTEST

//******************************************************************************
std::optional<std::array<MeshlinePolicy*, 2>> detect_closest_meshline_policies(
	std::vector<MeshlinePolicy*> dimension,
	Coord proximity_limit);

} // namespace domain
