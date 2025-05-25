///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QDialog>
#include <QObject>

#include <memory>

class QAbstractButton;

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
	void on_dbb_ok_clicked(QAbstractButton* button);

public:
	explicit AboutDialog(QWidget* parent = nullptr);
	~AboutDialog() override;
};

} // namespace ui::qt
