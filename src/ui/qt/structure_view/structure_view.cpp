///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QPainter>
#include <QScrollBar>
#include <QSlider>
#include <QTransform>
#include <QWheelEvent>
#include <QGraphicsPathItem>

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
, repair(new QGraphicsPathItem(create_repair()))
, s_structure_zoom(nullptr)
, s_structure_rotation(nullptr)
, board(nullptr)
{
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	scale(1, -1);
	setDragMode(QGraphicsView::RubberBandDrag);
}

//******************************************************************************
StructureView::~StructureView() {
	delete repair;
}

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
	matrix.rotate(-s_structure_rotation->value());

	painter->resetTransform();
	painter->drawPath(matrix.map(repair->path()));

	QRectF box_x(0, 0, 1 * unit, 1 * unit);
	QRectF box_y(0, 0, 1 * unit, 1 * unit);
	box_x.moveCenter(matrix.map(QPointF(1.5 * unit, 0 * unit)));
	box_y.moveCenter(matrix.map(QPointF(0 * unit, 1.5 * unit)));

	// TODO wrap switch in set_scene()
	painter->drawText(box_x, Qt::AlignHCenter | Qt::AlignVCenter, [&]() {
		if(scene() == scenes[domain::Plane::YZ]) return "y";
		if(scene() == scenes[domain::Plane::ZX]) return "z";
		if(scene() == scenes[domain::Plane::XY]) return "x";
		return "?";
	} ());
	painter->drawText(box_y, Qt::AlignHCenter | Qt::AlignVCenter, [&]() {
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
			s_structure_zoom->setValue(s_structure_zoom->value() + 6);
		else
			s_structure_zoom->setValue(s_structure_zoom->value() - 6);
	} else if(event->modifiers() & Qt::AltModifier) {
		// Rotation.
		if (event->angleDelta().x() > 0)
			s_structure_rotation->setValue(s_structure_rotation->value() + 6);
		else
			s_structure_rotation->setValue(s_structure_rotation->value() - 6);
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
void StructureView::setup(QSlider* s_structure_zoom, QSlider* s_structure_rotation) {
	this->s_structure_zoom = s_structure_zoom;
	this->s_structure_rotation = s_structure_rotation;
}

//******************************************************************************
void StructureView::transform_view() {
//	qreal scale = qPow(qreal(2), (s_structure_zoom->value() - s_structure_zoom->maximum()/2) / qreal(50));
	qreal scale = qPow(qreal(2), (s_structure_zoom->value() - s_structure_zoom->maximum()/2) / qreal(25));

	QTransform matrix;
	matrix.rotate(s_structure_rotation->value());
	matrix.scale(scale, -scale); // Invert Y axis.
	setTransform(matrix);
}

//******************************************************************************
void StructureView::set_mesh_visibility(StructureScene::MeshVisibility mesh_visibility) {
	for(auto const plane : domain::AllPlane)
		scenes[plane]->set_mesh_visibility(mesh_visibility);
}

//******************************************************************************
void StructureView::set(domain::Board const* board) {
	this->board = board;
}

} // namespace ui::qt
