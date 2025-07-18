///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <algorithm>
#include <array>

#include "app/steps.hpp"
#include "domain/conflicts/conflict_too_close_meshline_policies.hpp"

#include "edit_model_conflict_too_close_meshline_policies.hpp"

namespace ui::qt {

//******************************************************************************
EditModelConflictTooCloseMeshlinePolicies::EditModelConflictTooCloseMeshlinePolicies(domain::ConflictTooCloseMeshlinePolicies* conflict, QObject* parent)
: EditModel(parent)
, conflict(conflict)
{
	auto const& state = conflict->get_current_state();
	setRowCount(1);

	make_row(0, "is_enabled", state.is_enabled, "");
}

//******************************************************************************
void EditModelConflictTooCloseMeshlinePolicies::commit() {
	auto state = conflict->get_current_state();

	std::array does_succeed = {
		try_to_bool(item(0, 1)->checkState(), state.is_enabled)
	};

	if(std::ranges::all_of(does_succeed, is_true)) {
		emit edit_from(app::Step::DETECT_AND_SOLVE_TCMLP, [&]() {
			conflict->set_next_state(state);
		});
	}
}

} // namespace ui::qt
