///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QPersistentModelIndex>
#include <QStyledItemDelegate>

namespace ui::qt {

//******************************************************************************
class EditDelegate : public QStyledItemDelegate {
public:
	explicit EditDelegate(QObject* parent = nullptr);

	QWidget* createEditor(QWidget* parent, QStyleOptionViewItem const& option, QModelIndex const& index) const override;
	void setEditorData(QWidget* editor, QModelIndex const& index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel* model, QModelIndex const& index) const override;
	void paint(QPainter* painter, QStyleOptionViewItem const& option, QModelIndex const& index) const override;

protected:
	bool eventFilter(QObject* editor, QEvent* event) override;

private:
	QPersistentModelIndex normal_index;
	QPersistentModelIndex policy_index;
};

} // namespace ui::qt
