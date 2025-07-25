///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGuiApplication>

#include "edit_delegate.hpp"
#include "edit_model.hpp"

#include "ui_edit_dialog.h"
#include "edit_dialog.hpp"

namespace ui::qt {

using namespace std;

//******************************************************************************
EditDialog::EditDialog(EditModel* model, QString const& title, QWidget* parent)
: QDialog(parent, Qt::Dialog)
, ui(make_unique<Ui::EditDialog>())
, delegate(new EditDelegate(this)) {
	ui->setupUi(this);
	setWindowIcon(QPixmap(":/openemsh.ico"));
	setWindowTitle(windowTitle() + " " + title);

	ui->tv_properties->setItemDelegate(delegate);
	ui->tv_properties->setModel(model);
	ui->tv_properties->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

//******************************************************************************
EditDialog::~EditDialog() = default;

//******************************************************************************
void EditDialog::on_dbb_ok_accepted() {
	QGuiApplication::setOverrideCursor(Qt::WaitCursor);
	static_cast<EditModel*>(ui->tv_properties->model())->commit();
	QGuiApplication::restoreOverrideCursor();
	accept();
}

//******************************************************************************
void EditDialog::on_dbb_ok_rejected() {
	reject();
}

} // namespace ui::qt
