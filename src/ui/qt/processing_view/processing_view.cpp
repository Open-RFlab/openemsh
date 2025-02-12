///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************


#include <QGraphicsPathItem>
#include <QScrollBar>
#include <QWheelEvent>

#include "domain/board.hpp"

#include "processing_view.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingView::ProcessingView(QWidget* parent)
: QGraphicsView(parent)
, processing_scene(new ProcessingScene(this))
{
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//	setViewportUpdateMode(QGraphicsView::SmartViewportUpdate); // glitch
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setDragMode(QGraphicsView::RubberBandDrag);
	setRubberBandSelectionMode(Qt::IntersectsItemShape); //default

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
			scale(1.1, 1.1);
		else
			scale(1 / 1.1, 1 / 1.1);
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
void ProcessingView::fit() {
	processing_scene->fit();
	fitInView(processing_scene->sceneRect(), Qt::KeepAspectRatio);
}

//******************************************************************************
void ProcessingView::populate(domain::Board const* board) {
	for(domain::Plane const plane : domain::AllPlane) {
		auto* processing_plane = processing_scene->add(plane);

		for(auto const& polygon : board->get_polygons(plane)) {
			auto* processing_polygon = processing_scene->add(polygon.get(), processing_plane);

			for(auto const& edge : polygon->edges)
				processing_scene->add(edge.get(), processing_polygon);

			processing_polygon->fit();
		}

		for(auto const& conflict : board->get_conflicts_edge_in_polygons(plane))
			processing_scene->add(conflict.get(), processing_plane);

		processing_plane->fit();
	}

	for(domain::Axis const axis : domain::AllAxis) {
		auto* processing_axis = processing_scene->add(axis);

		for(auto const& conflict : board->get_conflicts_colinear_edges(axis))
			processing_scene->add(conflict.get(), processing_axis);

		for(auto const& conflict : board->get_conflicts_too_close_meshline_policies(axis))
			processing_scene->add(conflict.get(), processing_axis);

		for(auto const& policy : board->get_meshline_policies(axis))
			processing_scene->add(policy.get(), processing_axis);

		for(auto const& interval : board->get_intervals(axis))
			processing_scene->add(interval.get(), processing_axis);

		for(auto const& meshline : board->get_meshlines(axis))
			processing_scene->add(meshline.get(), processing_axis);

		processing_axis->fit();
	}

	for(auto* edge : processing_scene->edges)
		processing_scene->wire_to_destination_first_output_port(edge);

	for(auto* conflict : processing_scene->conflict_colinear_edges)
		processing_scene->wire_to_destination_first_output_port(conflict);

	for(auto* conflict : processing_scene->conflict_too_close_meshline_policies)
		processing_scene->wire_to_destination_first_output_port(conflict);

	for(auto* conflict : processing_scene->conflict_edge_in_polygons)
		processing_scene->wire_to_destination_first_output_port(conflict);

	for(auto* policy : processing_scene->meshline_policies)
		processing_scene->wire_to_destination_first_output_port(policy);

	for(auto* meshline : processing_scene->meshlines)
		processing_scene->wire_to_destination_first_output_port(meshline);

	for(auto* interval : processing_scene->intervals)
		processing_scene->wire_to_destination_first_output_port(interval);

	processing_scene->fit_scene();
}

} // namespace ui::qt
