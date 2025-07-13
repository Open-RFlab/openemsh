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
	setRowCount(6);
	for(std::size_t i = 0; auto& str : {
		"metal_res",
		"substrate_res",
		"proximity_limit",
		"lambda",
		"lmin",
		"dmax"
	}) {
		setItem(i++, 0, make_property_item(str));
	}

	auto const& params = global->get_current_state();
	setItem(0, 1, new QStandardItem(QString::number(params.metal_res)));
	setItem(1, 1, new QStandardItem(QString::number(params.substrate_res)));
	setItem(2, 1, new QStandardItem(QString::number(params.proximity_limit)));
	setItem(3, 1, new QStandardItem(QString::number(params.lambda)));
	setItem(4, 1, new QStandardItem(QString::number(params.lmin)));
	setItem(5, 1, new QStandardItem(QString::number(params.dmax)));
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
