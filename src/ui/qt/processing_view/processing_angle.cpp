///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include "domain/geometrics/angle.hpp"
#include "domain/geometrics/edge.hpp"
#include "ui/qt/data_keys.hpp"
#include "ui/qt/utils/nodegraph/text.hpp"

#include "processing_angle.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingAngle::ProcessingAngle(domain::Angle const* angle, QGraphicsItem* parent)
: nodegraph::Node("Angle - " + (angle ? QString::number(angle->id) : QString()), parent)
, locate_processing_angle_params(default_locator<Params>)
, angle(angle)
{
	QList<QVariant> to_wire;

	locate_node_params = [this]() -> auto& {
		return locate_processing_angle_params().node;
	};

	title->locate_text_params = [this]() -> auto& {
		return locate_processing_angle_params().title;
	};

	auto* h_box = new QGraphicsLinearLayout(Qt::Horizontal, layout());
	auto* v_box1 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	auto* v_box2 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	auto* v_box3 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	layout()->addItem(h_box);
	h_box->addItem(v_box1);
	h_box->addItem(v_box2);
	h_box->addItem(v_box3);
	v_box1->addStretch();

	nodegraph::Port* edge0_port = add_input_port();
	edge0_port->setFlag(QGraphicsItem::ItemIsSelectable);
	edge0_port->setAcceptedMouseButtons(Qt::NoButton);
	edge0_port->locate_port_params = [this]() -> auto& {
		return locate_processing_angle_params().port;
	};
	v_box1->addItem(edge0_port);
	v_box1->addStretch();
	to_wire.emplace_back(DataKeys::set_to_wire(angle->e0, edge0_port));

	nodegraph::Port* edge1_port = add_input_port();
	edge1_port->setFlag(QGraphicsItem::ItemIsSelectable);
	edge1_port->setAcceptedMouseButtons(Qt::NoButton);
	edge1_port->locate_port_params = [this]() -> auto& {
		return locate_processing_angle_params().port;
	};
	v_box1->addItem(edge1_port);
	v_box1->addStretch();
	to_wire.emplace_back(DataKeys::set_to_wire(angle->e1, edge1_port));

	nodegraph::Port* output_port = add_output_port();
	output_port->setFlag(QGraphicsItem::ItemIsSelectable);
	output_port->setAcceptedMouseButtons(Qt::NoButton);
	output_port->locate_port_params = [this]() -> auto& {
		return locate_processing_angle_params().port;
	};

	QString angle_val("Angle (°): ");
	QString to_mesh_h("To mesh [H]: ");
	QString to_mesh_v("To mesh [V]: ");
	if(angle) {
		angle_val += QString::number(angle->angle.value());
		to_mesh_h += (angle->get_current_state().to_mesh[domain::H] ? "true" : "false");
		to_mesh_v += (angle->get_current_state().to_mesh[domain::V] ? "true" : "false");
	}

	auto* text_angle = new nodegraph::Text(angle_val, this);
	text_angle->setFlag(QGraphicsItem::ItemIsSelectable);
	text_angle->setAcceptedMouseButtons(Qt::NoButton);
	text_angle->locate_text_params = [this]() -> auto& {
		return locate_processing_angle_params().main;
	};

	auto* text_to_mesh_h = new nodegraph::Text(to_mesh_h, this);
	text_to_mesh_h->setFlag(QGraphicsItem::ItemIsSelectable);
	text_to_mesh_h->setAcceptedMouseButtons(Qt::NoButton);
	if(angle) {
		if(angle->get_current_state().to_mesh[domain::H])
			text_to_mesh_h->locate_text_params = [this]() -> auto& {
				return locate_processing_angle_params().enabled;
			};
		else
			text_to_mesh_h->locate_text_params = [this]() -> auto& {
				return locate_processing_angle_params().disabled;
			};
	} else {
		text_to_mesh_h->locate_text_params = [this]() -> auto& {
			return locate_processing_angle_params().main;
		};
	}

	auto* text_to_mesh_v = new nodegraph::Text(to_mesh_v, this);
	text_to_mesh_v->setFlag(QGraphicsItem::ItemIsSelectable);
	text_to_mesh_v->setAcceptedMouseButtons(Qt::NoButton);
	if(angle) {
		if(angle->get_current_state().to_mesh[domain::V])
			text_to_mesh_v->locate_text_params = [this]() -> auto& {
				return locate_processing_angle_params().enabled;
			};
		else
			text_to_mesh_v->locate_text_params = [this]() -> auto& {
				return locate_processing_angle_params().disabled;
			};
	} else {
		text_to_mesh_v->locate_text_params = [this]() -> auto& {
			return locate_processing_angle_params().main;
		};
	}

	v_box2->addItem(text_angle);
	v_box2->addItem(text_to_mesh_h);
	v_box2->addItem(text_to_mesh_v);
	v_box3->addStretch();
	v_box3->addItem(output_port);
	v_box3->addStretch();
	v_box3->setAlignment(output_port, Qt::AlignRight | Qt::AlignVCenter);

	setData(DataKeys::TYPE, "Angle");
	setData(DataKeys::ID, (qulonglong) angle->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(angle));
	setData(DataKeys::TO_WIRE, to_wire);
	retrieve_highlightable_children();
}

//******************************************************************************
int ProcessingAngle::type() const {
	return Type;
}

} // namespace ui::qt
