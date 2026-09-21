///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QDialogButtonBox>
#include <QEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QPushButton>

#include <ranges>

#include "edit_model_per_axis_per_criteria.hpp"
#include "ui_edit_dialog.h"

#include "edit_dialog_per_axis_per_criteria.hpp"

namespace ui::qt {

//******************************************************************************
EditDialogPerAxisPerCriteria::EditDialogPerAxisPerCriteria(IEditModelPerAxisPerCriteria* model, QString const& title, QString const& tooltip, QWidget* parent)
: EditDialog(model, title, parent)
{
	auto* dbb_action = new QDialogButtonBox(this);
	pb_delete = new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ListRemove), "Delete rules", dbb_action);
	pb_add = new QPushButton(QIcon::fromTheme(QIcon::ThemeIcon::ListAdd), "Add rule", dbb_action);
	dbb_action->addButton(pb_delete, QDialogButtonBox::ActionRole);
	dbb_action->addButton(pb_add, QDialogButtonBox::ActionRole);
	ui->vb_main->insertWidget(1, dbb_action);

	ui->tv_properties->setToolTip(tooltip);

	setModal(true);

	connect(pb_delete, &QPushButton::clicked,
		[this]() {
			std::set<int> rows;
			for(auto& index : ui->tv_properties->selectionModel()->selectedIndexes()) {
				rows.insert(index.row());
			}
			for(auto const& row : rows | std::views::reverse) {
				ui->tv_properties->model()->removeRow(row);
			}
		});
	connect(pb_add, &QPushButton::clicked,
		[this]() {
			get_model()->add_new_row();
		});
}

//******************************************************************************
IEditModelPerAxisPerCriteria* EditDialogPerAxisPerCriteria::get_model() const {
	return static_cast<IEditModelPerAxisPerCriteria*>(ui->tv_properties->model());
}

//******************************************************************************
void EditDialogPerAxisPerCriteria::focusInEvent(QFocusEvent* event) {
	ui->tv_properties->setFocus();
}

//******************************************************************************
void EditDialogPerAxisPerCriteria::showEvent(QShowEvent* event) {
	QDialog::showEvent(event);

	if(parentWidget()) {
		QPoint parent_center = parentWidget()->mapToGlobal(parentWidget()->rect().center());
		QPoint dialog_center = rect().center();
		move(parent_center - dialog_center);
	}
}

//******************************************************************************
void EditDialogPerAxisPerCriteria::keyPressEvent(QKeyEvent* event) {
	switch(event->key()) {
	case Qt::Key_Plus:
		pb_add->click();
		break;
	case Qt::Key_Minus: [[fallthrough]];
	case Qt::Key_Delete:
		pb_delete->click();
		break;
	default:
		EditDialog::keyPressEvent(event);
	}
}

} // namespace ui::qt
