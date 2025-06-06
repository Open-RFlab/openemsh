///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "edit_model.hpp"

namespace domain {
class Interval;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class EditModelInterval : public EditModel {
public:
	explicit EditModelInterval(domain::Interval* interval, QObject* parent = nullptr);
	void commit() override;

private:
	domain::Interval* interval;
};

} // namespace ui::qt
