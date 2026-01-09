///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "edit_model.hpp"

namespace domain {
class Angle;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class EditModelAngle : public EditModel {
public:
	explicit EditModelAngle(domain::Angle* angle, QObject* parent = nullptr);
	void commit() override;

private:
	domain::Angle* angle;
};

} // namespace ui::qt
