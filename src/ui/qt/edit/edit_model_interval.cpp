///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <algorithm>
#include <array>

#include "app/steps.hpp"
#include "domain/mesh/interval.hpp"

#include "edit_model_interval.hpp"

namespace ui::qt {

//******************************************************************************
EditModelInterval::EditModelInterval(domain::Interval* interval, QObject* parent)
: EditModel(parent)
, interval(interval)
{
	auto const& state = interval->get_current_state();
	setRowCount(5);

	make_row(0, "dmax", QString::number(state.dmax),
		"Maximum distance between two adjacent lines.");
	make_row(1, "Before.lmin", QString::number(state.before.lmin),
		"Minimum line number in the minimal interval half. "
		"Note a line will always be placed at the interval center.");
	make_row(2, "Before.Smoothness", QString::number(state.before.smoothness), QString("2"),
		"Smoothness factor <b>]1;2]</b> around the minimal side. "
		"Meshing algorithm will decrease it, better to start high.");
	make_row(3, "After.lmin", QString::number(state.after.lmin),
		"Minimum line number in the maximal interval half. "
		"Note a line will always be placed at the interval center.");
	make_row(4, "After.Smoothness", QString::number(state.after.smoothness), QString("2"),
		"Smoothness factor <b>]1;2]</b> around the maximal side. "
		"Meshing algorithm will decrease it, better to start high.");
}

//******************************************************************************
void EditModelInterval::commit() {
	auto state = interval->get_current_state();

	std::array does_succeed = {
		try_to_double(item(0, V)->text(), state.dmax),
		try_to_ulong(item(1, V)->text(), state.before.lmin),
		try_to_double(item(2, V)->text(), state.before.smoothness),
		try_to_ulong(item(3, V)->text(), state.after.lmin),
		try_to_double(item(4, V)->text(), state.after.smoothness)
	};

	if(std::ranges::all_of(does_succeed, is_true)) {
		emit edit_from(app::Step::MESH, [&] {
			interval->set_next_state(state);
		});
	}
}

} // namespace ui::qt
