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
		to_wire.emplace_back(std::in_place_type<DataKeys::ToWire>, policy, port);
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

} // namespace ui::qt
