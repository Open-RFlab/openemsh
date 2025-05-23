///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsView>
#include <QObject>
#include <QRectF>

#include <memory>

#include "domain/geometrics/space.hpp"
#include "utils/state_management.hpp"
#include "structure_scene.hpp"
#include "structure_style.hpp"

class QGraphicsPathItem;

namespace domain {
class Board;
} // namespace domain

namespace ui::qt {

//******************************************************************************
struct StructureState {
	domain::PlaneSpace<StructureScene*> scenes;
};

//******************************************************************************
class StructureView : public QGraphicsView {
	Q_OBJECT
public:
	explicit StructureView(QWidget* parent = nullptr);
	~StructureView() override;
	void init(domain::Board const* board);

	void fit();
	void rotate_view(qreal angle);
	void reset_rotation();
	qreal get_rotation() const;

	void set_mesh_visibility(StructureScene::MeshVisibility mesh_visibility);
	void set_display_plane(domain::Plane plane);

	StructureStyleSelector style_selector;
	std::map<Timepoint*, StructureState> states;

	StructureState& get_current_state();
	void make_current_state();
	void go_to_current_state();

protected:
	void drawForeground(QPainter* painter, QRectF const& rect) override;
	void wheelEvent(QWheelEvent* event) override;

private:
	// TODO
	//using QGraphicsView::setScene;
	// QString horizontal_axis;
	// QString vertical_axis;
	// + public set_scene() that wrap axis QStrings or repair

//	qreal scale_max;
	std::unique_ptr<QGraphicsPathItem const> const repair;

	using QGraphicsView::rotate;
	qreal rotation;
	domain::Plane displayed_plane;
	domain::Board const* board;
	Timepoint* current_timepoint;

	void populate(domain::PlaneSpace<StructureScene*> scenes);
};

} // namespace ui::qt
