///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "edit_model_conflict_too_close_meshline_policies.hpp"
#include "edit_model_edge.hpp"
#include "edit_model_interval.hpp"
#include "edit_model_meshline_policy.hpp"
#include "ui/qt/user_types.hpp"
#include "ui/qt/processing_view/processing_conflict_too_close_meshline_policies.hpp"
#include "ui/qt/processing_view/processing_edge.hpp"
#include "ui/qt/processing_view/processing_interval.hpp"
#include "ui/qt/processing_view/processing_meshline_policy.hpp"
#include "utils/unconst.hpp"

#include "edit_model.hpp"

namespace ui::qt {

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
	case UserTypes::PROCESSING_EDGE:
		return new EditModelEdge(unconst(static_cast<ProcessingEdge*>(node)->edge), parent);
	case UserTypes::PROCESSING_INTERVAL:
		return new EditModelInterval(unconst(static_cast<ProcessingInterval*>(node)->interval), parent);
	case UserTypes::PROCESSING_MESHLINE_POLICY:
		return new EditModelMeshlinePolicy(unconst(static_cast<ProcessingMeshlinePolicy*>(node)->meshline_policy), parent);
	case UserTypes::PROCESSING_CONFLICT_TCMLP:
		return new EditModelConflictTooCloseMeshlinePolicies(unconst(static_cast<ProcessingConflictTooCloseMeshlinePolicies*>(node)->conflict), parent);
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
