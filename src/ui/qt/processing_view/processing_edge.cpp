///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include "domain/geometrics/edge.hpp"
#include "domain/conflicts/conflict.hpp"
#include "ui/qt/data_keys.hpp"
#include "ui/qt/utils/nodegraph/text.hpp"
#include "infra/utils/to_string.hpp"

#include "processing_edge.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingEdge::ProcessingEdge(domain::Edge const* edge, QGraphicsItem* parent)
: nodegraph::Node("Edge", parent)
, locate_processing_edge_params(default_locator<Params>)
, edge(edge)
{
	QList<QVariant> to_wire;

	locate_node_params = [this]() -> auto& {
		return locate_processing_edge_params().node;
	};

	title->locate_text_params = [this]() -> auto& {
		return locate_processing_edge_params().title;
	};

	nodegraph::Port* input_port = add_input_port();
	input_port->setFlag(QGraphicsItem::ItemIsSelectable);
	input_port->setAcceptedMouseButtons(Qt::NoButton);
	input_port->locate_port_params = [this]() -> auto& {
		return locate_processing_edge_params().port;
	};
	if(edge->get_current_state().conflict) {
		to_wire.emplace_back(DataKeys::set_to_wire(edge->get_current_state().conflict, input_port));
	}

	nodegraph::Port* output_port = add_output_port();
	output_port->setFlag(QGraphicsItem::ItemIsSelectable);
	output_port->setAcceptedMouseButtons(Qt::NoButton);
	output_port->locate_port_params = [this]() -> auto& {
		return locate_processing_edge_params().port;
	};

	QString normal("Normal: ");
	QString to_mesh("To mesh: ");
	QString to_reverse("To reverse: ");
	if(edge) {
		normal += QString::fromStdString(to_string(edge->normal));
		to_mesh += (edge->get_current_state().to_mesh ? "true" : "false");
		to_reverse += (edge->get_current_state().to_reverse ? "true" : "false");
	}

	auto* text_normal = new nodegraph::Text(normal, this);
	text_normal->setFlag(QGraphicsItem::ItemIsSelectable);
	text_normal->setAcceptedMouseButtons(Qt::NoButton);
	text_normal->locate_text_params = [this]() -> auto& {
		return locate_processing_edge_params().main;
	};

	auto* text_to_reverse = new nodegraph::Text(to_reverse, this);
	text_to_reverse->setFlag(QGraphicsItem::ItemIsSelectable);
	text_to_reverse->setAcceptedMouseButtons(Qt::NoButton);
	text_to_reverse->locate_text_params = [this]() -> auto& {
		return locate_processing_edge_params().main;
	};

	auto* text_to_mesh = new nodegraph::Text(to_mesh, this);
	text_to_mesh->setFlag(QGraphicsItem::ItemIsSelectable);
	text_to_mesh->setAcceptedMouseButtons(Qt::NoButton);
	if(edge) {
		if(edge->get_current_state().to_mesh)
			text_to_mesh->locate_text_params = [this]() -> auto& {
				return locate_processing_edge_params().enabled;
			};
		else
			text_to_mesh->locate_text_params = [this]() -> auto& {
				return locate_processing_edge_params().disabled;
			};
	} else {
		text_to_mesh->locate_text_params = [this]() -> auto& {
			return locate_processing_edge_params().main;
		};
	}

	auto* h_box = new QGraphicsLinearLayout(Qt::Horizontal, layout());
	auto* v_box1 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	auto* v_box2 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	auto* v_box3 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	layout()->addItem(h_box);
	h_box->addItem(v_box1);
	h_box->addItem(v_box2);
	h_box->addItem(v_box3);
	v_box1->addStretch();
	v_box1->addItem(input_port);
	v_box1->addStretch();
	v_box2->addItem(text_normal);
	v_box2->addItem(text_to_mesh);
	v_box2->addItem(text_to_reverse);
	v_box3->addStretch();
	v_box3->addItem(output_port);
	v_box3->addStretch();
	v_box3->setAlignment(output_port, Qt::AlignRight | Qt::AlignVCenter);

	setData(DataKeys::TYPE, "Edge");
	setData(DataKeys::ID, (qulonglong) edge->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(edge));
	setData(DataKeys::TO_WIRE, to_wire);
	retrieve_highlightable_children();
}

//******************************************************************************
int ProcessingEdge::type() const {
	return Type;
}

} // namespace ui::qt
