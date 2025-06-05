///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "edit_model.hpp"

#include "ui_edit_dialog.h"
#include "edit_dialog.hpp"

namespace ui::qt {

using namespace std;

//******************************************************************************
EditDialog::EditDialog(EditModel* model, QString const& title, QWidget* parent)
: QDialog(parent, Qt::Dialog)
, ui(make_unique<Ui::EditDialog>()) {
	ui->setupUi(this);
	setWindowIcon(QPixmap(":/openemsh.ico"));
	setWindowTitle(windowTitle() + " " + title);

	ui->tv_properties->setModel(model);
	ui->tv_properties->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

//******************************************************************************
EditDialog::~EditDialog() = default;

//******************************************************************************
void EditDialog::on_dbb_ok_accepted() {
	setCursor(Qt::WaitCursor);
	static_cast<EditModel*>(ui->tv_properties->model())->commit();
	unsetCursor();
	accept();
}

//******************************************************************************
void EditDialog::on_dbb_ok_rejected() {
	reject();
}

} // namespace ui::qt
