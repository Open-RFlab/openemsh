///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "edit_model.hpp"

namespace ui::qt {

using namespace std;

//******************************************************************************
QStandardItem* EditModel::make_property_item(QString const& str) {
	auto* item = new QStandardItem(str);
	item->setFlags(item->flags() & ~Qt::ItemIsEditable);
	return item;
}

//******************************************************************************
QStandardItem* EditModel::make_bool_item(bool in) {
	auto* item = new QStandardItem();
	item->setCheckable(true);
	if(in)
		item->setCheckState(Qt::Checked);
	else
		item->setCheckState(Qt::Unchecked);
	return item;
}

//******************************************************************************
bool EditModel::try_to_double(QString const& in, double& out) {
	bool does_succeed = false;
	auto result = in.toDouble(&does_succeed);
	if(does_succeed)
		out = result;
	return does_succeed;
}

//******************************************************************************
bool EditModel::try_to_ulong(QString const& in, std::size_t& out) {
	bool does_succeed = false;
	auto result = in.toULong(&does_succeed);
	if(does_succeed)
		out = result;
	return does_succeed;
}

//******************************************************************************
bool EditModel::try_to_bool(Qt::CheckState const in, bool& out) {
	bool does_succeed = false;
	switch(in) {
	case Qt::Checked:
		does_succeed = true;
		out = true;
		break;
	case Qt::Unchecked:
		does_succeed = true;
		out = false;
		break;
	default:
		break;
	}
	return does_succeed;
}

//******************************************************************************
EditModel* EditModel::make(nodegraph::Node* node, QObject* parent) {
	switch(node->type()) {
	default:
		return nullptr;
	}
}

//******************************************************************************
EditModel::EditModel(QObject* parent)
: QStandardItemModel(parent)
{
	setColumnCount(2);
	// TODO maybe 3 columns { "Property", "Old value", "New value" } would be better for UI
	// especially for setting lambda to 2 by default
	setHorizontalHeaderLabels({ "Property", "Value" });
}

//******************************************************************************
void EditModel::commit() {}

} // namespace ui::qt
