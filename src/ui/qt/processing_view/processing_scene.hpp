///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsScene>
#include <QObject>
#include <QList>

#include <concepts>
#include <functional>
#include <map>
#include <set>

#include "app/steps.hpp"
#include "domain/geometrics/space.hpp"
#include "ui/qt/utils/nodegraph/wire.hpp"
#include "utils/concepts.hpp"
#include "processing_style.hpp"

class Entity;

namespace ui::qt {

class ProcessingAxis;
class ProcessingPlane;
class ProcessingPolygon;
class ProcessingEdge;
class ProcessingConflictColinearEdges;
class ProcessingConflictEdgeInPolygon;
class ProcessingConflictTooCloseMeshlinePolicies;
class ProcessingMeshlinePolicy;
class ProcessingInterval;
class ProcessingMeshline;

//******************************************************************************
class ProcessingScene : public QGraphicsScene {
	Q_OBJECT
public:
	enum class DisplayMode {
		EVERYTHING,
		STRUCTURE_VIEW,
		SELECTED_CHAIN
	};

	explicit ProcessingScene(ProcessingStyleSelector& style_selector, QObject* parent = nullptr);
	~ProcessingScene() override;
	void init();

	void set_wire_style(nodegraph::Wire::Style style) const;
	void fit_containers() const;
	void fit_scene() const;
	void fit(QMarginsF margins = QMarginsF());
	QRectF visible_items_bounding_rect() const;

	ProcessingPlane* add(domain::Plane plane);
	ProcessingAxis* add(domain::Axis axis);
	ProcessingPolygon* add(domain::Polygon* polygon, ProcessingPlane* to_plane);
	ProcessingEdge* add(domain::Edge* edge, ProcessingPolygon* to_polygon);
	ProcessingConflictColinearEdges* add(domain::ConflictColinearEdges* conflict, ProcessingAxis* to_axis);
	ProcessingConflictEdgeInPolygon* add(domain::ConflictEdgeInPolygon* conflict, ProcessingPlane* to_plane);
	ProcessingConflictTooCloseMeshlinePolicies* add(domain::ConflictTooCloseMeshlinePolicies* conflict, ProcessingAxis* to_axis);
	ProcessingMeshlinePolicy* add(domain::MeshlinePolicy* policy, ProcessingAxis* to_axis);
	ProcessingInterval* add(domain::Interval* interval, ProcessingAxis* to_axis);
	ProcessingMeshline* add(domain::Meshline* meshline, ProcessingAxis* to_axis);

	nodegraph::Wire* wire_together(nodegraph::Port* begin, nodegraph::Port* end);

	void wire_to_destination_first_output_port(nodegraph::Node* node);

	QList<nodegraph::Node*> selected_nodes() const;
	QList<nodegraph::Node*> highlighted_nodes() const;
	void reset_visibility(bool are_visible = true) const;
	DisplayMode get_display_mode() const;
	void set_display_mode(DisplayMode mode);
	void set_display_view_axes(domain::ViewAxisSpace<bool> const& axes);
	void set_display_plane(domain::Plane plane);

	void edit_selected_nodes(QPoint const& pos);

	ProcessingStyleSelector& style_selector;

//private: //TODO
	QList<nodegraph::Node*> nodes;

	QList<nodegraph::Wire*> wires;
	domain::PlaneSpace<ProcessingPlane*> planes;
	domain::AxisSpace<ProcessingAxis*> axes;
	QList<ProcessingPolygon*> polygons;
	QList<ProcessingEdge*> edges;
	QList<ProcessingMeshline*> meshlines;
	QList<ProcessingMeshlinePolicy*> meshline_policies;
	QList<ProcessingInterval*> intervals;
	QList<ProcessingConflictColinearEdges*> conflict_colinear_edges;
	QList<ProcessingConflictEdgeInPolygon*> conflict_edge_in_polygons;
	QList<ProcessingConflictTooCloseMeshlinePolicies*> conflict_too_close_meshline_policies;

//	std::map<std::size_t, QGraphicsItem*> index;
	std::map<Entity const*, nodegraph::Node*> index;

private slots:
	void on_selectionChanged();
signals:
	void selection_changed(QList<QGraphicsItem*> items);
	void requires_fit();
	void edit_global_params();
	void edit_from(app::Step from, std::function<void ()> const& edit);

public slots:
	void select_counterparts(QList<QGraphicsItem*> foreign_items);

private:
	bool is_select_counterparts_locked;
	bool is_display_selected_chain_locked;

	DisplayMode display_mode;
	domain::Plane plane_displayed_on_structure_view;
	domain::ViewAxisSpace<bool> axes_displayed_on_structure_view;

	template<std::derived_from<nodegraph::Node> Node, Enum Space>
	Node* add_node(Space space);
	template<std::derived_from<nodegraph::Node> Node, std::derived_from<::Entity> Entity>
	Node* add_node(Entity* entity);
	template<std::derived_from<nodegraph::Node> Node, std::derived_from<::Entity> Entity>
	Node* add_node(Entity* entity, nodegraph::Container* to_container);

	void display_structure_view();
	void display_selected_chain();
	void edit(QList<nodegraph::Node*> nodes, QPoint const& pos);

protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
};

} // namespace ui::qt
