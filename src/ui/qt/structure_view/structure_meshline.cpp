///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QStyleOptionGraphicsItem>

//#include <limits>

#include "domain/mesh/meshline.hpp"
#include "utils/unreachable.hpp"
#include "ui/qt/data_keys.hpp"

#include "structure_meshline.hpp"

namespace ui::qt {

//******************************************************************************
static QLineF convert(domain::ViewAxis axis, domain::Meshline const* meshline, QRectF scene_rect) {
	qreal side = qMax(scene_rect.width(), scene_rect.height());
	scene_rect += QMarginsF(side, side, side, side) * 2;

	switch(axis) {
	case domain::ViewAxis::H:
		return QLineF(
			scene_rect.left(), meshline->coord.value(),
			scene_rect.right(), meshline->coord.value());
	case domain::ViewAxis::V:
		return QLineF(
			meshline->coord.value(), scene_rect.bottom(),
			meshline->coord.value(), scene_rect.top());
	default:
		unreachable();
	}
}

//******************************************************************************
StructureMeshline::StructureMeshline(domain::ViewAxis axis, domain::Meshline const* meshline, QRectF scene_rect, QGraphicsItem* parent)
: QGraphicsLineItem(convert(axis, meshline, scene_rect), parent)
, axis(axis)
, meshline(meshline)
{
	setFlags(ItemIsSelectable);

	setData(DataKeys::TYPE, "Meshline");
	setData(DataKeys::ID, (qulonglong) meshline->id);
	setData(DataKeys::ENTITY, DataKeys::set_entity(meshline));
//	setData(DataKeys::NAME, meshline->name.c_str());
}

//******************************************************************************
int StructureMeshline::type() const {
	return Type;
}

//******************************************************************************
void StructureMeshline::paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* /*widget*/) {
	painter->setPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

	if(option->state & QStyle::State_Selected) {
		QPen pen = painter->pen();
		pen.setColor(Qt::red);
		painter->setPen(pen);
	}

	painter->drawLine(line());
//	painter->drawLine(QLineF(meshline->coord.value(), std::numeric_limits<qreal>::lowest()/10, meshline->coord.value(), std::numeric_limits<qreal>::max()/10));
//	painter->drawPath(clipPath());
}



} // namespace ui::qt
