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

	void populate(domain::Board const* board);
	void reset_view();

	void fit();
	void rotate_view(qreal angle);
	void reset_rotation();
	qreal get_rotation() const;

	void set_mesh_visibility(StructureScene::MeshVisibility mesh_visibility);

	StructureStyleSelector style_selector;
	domain::PlaneSpace<StructureScene*> scenes;

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

	using QGraphicsView::rotate;
	qreal rotation;
};

} // namespace ui::qt
