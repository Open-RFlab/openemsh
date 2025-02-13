///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QGraphicsLinearLayout>

#include "domain/mesh/meshline_policy.hpp"
#include "domain/conflicts/conflict_too_close_meshline_policies.hpp"
#include "ui/qt/data_keys.hpp"
#include "processing_meshline_policy.hpp"

#include "processing_conflict_too_close_meshline_policies.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingConflictTooCloseMeshlinePolicies::ProcessingConflictTooCloseMeshlinePolicies(domain::ConflictTooCloseMeshlinePolicies const* conflict, QGraphicsItem* parent)
: nodegraph::Node("ConflictTooCloseMeshlinePolicies", parent)
, locate_processing_conflict_tcmlp_params(default_locator<Params>)
, conflict(conflict)
{
	QList<QVariant> to_wire;

	locate_node_params = [&]() -> auto& {
		return locate_processing_conflict_tcmlp_params().node;
	};

	title->locate_text_params = [&]() -> auto& {
		return locate_processing_conflict_tcmlp_params().title;
	};

	QGraphicsLinearLayout* h_box = new QGraphicsLinearLayout(Qt::Horizontal, layout());
	QGraphicsLinearLayout* v_box1 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	QGraphicsLinearLayout* v_box2 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	layout()->addItem(h_box);
	h_box->addItem(v_box1);
	h_box->addItem(v_box2);

	for(domain::MeshlinePolicy* policy : conflict->meshline_policies) {
		nodegraph::Port* port = add_input_port(" ");
		port->setFlag(QGraphicsItem::ItemIsSelectable);
		port->setAcceptedMouseButtons(Qt::NoButton);
		port->locate_port_params = [&]() -> auto& {
			return locate_processing_conflict_tcmlp_params().port;
		};
		v_box1->addItem(port);
		to_wire.emplace_back(DataKeys::set_to_wire(policy, port));
	}

	nodegraph::Port* out = add_output_port();
	out->setFlag(QGraphicsItem::ItemIsSelectable);
	out->setAcceptedMouseButtons(Qt::NoButton);
	out->locate_port_params = [&]() -> auto& {
		return locate_processing_conflict_tcmlp_params().port;
	};

	v_box2->addStretch();
	v_box2->addItem(out);
	v_box2->addStretch();
	v_box2->setAlignment(out, Qt::AlignRight | Qt::AlignVCenter);

	setData(DataKeys::TYPE, "ConflictTooCloseMeshlinePolicies");
	setData(DataKeys::ID, (qulonglong) conflict->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(conflict));
	setData(DataKeys::TO_WIRE, std::move(to_wire));
	retrieve_highlightable_children();
}

//******************************************************************************
ProcessingConflictTooCloseMeshlinePolicies::~ProcessingConflictTooCloseMeshlinePolicies() = default;

//******************************************************************************
int ProcessingConflictTooCloseMeshlinePolicies::type() const {
	return Type;
}

//******************************************************************************
QList<ProcessingMeshlinePolicy*> ProcessingConflictTooCloseMeshlinePolicies::get_mlp_origins() const {
	QList<ProcessingMeshlinePolicy*> ret;

	for(auto* node : get_input_nodes())
		if(auto* policy = qgraphicsitem_cast<ProcessingMeshlinePolicy*>(node)
		; policy)
			ret.append(policy);

	return ret;
}

//******************************************************************************
std::size_t ProcessingConflictTooCloseMeshlinePolicies::count_tcmlp_mlp_deepness() const {
	std::size_t deepness = 0;

	for(auto const* policy : get_mlp_origins()) {
		for(auto* node : policy->get_input_nodes()) {
			if(auto* conflict = qgraphicsitem_cast<ProcessingConflictTooCloseMeshlinePolicies*>(node)
			; conflict) {
				deepness = qMax(deepness, conflict->count_tcmlp_mlp_deepness() + 2);
			}
		}
	}

	return deepness;
}

} // namespace ui::qt
