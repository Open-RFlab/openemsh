///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <algorithm>
#include <array>

#include "app/steps.hpp"
#include "domain/geometrics/angle.hpp"

#include "edit_model_angle.hpp"

namespace ui::qt {

//******************************************************************************
EditModelAngle::EditModelAngle(domain::Angle* angle, QObject* parent)
: EditModel(parent)
, angle(angle)
{
	auto const& state = angle->get_current_state();
	setRowCount(2);

	make_row(0, "To mesh[H]", state.to_mesh[domain::H], "Take into account in the meshing process of the horizontal axis.");
	make_row(1, "To mesh[V]", state.to_mesh[domain::V], "Take into account in the meshing process of the vertical axis.");
}

//******************************************************************************
void EditModelAngle::commit() {
	auto state = angle->get_current_state();

	std::array does_succeed = {
		try_to_bool(item(0, V)->checkState(), state.to_mesh[domain::H]),
		try_to_bool(item(1, V)->checkState(), state.to_mesh[domain::V])
	};

	if(std::ranges::all_of(does_succeed, is_true)) {
		emit edit_from(app::Step::DETECT_CONFLICT_EIP, [&]() {
			angle->set_next_state(state);
		});
	}
}

} // namespace ui::qt
