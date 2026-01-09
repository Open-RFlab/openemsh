///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "edit_model.hpp"

namespace domain {
class ConflictDiagonalOrCircularZone;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class EditModelConflictDiagonalOrCircularZone : public EditModel {
public:
	explicit EditModelConflictDiagonalOrCircularZone(domain::ConflictDiagonalOrCircularZone* conflict, QObject* parent = nullptr);
	void commit() override;

private:
	domain::ConflictDiagonalOrCircularZone* conflict;
};

} // namespace ui::qt
