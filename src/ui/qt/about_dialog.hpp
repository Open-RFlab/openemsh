///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QDialog>
#include <QObject>

#include <memory>

namespace Ui {
class AboutDialog;
} // namespace Ui

namespace ui::qt {

//******************************************************************************
class AboutDialog : public QDialog {
	Q_OBJECT
private:
	std::unique_ptr<Ui::AboutDialog> ui;

private slots:
	void on_pb_ok_clicked();

public:
	explicit AboutDialog(QWidget* parent = nullptr);
	~AboutDialog();
};

} // namespace ui::qt
