///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <algorithm>
#include <limits>

#include "domain/geometrics/normal.hpp"
#include "utils/unreachable.hpp"
#include "utils/vector_utils.hpp"
#include "conflict_manager.hpp"

#include "meshline_policy_manager.hpp"

namespace domain {

using namespace std;

//******************************************************************************
MeshlinePolicyManager::MeshlinePolicyManager(Params& params, ConflictManager* conflict_manager)
: params(params)
, conflict_manager(conflict_manager)
{}

//******************************************************************************
MeshlinePolicy* MeshlinePolicyManager::add_meshline_policy(
		IMeshLineOrigin* origin,
		Axis const axis,
		MeshlinePolicy::Policy const policy,
		MeshlinePolicy::Normal const normal,
		Coord const coord,
		bool const is_enabled) {

	if((policy == MeshlinePolicy::Policy::THIRDS && normal == MeshlinePolicy::Normal::NONE)
	|| (policy != MeshlinePolicy::Policy::THIRDS && normal != MeshlinePolicy::Normal::NONE))
		return nullptr;

	unique_ptr<MeshlinePolicy>& line_policy = line_policies[axis].emplace_back(make_unique<MeshlinePolicy>(
		axis, policy, normal, params, coord, is_enabled));

	line_policy->origins.emplace_back(origin);

	return line_policy.get();
}

//******************************************************************************
optional<array<MeshlinePolicy*, 2>> detect_closest_meshline_policies(
		vector<MeshlinePolicy*> dimension,
		Coord proximity_limit) {

	dimension.erase(remove_if(begin(dimension), end(dimension),
		[](MeshlinePolicy const* a) {
			return (!a->is_enabled)
			    || a->policy == MeshlinePolicy::Policy::ONELINE;
		}),
		end(dimension));

	sort(begin(dimension), end(dimension),
		[](MeshlinePolicy const* a, MeshlinePolicy const* b) {
			return a->coord < b->coord;
		});

	Coord smallest_space(numeric_limits<double>::infinity());
	array<MeshlinePolicy*, 2> closest;

	for(size_t i = 1; i < dimension.size(); ++i) {
		Coord space(abs((double) (dimension[i]->coord - dimension[i-1]->coord)));
		if(space < smallest_space) {
			smallest_space = space;
			closest = { dimension[i-1], dimension[i] };
		}
	}

	if(smallest_space <= proximity_limit)
		return closest;
	else
		return nullopt;
}

//******************************************************************************
void MeshlinePolicyManager::detect_and_solve_too_close_meshline_policies(Axis const axis) {
	while(true) {
		auto closest = detect_closest_meshline_policies(create_view(line_policies[axis]), params.proximity_limit);
		if(!closest)
			break;

		ConflictTooCloseMeshlinePolicies* conflict = conflict_manager->add_too_close_meshline_policies(closest->front(), closest->back());
		if(conflict)
			conflict->auto_solve(*this);
	}
}

//******************************************************************************
void MeshlinePolicyManager::detect_intervals(Axis const axis) {
	auto dimension = create_view(line_policies[axis]);

	dimension.erase(remove_if(begin(dimension), end(dimension),
		[](MeshlinePolicy const* a) {
			return (!a->is_enabled);
		}),
		end(dimension));

	sort(begin(dimension), end(dimension),
		[](MeshlinePolicy const* a, MeshlinePolicy const* b) {
			return a->coord < b->coord;
		});

	for(size_t i = 1; i < dimension.size(); ++i) {
		unique_ptr<Interval> const& interval = intervals[axis].emplace_back(make_unique<Interval>(
			dimension[i-1], dimension[i], axis, params));
		// TODO add links MLP -> I ?
	}
}

//******************************************************************************
void MeshlinePolicyManager::mesh(Axis const axis) {
	auto dimension_view = create_view(intervals[axis]);

	sort(begin(dimension_view), end(dimension_view),
		[](Interval const* a, Interval const* b) {
			return a->h < b->h;
		});

	for(auto interval : dimension_view)
		interval->auto_solve_d();

	size_t new_size = meshlines[axis].size();
	vector<vector<unique_ptr<Meshline>>> interval_meshlines;

	for(auto* interval : dimension_view) {
		interval->auto_solve_d();
		interval->auto_solve_lambda();
		interval_meshlines.emplace_back(interval->mesh());

		new_size += interval_meshlines.back().size();
	}

	for(auto const& line_policy : line_policies[axis]) {
		if(auto meshline = line_policy->mesh(); meshline) {
			vector<unique_ptr<Meshline>> v;
			v.push_back(make_unique<Meshline>(meshline.value()));
			interval_meshlines.emplace_back(move(v));
			++new_size;
		}
	}

	meshlines[axis].reserve(new_size);
	for(auto& it : interval_meshlines) {
		move(begin(it), end(it), back_inserter(meshlines[axis]));
	}

	sort(begin(meshlines[axis]), end(meshlines[axis]),
		[](unique_ptr<Meshline> const& a, unique_ptr<Meshline> const& b) {
			return *a < *b;
		});
}

//******************************************************************************
vector<unique_ptr<Meshline>> const& MeshlinePolicyManager::get_meshlines(Axis axis) const {
	return meshlines[axis];
}

//******************************************************************************
vector<unique_ptr<Meshline>> MeshlinePolicyManager::get_meshline_policies_meshlines(Axis axis) const {
	vector<unique_ptr<Meshline>> mesh;
	for(auto const& line_policy : line_policies[axis])
		if(line_policy->is_enabled)
			mesh.emplace_back(make_unique<Meshline>(line_policy->coord, nullptr, nullptr));
	return mesh;
}

//******************************************************************************
vector<unique_ptr<MeshlinePolicy>> const& MeshlinePolicyManager::get_meshline_policies(Axis axis) const {
	return line_policies[axis];
}

//******************************************************************************
vector<unique_ptr<Interval>> const& MeshlinePolicyManager::get_intervals(Axis axis) const {
	return intervals[axis];
}

} // namespace domain
