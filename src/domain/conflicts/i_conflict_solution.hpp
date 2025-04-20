///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

namespace domain {

class Conflict;

//******************************************************************************
struct IConflictSolutionState {
	Conflict const* conflict = nullptr; // TODO rename like conflict_from
};

//******************************************************************************
class IConflictSolution {
public:
	virtual ~IConflictSolution() = default;
};

} // namespace domain
