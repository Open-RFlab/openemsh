///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QDialog>

#include <memory>

class QAbstractButton;

namespace Ui {
class AboutDialog;
} // namespace Ui

namespace ui::qt {

//******************************************************************************
class AboutDialog : public QDialog {
private:
	std::unique_ptr<Ui::AboutDialog> ui;

public:
	explicit AboutDialog(QWidget* parent = nullptr);
	~AboutDialog() override;
};

} // namespace ui::qt
