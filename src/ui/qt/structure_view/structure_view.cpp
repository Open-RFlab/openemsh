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
, board(nullptr)
, current_timepoint(nullptr)
, repair(std::make_unique<QGraphicsPathItem const>(create_repair()))
, rotation(0)
, displayed_plane(domain::XY)
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
void StructureView::init(domain::Board const* _board) {
	board = _board;
}

//******************************************************************************
void StructureView::clear() {
	for(auto [t, state] : states)
		for(auto* ptr : state.scenes)
			delete ptr;
	states.clear();
	board = nullptr;
	current_timepoint = nullptr;
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
	matrix.rotate(rotation);

	painter->resetTransform();
	painter->drawPath(matrix.map(repair->path()));

	QRectF box_x(0, 0, 1 * unit, 1 * unit);
	QRectF box_y(0, 0, 1 * unit, 1 * unit);
	box_x.moveCenter(matrix.map(QPointF(1.5 * unit, 0 * unit)));
	box_y.moveCenter(matrix.map(QPointF(0 * unit, 1.5 * unit)));

	// TODO wrap switch in set_scene()
	painter->drawText(box_x, Qt::AlignHCenter | Qt::AlignVCenter, [this]() {
		switch(displayed_plane) {
		case domain::Plane::YZ: return "y";
		case domain::Plane::ZX: return "z";
		case domain::Plane::XY: return "x";
		default: return "?";
		}
	} ());
	painter->drawText(box_y, Qt::AlignHCenter | Qt::AlignVCenter, [this]() {
		switch(displayed_plane) {
		case domain::Plane::YZ: return "z";
		case domain::Plane::ZX: return "x";
		case domain::Plane::XY: return "y";
		default: return "?";
		}
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
StructureScene::MeshVisibility StructureView::get_mesh_visibility() {
	return get_current_state().scenes.front()->get_mesh_visibility();
}

//******************************************************************************
void StructureView::set_mesh_visibility(StructureScene::MeshVisibility mesh_visibility) {
	for(auto const plane : domain::AllPlane)
		get_current_state().scenes[plane]->set_mesh_visibility(mesh_visibility);
}

//******************************************************************************
void StructureView::set_display_plane(domain::Plane plane) {
	displayed_plane = plane;
	setScene(get_current_state().scenes[plane]);
	centerOn(get_current_state().scenes[plane]->polygons->boundingRect().center());
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
StructureState& StructureView::get_current_state() {
	return states.at(current_timepoint);
}

//******************************************************************************
void StructureView::make_current_state() {
	if(!board)
		return;

	domain::PlaneSpace<StructureScene*> scenes = {{
		std::make_unique<StructureScene>(style_selector, this).release(),
		std::make_unique<StructureScene>(style_selector, this).release(),
		std::make_unique<StructureScene>(style_selector, this).release() }};

	populate(scenes);
	if(auto const* current_scene = static_cast<StructureScene*>(scene()); current_scene)
		for(auto* scene : scenes)
			scene->set_mesh_visibility(current_scene->get_mesh_visibility());

	states.try_emplace(Caretaker::singleton().get_current_timepoint(), scenes);

	go_to_current_state();
}

//******************************************************************************
void StructureView::go_to_current_state() {
	auto* t = Caretaker::singleton().get_current_timepoint();
	if(current_timepoint != t) {
		if(states.contains(t)) {
			current_timepoint = t;
			setScene(states.at(t).scenes[displayed_plane]);
		} else {
			make_current_state(); // Should only be called from MainWindow, because some signals/slots must be connected at this level
		}
	}
	// TODO switch current viewed scene
}

//******************************************************************************
void StructureView::populate(domain::PlaneSpace<StructureScene*> scenes) {
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
