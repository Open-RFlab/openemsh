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

#include "mesh/meshline.hpp"
#include "mesh/meshline_policy.hpp"

class ConflictManager;
class Meshline;

#ifdef UNITTEST
#define private public
#endif // UNITTEST

//******************************************************************************
template<typename T>
using Grid = typename std::array<T, 2>;
enum GridAxis { H, V };

//******************************************************************************
class MeshlinePolicyManager { // TODO MeshlineManager
private:
	Params& params;
	ConflictManager* const conflict_manager;

	Grid<std::vector<std::unique_ptr<MeshlinePolicy>>> line_policies;
	Grid<std::vector<std::unique_ptr<Meshline>>> meshlines;


public:
	MeshlinePolicyManager(Params& params, ConflictManager* conflict_manager);

	MeshlinePolicy* add_meshline_policy(
		IMeshLineOrigin* origin,
		MeshlinePolicy::Axis const axis,
		MeshlinePolicy::Policy const policy,
		Normal const normal,
		Coord const coord,
		bool const is_enabled = true);

	void mesh(MeshlinePolicy& policy);

	void detect_and_solve_too_close_meshline_policies();
	void detect_intervals();
	void mesh();
};

#ifdef UNITTEST
#undef private
#endif // UNITTEST

//******************************************************************************
std::optional<std::array<MeshlinePolicy*, 2>> detect_closest_meshline_policies(
	std::vector<MeshlinePolicy*> dimension,
	Coord proximity_limit);

//******************************************************************************
GridAxis cast(MeshlinePolicy::Axis const a) noexcept;
