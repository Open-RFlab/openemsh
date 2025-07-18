///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QDialog>

#include <memory>

namespace Ui {
class EditDialog;
} // namespace Ui

namespace ui::qt {

class EditDelegate;
class EditModel;

//******************************************************************************
class EditDialog : public QDialog {
	Q_OBJECT
private:
	std::unique_ptr<Ui::EditDialog> ui;
	EditDelegate* delegate;

private slots:
	void on_dbb_ok_accepted();
	void on_dbb_ok_rejected();

public:
	explicit EditDialog(EditModel* model, QString const& title = QString(), QWidget* parent = nullptr);
	~EditDialog() override;
};

} // namespace ui::qt
