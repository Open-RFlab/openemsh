///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************


#include <QGraphicsPathItem>
#include <QScrollBar>
#include <QSlider>
#include <QTransform>
#include <QWheelEvent>

#include "processing_view.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingView::ProcessingView(QWidget* parent)
: QGraphicsView(parent)
, processing_scene(new ProcessingScene(this))
, s_processing_zoom(nullptr)
, board(nullptr)
{
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//	setViewportUpdateMode(QGraphicsView::SmartViewportUpdate); // glitch
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setDragMode(QGraphicsView::RubberBandDrag);
	setRubberBandSelectionMode(Qt::IntersectsItemShape); //default

//	setBackgroundBrush(Qt::darkGreen);
	setBackgroundBrush(QColor(57, 57, 57));

	setScene(processing_scene);
	connect(
		processing_scene, &ProcessingScene::requires_fit,
		this, &ProcessingView::fit);

}

//******************************************************************************
ProcessingView::~ProcessingView() = default;

//******************************************************************************
void ProcessingView::wheelEvent(QWheelEvent* event) {
	if(event->modifiers() & Qt::ControlModifier) {
		// Zoom.
		if (event->angleDelta().y() > 0)
			s_processing_zoom->setValue(s_processing_zoom->value() + 6);
		else
			s_processing_zoom->setValue(s_processing_zoom->value() - 6);
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
void ProcessingView::setup(QSlider* s_processing_zoom) {
	this->s_processing_zoom = s_processing_zoom;
}

//******************************************************************************
void ProcessingView::transform_view() {
//	qreal scale = qPow(qreal(2), (s_processing_zoom->value() - s_processing_zoom->maximum()/2) / qreal(50));
	qreal scale = qPow(qreal(2), (s_processing_zoom->value() - s_processing_zoom->maximum()/2) / qreal(25));

//	qreal scale = to_scale(s_processing_zoom->value());
//	qreal scale = to_slider(s_processing_zoom->value());

	QTransform matrix;
	matrix.scale(scale, scale);
	setTransform(matrix);
}

//******************************************************************************
void ProcessingView::fit() {
	processing_scene->fit();
	fitInView(processing_scene->sceneRect(), Qt::KeepAspectRatio);
	centerOn(processing_scene->sceneRect().center());
}

//******************************************************************************
void ProcessingView::set(domain::Board const* board) {
	this->board = board;
}

} // namespace ui::qt
