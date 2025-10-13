///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGraphicsLinearLayout>

#include "domain/geometrics/polygon.hpp"
#include "ui/qt/data_keys.hpp"
#include "ui/qt/utils/nodegraph/rect.hpp"
#include "infra/utils/to_string.hpp"
#include "processing_edge.hpp"

#include "processing_polygon.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingPolygon::ProcessingPolygon(domain::Polygon const* polygon, QGraphicsItem* parent)
: nodegraph::Container("Polygon - " + (polygon ? QString::number(polygon->id) + " - " + QString::fromStdString(polygon->name) : QString()), QSizeF(50, 10), parent)
, locate_processing_polygon_params(default_locator<Params>)
, polygon(polygon)
{
	locate_node_params = [this]() -> auto& {
		return locate_processing_polygon_params().node;
	};

	title->locate_text_params = [this]() -> auto& {
		return locate_processing_polygon_params().title;
	};

	nested_zone->locate_rect_params = [this]() -> auto& {
		return locate_processing_polygon_params().nested_zone;
	};

	nodegraph::Port* output_port = add_output_port("", nodegraph::Port::AnchorPoint::BOTTOM);
	output_port->setFlag(QGraphicsItem::ItemIsSelectable);
	output_port->setAcceptedMouseButtons(Qt::NoButton);
	output_port->locate_port_params = [this]() -> auto& {
		return locate_processing_polygon_params().port;
	};

	QString type("Type: ");
	QString priority("Priority: ");
	if(polygon) {
		if(polygon->material) {
			type += QString::fromStdString(to_string(polygon->material->type));
		}
		priority += QString::number(polygon->priority);
	}

	auto* text_type = new nodegraph::Text(type, this);
	text_type->setFlag(QGraphicsItem::ItemIsSelectable);
	text_type->setAcceptedMouseButtons(Qt::NoButton);
	text_type->locate_text_params = [this]() -> auto& {
		return locate_processing_polygon_params().main;
	};

	auto* text_priority = new nodegraph::Text(priority, this);
	text_priority->setFlag(QGraphicsItem::ItemIsSelectable);
	text_priority->setAcceptedMouseButtons(Qt::NoButton);
	text_priority->locate_text_params = [this]() -> auto& {
		return locate_processing_polygon_params().main;
	};

	auto* h_box = new QGraphicsLinearLayout(Qt::Horizontal, layout());
	auto* v_box = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	layout()->insertItem(1, v_box);
	v_box->addItem(text_type);
	v_box->addItem(text_priority);
	layout()->addItem(h_box);
	h_box->addStretch();
	h_box->addItem(output_port);
	h_box->addStretch();
	h_box->setAlignment(output_port, Qt::AlignHCenter | Qt::AlignVCenter);

	setData(DataKeys::TYPE, "Polygon");
	setData(DataKeys::ID, (qulonglong) polygon->id);
	setData(DataKeys::NAME, QString::fromStdString(polygon->name));
	setData(DataKeys::ENTITY, DataKeys::set_entity(polygon));
	retrieve_highlightable_children();
}

//******************************************************************************
int ProcessingPolygon::type() const {
	return Type;
}

//******************************************************************************
QList<ProcessingEdge*> ProcessingPolygon::edges() const {
	// TODO better
	QList<QGraphicsItem*> const children = nested_zone->childItems();
	QList<ProcessingEdge*> edges(children.size());
	for(qsizetype i = 0; i < children.size(); ++i)
		edges[i] = qgraphicsitem_cast<ProcessingEdge*>(children[i]);
	return edges;
}

} // namespace ui::qt
