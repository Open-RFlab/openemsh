///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "edit_model.hpp"

namespace domain {
class GlobalParams;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class EditModelGlobal : public EditModel {
public:
	explicit EditModelGlobal(domain::GlobalParams* global, QObject* parent = nullptr);
	void commit() override;

private:
	domain::GlobalParams* global;
};

} // namespace ui::qt
