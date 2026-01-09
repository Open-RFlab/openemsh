///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <algorithm>
#include <array>

#include "app/steps.hpp"
#include "domain/conflicts/conflict_diagonal_or_circular_zone.hpp"

#include "edit_model_conflict_diagonal_or_circular_zone.hpp"

namespace ui::qt {

//******************************************************************************
EditModelConflictDiagonalOrCircularZone::EditModelConflictDiagonalOrCircularZone(domain::ConflictDiagonalOrCircularZone* conflict, QObject* parent)
: EditModel(parent)
, conflict(conflict)
{
	auto const& state = conflict->get_current_state();
	setRowCount(3);

	make_row(0, "lmin", QString::number(state.lmin),
		"Minimum line number per Interval half, at extremities of the diagonal zone.");
	make_row(1, "dmax", QString::number(state.dmax),
		"Maximum distance between two adjacent lines.");
	make_row(2, "Minimal angle", QString::number(state.minimal_angle),
		"Angle threshold, above which angles between diagonal edges will generate MeshlinePolicies.");
}

//******************************************************************************
void EditModelConflictDiagonalOrCircularZone::commit() {
	auto state = conflict->get_current_state();

	std::array does_succeed = {
		try_to_ulong(item(0, V)->text(), state.lmin),
		try_to_double(item(1, V)->text(), state.dmax),
		try_to_double(item(2, V)->text(), state.minimal_angle)
	};

	if(std::ranges::all_of(does_succeed, is_true)) {
		emit edit_from((state.minimal_angle != conflict->get_current_state().minimal_angle
		               ? app::Step::SOLVE_DIAG_ZONES_ANGLES
		               : app::Step::SOLVE_DIAG_ZONES_INTERVALS), [&]() {
			conflict->set_next_state(state);
		});
	}
}

} // namespace ui::qt
