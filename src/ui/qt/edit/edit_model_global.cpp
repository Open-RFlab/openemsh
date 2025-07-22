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

// https://doc.qt.io/qt-6/qitemeditorfactory.html#registerEditor
// https://www.bogotobogo.com/Qt/Qt5_QTableView_QItemDelegate_ModelView_MVC.php
// https://stackoverflow.com/questions/60767527/creating-item-with-a-integer-qstandarditem-doesnt-provide-qspinbox
// https://stackoverflow.com/questions/3135505/qstandarditem-qcombobox
//******************************************************************************
EditModelGlobal::EditModelGlobal(domain::GlobalParams* global, QObject* parent)
: EditModel(parent)
, global(global)
{
	auto const& params = global->get_current_state();
	setRowCount(4);

//	make_row(0, "metal_res", QString::number(params.metal_res), "");
//	make_row(1, "substrate_res", QString::number(params.substrate_res), "");
	make_row(0, "Proximity limit", QString::number(params.proximity_limit),
		"Distance below which two MeshlinePolicies will be merged.");
	make_row(1, "Smoothness", QString::number(params.lambda),
		"Smoothness factor <b>]1;2]</b>. Meshing algorithm will decrease it, better to start high.");
	make_row(2, "lmin", QString::number(params.lmin),
		"Minimum line number per Interval half.");
	make_row(3, "dmax", QString::number(params.dmax),
		"Maximum distance between two adjacent lines.");
}

//******************************************************************************
void EditModelGlobal::commit() {
	domain::Params params;

	std::array does_succeed = {
//		try_to_double(item(0, V)->text(), params.metal_res),
//		try_to_double(item(1, V)->text(), params.substrate_res),
		try_to_double(item(0, V)->text(), params.proximity_limit),
		try_to_double(item(1, V)->text(), params.lambda),
		try_to_ulong(item(2, V)->text(), params.lmin),
		try_to_double(item(3, V)->text(), params.dmax)
	};

	if(std::ranges::all_of(does_succeed, is_true)) {
		emit edit_from(app::Step::DETECT_CONFLICT_EIP, [&]() {
			global->set_next_state(params);
		});
	}
}

} // namespace ui::qt
