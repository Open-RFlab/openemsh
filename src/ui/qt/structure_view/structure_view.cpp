///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGraphicsPathItem>
#include <QPainter>
#include <QScrollBar>
#include <QTransform>
#include <QWheelEvent>

#include <cmath>

#include "domain/board.hpp"

#include "structure_view.hpp"

namespace ui::qt {

static qreal constexpr unit = 100 / 3;

//******************************************************************************
static QPainterPath create_repair() {
	QPainterPath repair;
	// Axes.
	repair.moveTo((1 - 0.25/4) * unit, 0 * unit);
	repair.lineTo(0 * unit, 0 * unit);
	repair.lineTo(0 * unit, (1 - 0.25/4) * unit);

	// X arrow.
	repair.moveTo((1 - 0.25) * unit, (0 + 0.25) * unit);
	repair.lineTo(1 * unit, 0 * unit);
	repair.lineTo((1 - 0.25) * unit, (0 - 0.25) * unit);

	// Y arrow.
	repair.moveTo((0 + 0.25) * unit, (1 - 0.25) * unit);
	repair.lineTo(0 * unit, 1 * unit);
	repair.lineTo((0 - 0.25) * unit, (1 - 0.25) * unit);
	return repair;
}

//******************************************************************************
StructureView::StructureView(QWidget* parent)
: QGraphicsView(parent)
, scenes{{
	new StructureScene(style_selector, this),
	new StructureScene(style_selector, this),
	new StructureScene(style_selector, this) }}
, repair(std::make_unique<QGraphicsPathItem const>(create_repair()))
, rotation(0)
{
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	scale(1, -1);
	setDragMode(QGraphicsView::RubberBandDrag);
}

//******************************************************************************
StructureView::~StructureView() = default;

//******************************************************************************
void StructureView::drawForeground(QPainter* painter, QRectF const& rect) {
	QGraphicsView::drawForeground(painter, rect);

	painter->setPen(QPen(Qt::black, unit / 20, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
	QFont font = painter->font();
	font.setWeight(QFont::DemiBold);
	font.setPointSize(14);
	painter->setFont(font);

	QTransform matrix;
	matrix.translate(0, viewport()->rect().height());
	matrix.translate(2 * unit, -2 * unit);
	matrix.scale(1, -1);
	matrix.rotate(rotation);

	painter->resetTransform();
	painter->drawPath(matrix.map(repair->path()));

	QRectF box_x(0, 0, 1 * unit, 1 * unit);
	QRectF box_y(0, 0, 1 * unit, 1 * unit);
	box_x.moveCenter(matrix.map(QPointF(1.5 * unit, 0 * unit)));
	box_y.moveCenter(matrix.map(QPointF(0 * unit, 1.5 * unit)));

	// TODO wrap switch in set_scene()
	painter->drawText(box_x, Qt::AlignHCenter | Qt::AlignVCenter, [this]() {
		if(scene() == scenes[domain::Plane::YZ]) return "y";
		if(scene() == scenes[domain::Plane::ZX]) return "z";
		if(scene() == scenes[domain::Plane::XY]) return "x";
		return "?";
	} ());
	painter->drawText(box_y, Qt::AlignHCenter | Qt::AlignVCenter, [this]() {
		if(scene() == scenes[domain::Plane::YZ]) return "z";
		if(scene() == scenes[domain::Plane::ZX]) return "x";
		if(scene() == scenes[domain::Plane::XY]) return "y";
		return "?";
	} ());
}

//******************************************************************************
void StructureView::wheelEvent(QWheelEvent* event) {
	if(event->modifiers() & Qt::ControlModifier) {
		// Zoom.
		if (event->angleDelta().y() > 0)
			scale(1.1, 1.1);
		else
			scale(1 / 1.1, 1 / 1.1);
	} else if(event->modifiers() & Qt::AltModifier) {
		// Rotation.
		if (event->angleDelta().x() > 0)
			rotate_view(5);
		else
			rotate_view(-5);
	} else if(event->modifiers() & Qt::ShiftModifier) {
		// Horizontal scroll.
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() - event->angleDelta().y() / 8);
	} else {
		// Vertical scroll.
		verticalScrollBar()->setValue(verticalScrollBar()->value() - event->angleDelta().y() / 8);
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() - event->angleDelta().x() / 8); // Might have effect on pads that support horizontal scroll. ?
	}
	event->accept();
}

//******************************************************************************
void StructureView::fit() {
	fitInView(static_cast<StructureScene*>(scene())->polygons->boundingRect() + QMarginsF(5, 5, 5, 5), Qt::KeepAspectRatio);
}

//******************************************************************************
void StructureView::set_mesh_visibility(StructureScene::MeshVisibility mesh_visibility) {
	for(auto const plane : domain::AllPlane)
		scenes[plane]->set_mesh_visibility(mesh_visibility);
}

//******************************************************************************
void StructureView::reset_rotation() {
	rotate_view(-rotation);
}

//******************************************************************************
void StructureView::rotate_view(qreal angle) {
	rotation = std::fmod(rotation + angle, 360);
	rotate(angle);
}

//******************************************************************************
qreal StructureView::get_rotation() const {
	return rotation;
}

//******************************************************************************
void StructureView::populate(domain::Board const* board) {
	for(domain::Plane const plane : domain::AllPlane) {
		scenes[plane]->clear_all();

		for(auto const& polygon : board->get_polygons(plane)) {
			scenes[plane]->add(polygon.get());

			for(auto const& edge : polygon->edges)
				scenes[plane]->add(edge.get());
		}

		QRectF const scene_rect(scenes[plane]->sceneRect());

		for(domain::Axis const axis : domain::Axes[plane]) {
			if(auto const view_axis = domain::transpose(plane, axis); view_axis) {
				for(auto const& meshline : board->get_meshlines(axis))
					scenes[plane]->add(meshline.get(), view_axis.value(), scene_rect);

				for(auto const& policy : board->get_meshline_policies(axis))
					scenes[plane]->add(policy.get(), view_axis.value(), scene_rect);

				for(auto const& interval : board->get_intervals(axis))
					scenes[plane]->add(interval.get(), view_axis.value(), scene_rect);

				for(auto const& conflict : board->get_conflicts_colinear_edges(axis))
					scenes[plane]->add(conflict.get(), view_axis.value(), scene_rect);

				for(auto const& conflict : board->get_conflicts_too_close_meshline_policies(axis))
					scenes[plane]->add(conflict.get(), view_axis.value(), scene_rect);
			}
		}
	}
}

} // namespace ui::qt
