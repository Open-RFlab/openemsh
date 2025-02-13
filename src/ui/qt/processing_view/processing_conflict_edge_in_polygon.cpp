///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QGraphicsLinearLayout>

#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/polygon.hpp"
#include "domain/conflicts/conflict_edge_in_polygon.hpp"
#include "ui/qt/data_keys.hpp"

#include "processing_conflict_edge_in_polygon.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingConflictEdgeInPolygon::ProcessingConflictEdgeInPolygon(domain::ConflictEdgeInPolygon const* conflict, QGraphicsItem* parent)
: nodegraph::Node("ConflictEdgeInPolygon", parent)
, locate_processing_conflict_eip_params(default_locator<Params>)
, conflict(conflict)
{
	QList<QVariant> to_wire;

	locate_node_params = [&]() -> auto& {
		return locate_processing_conflict_eip_params().node;
	};

	title->locate_text_params = [&]() -> auto& {
		return locate_processing_conflict_eip_params().title;
	};

	QGraphicsLinearLayout* h_box = new QGraphicsLinearLayout(Qt::Horizontal, layout());
	QGraphicsLinearLayout* v_box1 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	QGraphicsLinearLayout* v_box2 = new QGraphicsLinearLayout(Qt::Vertical, h_box);
	layout()->addItem(h_box);
	h_box->addItem(v_box1);
	h_box->addItem(v_box2);

	nodegraph::Port* in = add_input_port(" ");
	in->setFlag(QGraphicsItem::ItemIsSelectable);
	in->setAcceptedMouseButtons(Qt::NoButton);
	in->locate_port_params = [&]() -> auto& {
		return locate_processing_conflict_eip_params().port;
	};
	to_wire.emplace_back(DataKeys::set_to_wire(conflict->edge, in));

	v_box1->addItem(in);

	for(auto const& [polygon, range, overlapping_edge] : conflict->overlaps) {
		nodegraph::Port* port = add_input_port(" ");
		port->setFlag(QGraphicsItem::ItemIsSelectable);
		port->setAcceptedMouseButtons(Qt::NoButton);
		port->locate_port_params = [&]() -> auto& {
			return locate_processing_conflict_eip_params().port;
		};
		v_box1->addItem(port);
		to_wire.emplace_back(DataKeys::set_to_wire(polygon, port));
//		if(overlapping_edge)
//			to_wire.emplace_back(DataKeys::set_to_wire(overlaping_edge, port));
	}

	nodegraph::Port* out = add_output_port();
	out->setFlag(QGraphicsItem::ItemIsSelectable);
	out->setAcceptedMouseButtons(Qt::NoButton);
	out->locate_port_params = [&]() -> auto& {
		return locate_processing_conflict_eip_params().port;
	};

	v_box2->addStretch();
	v_box2->addItem(out);
	v_box2->addStretch();
	v_box2->setAlignment(out, Qt::AlignRight | Qt::AlignVCenter);

	setData(DataKeys::TYPE, "ConflictEdgeInPolygon");
	setData(DataKeys::ID, (qulonglong) conflict->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(conflict));
	setData(DataKeys::TO_WIRE, std::move(to_wire));
	retrieve_highlightable_children();
}

//******************************************************************************
ProcessingConflictEdgeInPolygon::~ProcessingConflictEdgeInPolygon() = default;

//******************************************************************************
int ProcessingConflictEdgeInPolygon::type() const {
	return Type;
}

} // namespace ui::qt
