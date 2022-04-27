///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

//******************************************************************************
class IConflictSolution {
public:
	Conflict const* conflict; // TODO rename like conflict_from

	virtual ~IConflictSolution() = default;
};
