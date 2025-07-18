///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QStandardItemModel>

#include <functional>

#include "app/steps.hpp"
#include "utils/concepts.hpp"

namespace ui::qt {

namespace nodegraph {
class Node;
} // namespace nodegraph

//******************************************************************************
class EditModel : public QStandardItemModel {
	Q_OBJECT
public:
	static EditModel* make(nodegraph::Node* node, QObject* parent = nullptr);

	explicit EditModel(QObject* parent = nullptr);
	virtual void commit();

protected:
	static auto constexpr V = 2; // Value column index.

	template<typename T>
	void make_row(int row, QString const& property, T const& old_value, T const& new_value, QString const& tool_tip);
	void make_row(int row, QString const& property, auto const& value, QString const& tool_tip);
	static bool is_true(bool const val) { return val; }
	static bool try_to_double(QString const& in, double& out);
	static bool try_to_ulong(QString const& in, std::size_t& out);
	static bool try_to_bool(Qt::CheckState const in, bool& out);

private:
	template<Enum E>
	static void set_content(QStandardItem* item, E e);
	static void set_content(QStandardItem* item, QString const& s);
	static void set_content(QStandardItem* item, bool b);
	static void set_uneditable(QStandardItem* item);

signals:
	void edit_from(app::Step from, std::function<void ()> const& edit);
};

//******************************************************************************
template<Enum E>
void EditModel::set_content(QStandardItem* item, E e) {
	item->setData(QVariant::fromValue(e));
}

//******************************************************************************
inline void EditModel::set_content(QStandardItem* item, QString const& s) {
	item->setText(s);
}

//******************************************************************************
inline void EditModel::set_content(QStandardItem* item, bool b) {
	item->setCheckable(true);
	item->setCheckState(b ? Qt::Checked : Qt::Unchecked);
	set_uneditable(item);
}

//******************************************************************************
inline void EditModel::set_uneditable(QStandardItem* item) {
	item->setFlags(item->flags() & ~Qt::ItemIsEditable);
}

//******************************************************************************
template<typename T>
void EditModel::make_row(int row, QString const& property, T const& old_value, T const& new_value, QString const& tool_tip) {
	setItem(row, 0, new QStandardItem(property));
	item(row, 0)->setToolTip(tool_tip);
	set_uneditable(item(row, 0));

	if constexpr(V == 2) {
		setItem(row, 1, new QStandardItem());
		item(row, 1)->setToolTip(tool_tip);
		set_uneditable(item(row, 1));
		set_content(item(row, 1), old_value);
	}

	setItem(row, V, new QStandardItem());
	item(row, V)->setToolTip(tool_tip);
	if constexpr(V == 2) {
		set_content(item(row, V), new_value);
	} else {
		set_content(item(row, V), old_value);
	}
}

//******************************************************************************
void EditModel::make_row(int row, QString const& property, auto const& value, QString const& tool_tip) {
	make_row(row, property, value, value, tool_tip);
}

} // namespace ui::qt
