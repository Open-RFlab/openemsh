///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <algorithm>
#include <limits>

#include "domain/geometrics/normal.hpp"
#include "infra/utils/to_string.hpp"
#include "utils/progress.hpp"
#include "utils/unreachable.hpp"
#include "utils/vector_utils.hpp"
#include "conflict_manager.hpp"

#include "meshline_policy_manager.hpp"

namespace domain {

using namespace std;

//******************************************************************************
MeshlinePolicyManager::MeshlinePolicyManager(GlobalParams* global_params, Timepoint* t)
: Originator(t)
, global_params(global_params)
, conflict_manager(nullptr)
{}

//******************************************************************************
MeshlinePolicyManager::MeshlinePolicyManager(
	GlobalParams* global_params,
	AxisSpace<std::vector<std::shared_ptr<MeshlinePolicy>>>&& line_policies,
	Timepoint* t)
: Originator(t, { .line_policies = std::move(line_policies) })
, global_params(global_params)
, conflict_manager(nullptr)
{
	for(auto const& axis : AllAxis)
		for(auto const& policy : get_current_state().line_policies[axis])
			get_caretaker().take_care_of(policy);
}

//******************************************************************************
void MeshlinePolicyManager::init(ConflictManager* _conflict_manager) {
	conflict_manager = _conflict_manager;
}

//******************************************************************************
MeshlinePolicy* MeshlinePolicyManager::add_meshline_policy(
		vector<IMeshLineOrigin*> origins,
		Axis const axis,
		MeshlinePolicy::Policy const policy,
		MeshlinePolicy::Normal const normal,
		Coord const coord,
		bool const is_enabled,
		Timepoint* t) {

	if((policy == MeshlinePolicy::Policy::THIRDS && normal == MeshlinePolicy::Normal::NONE)
	|| (policy != MeshlinePolicy::Policy::THIRDS && normal != MeshlinePolicy::Normal::NONE))
		return nullptr;

	auto state = get_current_state();

	auto const& line_policy = state.line_policies[axis].emplace_back(make_shared<MeshlinePolicy>(
		axis, policy, normal, global_params, coord, t, origins, is_enabled));
	get_caretaker().take_care_of(line_policy);

	set_given_or_next_state(state, t);
	return line_policy.get();
}

//******************************************************************************
optional<array<MeshlinePolicy*, 2>> detect_closest_meshline_policies(
		vector<MeshlinePolicy*> dimension,
		Coord proximity_limit) {

	erase_if(dimension,
		[](MeshlinePolicy const* a) {
			return (!a->get_current_state().is_enabled)
			    || a->get_current_state().policy == MeshlinePolicy::Policy::ONELINE;
		});

	ranges::sort(dimension,
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
	auto possible_max = get_current_state().line_policies[axis].size();
	auto [bar, found, i] = Progress::Bar::build(
		possible_max,
		"["s + to_string(axis) + "] Detecting & solving TCMLP conflicts ");

	while(true) {
		++i;
		auto closest = detect_closest_meshline_policies(create_view(get_current_state().line_policies[axis]), global_params->get_current_state().proximity_limit);
		if(!closest)
			break;

		ConflictTooCloseMeshlinePolicies* conflict = conflict_manager->add_too_close_meshline_policies(closest->front(), closest->back());
		if(conflict) {
			conflict->auto_solve(*this);
			bar.tick(++found, i);
		}
	}

	bar.tick(found, possible_max);
	bar.complete();
}

//******************************************************************************
void MeshlinePolicyManager::detect_intervals(Axis const axis) {
	auto [t, state] = make_next_state();

	auto dimension = create_view(state.line_policies[axis]);

	erase_if(dimension,
		[](MeshlinePolicy const* a) {
			return (!a->get_current_state().is_enabled);
		});

//	// TODO C++26 already implemented
//	auto [bar, _, _] = Progress::Bar::build(
	auto [bar, _, __] = Progress::Bar::build(
		dimension.size(),
		"["s + to_string(axis) + "] Detecting Intervals ");

	ranges::sort(dimension,
		[](MeshlinePolicy const* a, MeshlinePolicy const* b) {
			return a->coord < b->coord;
		});

	for(size_t i = 1; i < dimension.size(); ++i) {
		auto const& interval = state.intervals[axis].emplace_back(make_shared<Interval>(
			dimension[i-1], dimension[i], axis, global_params, t));
		get_caretaker().take_care_of(interval);
		bar.tick(i);
		// TODO add links MLP -> I ?
	}

	set_state(t, state);
	bar.complete();
}

//******************************************************************************
void MeshlinePolicyManager::mesh(Axis const axis) {
	auto [t, state] = make_next_state();

	auto dimension_view = create_view(state.intervals[axis]);

	auto [bar, i, _] = Progress::Bar::build(
		dimension_view.size() + state.line_policies[axis].size() + 1,
		"["s + to_string(axis) + "] Meshing Intervals + Meshline Policies ");

	ranges::sort(dimension_view,
		[](Interval const* a, Interval const* b) {
			return a->h < b->h;
		});

	for(auto interval : dimension_view)
		interval->auto_solve_d();

	size_t new_size = state.meshlines[axis].size();
	vector<vector<shared_ptr<Meshline>>> interval_meshlines;

	for(auto* interval : dimension_view) {
		interval->auto_solve_d();
		interval->auto_solve_smoothness();
		interval_meshlines.emplace_back(interval->mesh());

		new_size += interval_meshlines.back().size();
		bar.tick(++i);
	}

	for(auto const& line_policy : state.line_policies[axis]) {
		if(auto meshline = line_policy->mesh(); meshline) {
			vector<shared_ptr<Meshline>> v;
			v.push_back(make_shared<Meshline>(meshline.value()));
			interval_meshlines.emplace_back(std::move(v));
			++new_size;
		}
		bar.tick(++i);
	}

	state.meshlines[axis].reserve(new_size);
	for(auto& it : interval_meshlines) {
		ranges::move(it, back_inserter(state.meshlines[axis]));
	}

	ranges::sort(state.meshlines[axis],
		[](auto const& a, auto const& b) {
			return *a < *b;
		});

	set_state(t, state);
	bar.complete();
}

//******************************************************************************
vector<shared_ptr<Meshline>> const& MeshlinePolicyManager::get_meshlines(Axis axis) const {
	return get_current_state().meshlines[axis];
}

//******************************************************************************
vector<shared_ptr<Meshline>> MeshlinePolicyManager::get_meshline_policies_meshlines(Axis axis) const {
	vector<shared_ptr<Meshline>> mesh;
	for(auto const& line_policy : get_current_state().line_policies[axis])
		if(line_policy->get_current_state().is_enabled)
			mesh.emplace_back(make_shared<Meshline>(line_policy->coord, nullptr, nullptr));
	return mesh;
}

//******************************************************************************
vector<shared_ptr<MeshlinePolicy>> const& MeshlinePolicyManager::get_meshline_policies(Axis axis) const {
	return get_current_state().line_policies[axis];
}

//******************************************************************************
vector<shared_ptr<Interval>> const& MeshlinePolicyManager::get_intervals(Axis axis) const {
	return get_current_state().intervals[axis];
}

//******************************************************************************
size_t MeshlinePolicyManager::get_mesh_cell_number() const {
	size_t n = 1;
	for(auto const& axis : get_current_state().meshlines)
		n *= axis.size();
	return n;
}

} // namespace domain
