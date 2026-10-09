///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "edit_dialog.hpp"

class QPushButton;

namespace ui::qt {

class IEditModelPerAxisPerCriteria;

//******************************************************************************
class EditDialogPerAxisPerCriteria : public EditDialog {
public:
	explicit EditDialogPerAxisPerCriteria(IEditModelPerAxisPerCriteria* model, QString const& title = QString(), QString const& tooltip = QString(), QWidget* parent = nullptr);

	IEditModelPerAxisPerCriteria* get_model() const;

protected:
	void focusInEvent(QFocusEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void showEvent(QShowEvent* event) override;

private:
	QPushButton* pb_add;
	QPushButton* pb_delete;
};

} // namespace ui::qt
