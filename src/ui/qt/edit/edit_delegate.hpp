///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QComboBox>
#include <QPersistentModelIndex>
#include <QStyledItemDelegate>

#include "domain/global.hpp"

namespace ui::qt {

//******************************************************************************
using OptionalMaterial = std::optional<domain::Params::Mat>;
enum class OptionalMaterialState { ALL, TYPE, NAME };

//******************************************************************************
class OptionalMaterialEditor : public QComboBox {
	Q_OBJECT
public:
	explicit OptionalMaterialEditor(QWidget* parent = nullptr);

	void set_value(OptionalMaterial const& material);
	OptionalMaterial get_value() const;

signals:
	void editing_finished();

private slots:
	void on_activated(int index);

private:
	static QString const by_name_str;
	int index_of_wildcard() const;
	int index_of_by_name() const;
};

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
