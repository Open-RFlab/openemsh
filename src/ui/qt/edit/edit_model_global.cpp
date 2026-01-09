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

	make_row(0, "Proximity limit", QString::number(params.proximity_limit),
		"Distance below which two MeshlinePolicies will be merged.");
	make_row(1, "Smoothness", QString::number(params.smoothness),
		"Smoothness factor <b>]1;2]</b>. Meshing algorithm will decrease it, better to start high.");
	make_row(2, "lmin", QString::number(params.lmin),
		"Minimum line number per Interval half.");
	make_row(3, "dmax", QString::number(params.dmax),
		"Maximum distance between two adjacent lines.");
	make_row(4, "Minimal angle", QString::number(params.consecutive_diagonal_minimal_angle),
		"Angle threshold, above which angles between diagonal edges will generate MeshlinePolicies.");
	make_row(5, "Diagonal dmax", QString::number(params.diagonal_dmax),
		"Maximum distance between two adjacent lines in diagonal zones.");
	make_row(6, "Diagonal lmin", QString::number(params.diagonal_lmin),
		"Minimum line number per Interval half, at extremities of diagonal zones.");
}

//******************************************************************************
void EditModelGlobal::commit() {
	domain::Params params;

	std::array does_succeed = {
		try_to_double(item(0, V)->text(), params.proximity_limit),
		try_to_double(item(1, V)->text(), params.smoothness),
		try_to_ulong(item(2, V)->text(), params.lmin),
		try_to_double(item(3, V)->text(), params.dmax),
		try_to_double(item(4, V)->text(), params.consecutive_diagonal_minimal_angle),
		try_to_double(item(5, V)->text(), params.diagonal_dmax),
		try_to_ulong(item(6, V)->text(), params.diagonal_lmin)
	};

	if(std::ranges::all_of(does_succeed, is_true)) {
		emit edit_from(app::Step::DETECT_DIAG_ZONES, [&]() {
			global->set_next_state(params);
		});
	}
}

} // namespace ui::qt
