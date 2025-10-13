///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>

#include "domain/mesh/interval.hpp"
#include "domain/mesh/meshline.hpp"
#include "domain/mesh/meshline_policy.hpp"
#include "ui/qt/data_keys.hpp"
#include "ui/qt/utils/nodegraph/text.hpp"

#include "processing_meshline.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingMeshline::ProcessingMeshline(domain::Meshline const* meshline, QGraphicsItem* parent)
: nodegraph::Node("Meshline - " + (meshline ? QString::number(meshline->id) : QString()), parent)
, locate_processing_meshline_params(default_locator<Params>)
, meshline(meshline)
{
	QList<QVariant> to_wire;

	locate_node_params = [this]() -> auto& {
		return locate_processing_meshline_params().node;
	};

	title->locate_text_params = [this]() -> auto& {
		return locate_processing_meshline_params().title;
	};

	auto* h_box = new QGraphicsLinearLayout(Qt::Horizontal, layout());
	auto* v_box1 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	auto* v_box2 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	layout()->addItem(h_box);
	h_box->addItem(v_box1);
	h_box->addItem(v_box2);

	nodegraph::Port* interval_port = add_input_port(" ");
	interval_port->setFlag(QGraphicsItem::ItemIsSelectable);
	interval_port->setAcceptedMouseButtons(Qt::NoButton);
	interval_port->locate_port_params = [this]() -> auto& {
		return locate_processing_meshline_params().port;
	};
	v_box1->addItem(interval_port);

	nodegraph::Port* policy_port = add_input_port(" ");
	policy_port->setFlag(QGraphicsItem::ItemIsSelectable);
	policy_port->setAcceptedMouseButtons(Qt::NoButton);
	policy_port->locate_port_params = [this]() -> auto& {
		return locate_processing_meshline_params().port;
	};
	v_box1->addItem(policy_port);

	QString coord("Coord: ");
	if(meshline) {
		coord += QString::number(meshline->coord.value(), 'f', 6);
		if(meshline->interval)
			to_wire.emplace_back(DataKeys::set_to_wire(meshline->interval, interval_port));
		if(meshline->policy)
			to_wire.emplace_back(DataKeys::set_to_wire(meshline->policy, policy_port));
	}

	auto* text_coord = new nodegraph::Text(coord, this);
	text_coord->setFlag(QGraphicsItem::ItemIsSelectable);
	text_coord->setAcceptedMouseButtons(Qt::NoButton);
	text_coord->locate_text_params = [this]() -> auto& {
		return locate_processing_meshline_params().main;
	};

	v_box2->addStretch();
	v_box2->addItem(text_coord);
	v_box2->addStretch();

	setData(DataKeys::TYPE, "Meshline");
	setData(DataKeys::ID, (qulonglong) meshline->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(meshline));
	setData(DataKeys::TO_WIRE, to_wire);
	retrieve_highlightable_children();
}

//******************************************************************************
int ProcessingMeshline::type() const {
	return Type;
}

} // namespace ui::qt
