///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsView>
#include <QObject>
#include <QRectF>

#include "domain/geometrics/space.hpp"
#include "structure_scene.hpp"
#include "structure_style.hpp"

class QSlider;
class QGraphicsPathItem;

namespace domain {
class Board;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class StructureView : public QGraphicsView {
	Q_OBJECT
public:
	explicit StructureView(QWidget* parent = nullptr);
	~StructureView();

	void set(domain::Board const* board);
	void setup(QSlider* s_structure_zoom, QSlider* s_structure_rotation);
	void reset_view();

	void transform_view();

	void set_mesh_visibility(StructureScene::MeshVisibility mesh_visibility);

	StructureStyleSelector style_selector;
	domain::PlaneSpace<StructureScene> scenes;

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
	QGraphicsPathItem const* const repair;
	QSlider* s_structure_zoom;
	QSlider* s_structure_rotation;
	domain::Board const* board;
};

} // namespace ui::qt
