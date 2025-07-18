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

	make_row(0, "dmax",          QString::number(state.dmax), "");
	make_row(1, "before.lmin",   QString::number(state.before.lmin), "");
	make_row(2, "before.lambda", QString::number(state.before.lambda), QString("2"), "");
	make_row(3, "after.lmin",    QString::number(state.after.lmin), "");
	make_row(4, "after.lambda",  QString::number(state.after.lambda), QString("2"), "");
}

//******************************************************************************
void EditModelInterval::commit() {
	auto state = interval->get_current_state();

	std::array does_succeed = {
		try_to_double(item(0, 1)->text(), state.dmax),
		try_to_ulong(item(1, 1)->text(), state.before.lmin),
		try_to_double(item(2, 1)->text(), state.before.lambda),
		try_to_ulong(item(3, 1)->text(), state.after.lmin),
		try_to_double(item(4, 1)->text(), state.after.lambda)
	};

	if(std::ranges::all_of(does_succeed, is_true)) {
		emit edit_from(app::Step::MESH, [&] {
			interval->set_next_state(state);
		});
	}
}

} // namespace ui::qt
