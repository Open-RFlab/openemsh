///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "edit_model.hpp"

namespace domain {
class MeshlinePolicy;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class EditModelMeshlinePolicy : public EditModel {
public:
	explicit EditModelMeshlinePolicy(domain::MeshlinePolicy* meshline_policy, QObject* parent = nullptr);
	void commit() override;

private:
	domain::MeshlinePolicy* meshline_policy;
};

} // namespace ui::qt
