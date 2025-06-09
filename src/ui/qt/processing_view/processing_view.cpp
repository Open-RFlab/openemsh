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
, board(nullptr)
, current_timepoint(nullptr)
, display_mode(ProcessingScene::DisplayMode::SELECTED_CHAIN)
, plane_displayed_on_structure_view(domain::XY)
, axes_displayed_on_structure_view({ true, true })
{
	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
//	setViewportUpdateMode(QGraphicsView::SmartViewportUpdate); // glitch
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setDragMode(QGraphicsView::RubberBandDrag);
	setRubberBandSelectionMode(Qt::IntersectsItemShape); //default
}

//******************************************************************************
void ProcessingView::init(domain::Board const* _board) {
	board = _board;
}

//******************************************************************************
void ProcessingView::clear() {
	for(auto [t, state] : states)
		delete state.scene;
	states.clear();
	board = nullptr;
	current_timepoint = nullptr;
}

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
	get_current_state().scene->fit();
	fitInView(get_current_state().scene->sceneRect(), Qt::KeepAspectRatio);
}

//******************************************************************************
void ProcessingView::set_display(ProcessingScene::DisplayMode mode) {
	display_mode = mode;
	get_current_state().scene->set_display(mode);
}

//******************************************************************************
void ProcessingView::set_display_view_axes(domain::ViewAxisSpace<bool> const& axes) {
	axes_displayed_on_structure_view = axes;
	get_current_state().scene->set_display_view_axes(axes);
}

//******************************************************************************
void ProcessingView::set_display_plane(domain::Plane plane) {
	plane_displayed_on_structure_view = plane;
	get_current_state().scene->set_display_plane(plane);
}

//******************************************************************************
ProcessingState& ProcessingView::get_current_state() {
	return states.at(current_timepoint);
}

// TODO This seems to require to invoke some fit() at some point
//******************************************************************************
void ProcessingView::make_current_state() {
	if(!board)
		return;

	auto* scene = new ProcessingScene(style_selector, this);
	connect(
		scene, &ProcessingScene::requires_fit,
		this, &ProcessingView::fit);

	populate(scene);
	scene->init();
	scene->set_display(display_mode);
	scene->set_display_view_axes(axes_displayed_on_structure_view);
	scene->set_display_plane(plane_displayed_on_structure_view);

	states.emplace(Caretaker::singleton().get_current_timepoint(), scene);
	go_to_current_state();
}

//******************************************************************************
void ProcessingView::go_to_current_state() {
	auto* t = Caretaker::singleton().get_current_timepoint();
	if(current_timepoint != t) {
		if(states.contains(t)) {
			current_timepoint = t;
			setScene(states.at(t).scene);
		} else {
			make_current_state();
		}
	}
	// TODO switch current viewed scene
}

//******************************************************************************
void ProcessingView::populate(ProcessingScene* scene) {
	for(domain::Plane const plane : domain::AllPlane) {
		auto* processing_plane = scene->add(plane);

		for(auto const& polygon : board->get_polygons(plane)) {
			auto* processing_polygon = scene->add(polygon.get(), processing_plane);

			for(auto const& edge : polygon->edges)
				scene->add(edge.get(), processing_polygon);

			processing_polygon->fit();
		}

		for(auto const& conflict : board->get_conflicts_edge_in_polygons(plane))
			scene->add(conflict.get(), processing_plane);

		processing_plane->fit();
	}

	for(domain::Axis const axis : domain::AllAxis) {
		auto* processing_axis = scene->add(axis);

		for(auto const& conflict : board->get_conflicts_colinear_edges(axis))
			scene->add(conflict.get(), processing_axis);

		for(auto const& conflict : board->get_conflicts_too_close_meshline_policies(axis))
			scene->add(conflict.get(), processing_axis);

		for(auto const& policy : board->get_meshline_policies(axis))
			scene->add(policy.get(), processing_axis);

		for(auto const& interval : board->get_intervals(axis))
			scene->add(interval.get(), processing_axis);

		for(auto const& meshline : board->get_meshlines(axis))
			scene->add(meshline.get(), processing_axis);

		processing_axis->fit();
	}

	for(auto* edge : scene->edges)
		scene->wire_to_destination_first_output_port(edge);

	for(auto* conflict : scene->conflict_colinear_edges)
		scene->wire_to_destination_first_output_port(conflict);

	for(auto* conflict : scene->conflict_too_close_meshline_policies)
		scene->wire_to_destination_first_output_port(conflict);

	for(auto* conflict : scene->conflict_edge_in_polygons)
		scene->wire_to_destination_first_output_port(conflict);

	for(auto* policy : scene->meshline_policies)
		scene->wire_to_destination_first_output_port(policy);

	for(auto* meshline : scene->meshlines)
		scene->wire_to_destination_first_output_port(meshline);

	for(auto* interval : scene->intervals)
		scene->wire_to_destination_first_output_port(interval);

	scene->fit_scene();
}

} // namespace ui::qt
