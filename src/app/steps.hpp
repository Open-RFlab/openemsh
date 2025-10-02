///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

namespace app {

//******************************************************************************
enum class Step {
	ADJUST_EDGE_TO_MATERIAL,
	DETECT_CONFLICT_EIP,
	DETECT_CONFLICT_CE,
	DETECT_NON_CONFLICTING_EDGES,
	ADD_FIXED_MLP,
	SOLVE_ALL_EIP,
	SOLVE_ALL_CE,
	DETECT_AND_SOLVE_TCMLP,
	DETECT_INTERVALS,
	MESH
};

} // namespace app
