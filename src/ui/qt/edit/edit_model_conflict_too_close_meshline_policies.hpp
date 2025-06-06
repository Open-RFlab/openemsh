///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "edit_model.hpp"

namespace domain {
class ConflictTooCloseMeshlinePolicies;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class EditModelConflictTooCloseMeshlinePolicies : public EditModel {
public:
	explicit EditModelConflictTooCloseMeshlinePolicies(domain::ConflictTooCloseMeshlinePolicies* conflict, QObject* parent = nullptr);
	void commit() override;

private:
	domain::ConflictTooCloseMeshlinePolicies* conflict;
};

} // namespace ui::qt
