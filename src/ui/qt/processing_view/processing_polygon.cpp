///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "domain/geometrics/polygon.hpp"
#include "ui/qt/data_keys.hpp"
#include "ui/qt/utils/nodegraph/rect.hpp"
#include "processing_edge.hpp"

#include "processing_polygon.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingPolygon::ProcessingPolygon(domain::Polygon const* polygon, QGraphicsItem* parent)
: nodegraph::Container("Polygon " + (polygon ? QString::fromStdString(polygon->name) : QString()), QSizeF(50, 10), parent)
, locate_processing_polygon_params(default_locator<Params>)
, polygon(polygon)
{
	setData(DataKeys::TYPE, "Polygon");
	setData(DataKeys::ID, (qulonglong) polygon->id);
	setData(DataKeys::ENTITY, QVariant::fromValue(static_cast<void const*>(polygon)));
	setData(DataKeys::NAME, polygon->name.c_str());

	locate_node_params = [&]() -> auto& {
		return locate_processing_polygon_params().node;
	};

	title->locate_text_params = [&]() -> auto& {
		return locate_processing_polygon_params().title;
	};

	nested_zone->locate_rect_params = [&]() -> auto& {
		return locate_processing_polygon_params().nested_zone;
	};
}

//******************************************************************************
ProcessingPolygon::~ProcessingPolygon() = default;

//******************************************************************************
int ProcessingPolygon::type() const {
	return Type;
}

//******************************************************************************
QList<ProcessingEdge*> ProcessingPolygon::edges() {
	// TODO better
	QList<QGraphicsItem*> const children = nested_zone->childItems();
	QList<ProcessingEdge*> edges(children.size());
	for(qsizetype i = 0; i < children.size(); ++i)
		edges[i] = dynamic_cast<ProcessingEdge*>(children[i]);
	return edges;
}

} // namespace ui::qt
