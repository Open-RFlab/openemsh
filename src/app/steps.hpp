///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <optional>

#include "utils/unreachable.hpp"

namespace app {

//******************************************************************************
enum class Step {
	ADJUST_EDGE_TO_MATERIAL,
	DETECT_CONFLICT_EIP,
	DETECT_CONFLICT_CE,
	ADD_FIXED_MLP,
	SOLVE_ALL_EIP,
	SOLVE_ALL_CE,
	DETECT_INDIVIDUAL_EDGES,
	DETECT_AND_SOLVE_TCMLP,
	DETECT_INTERVALS,
	MESH
};

// TODO refactor with ITERABLE_ENUM
//******************************************************************************
inline std::size_t index(std::optional<Step> step) {
	if(!step.has_value()) return 0;

	switch(step.value()) {
	case Step::ADJUST_EDGE_TO_MATERIAL: return 1;
	case Step::DETECT_CONFLICT_EIP: return 2;
	case Step::DETECT_CONFLICT_CE: return 3;
	case Step::ADD_FIXED_MLP: return 4;
	case Step::SOLVE_ALL_EIP: return 5;
	case Step::SOLVE_ALL_CE: return 6;
	case Step::DETECT_INDIVIDUAL_EDGES: return 7;
	case Step::DETECT_AND_SOLVE_TCMLP: return 8;
	case Step::DETECT_INTERVALS: return 9;
	case Step::MESH: return 10;
	default: ::unreachable();
	}
}

//******************************************************************************
inline std::size_t index_max() {
	return index(Step::MESH);
}

} // namespace app
