///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <algorithm>
#include <array>

#include "app/steps.hpp"
#include "domain/global.hpp"

#include "edit_model_global.hpp"

namespace ui::qt {

//******************************************************************************
EditModelGlobal::EditModelGlobal(domain::GlobalParams* global, QObject* parent)
: EditModel(parent)
, global(global)
{
	auto const& params = global->get_current_state();
	setRowCount(6);

	make_row(0, "metal_res",       QString::number(params.metal_res), "");
	make_row(1, "substrate_res",   QString::number(params.substrate_res), "");
	make_row(2, "proximity_limit", QString::number(params.proximity_limit), "");
	make_row(3, "lambda",          QString::number(params.lambda), "");
	make_row(4, "lmin",            QString::number(params.lmin), "");
	make_row(5, "dmax",            QString::number(params.dmax), "");
}

//******************************************************************************
void EditModelGlobal::commit() {
	domain::Params params;

	std::array does_succeed = {
		try_to_double(item(0, 1)->text(), params.metal_res),
		try_to_double(item(1, 1)->text(), params.substrate_res),
		try_to_double(item(2, 1)->text(), params.proximity_limit),
		try_to_double(item(3, 1)->text(), params.lambda),
		try_to_ulong(item(4, 1)->text(), params.lmin),
		try_to_double(item(5, 1)->text(), params.dmax)
	};

	if(std::ranges::all_of(does_succeed, is_true)) {
		emit edit_from(app::Step::DETECT_CONFLICT_EIP, [&]() {
			global->set_next_state(params);
		});
	}
}

} // namespace ui::qt
