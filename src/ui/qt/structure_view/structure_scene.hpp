///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsScene>
#include <QObject>

namespace ui::qt {

class StructureEdge;
class StructureMeshline;
class StructurePolygon;

//******************************************************************************
class StructureGroup : public QGraphicsItem {
public:
	explicit StructureGroup(QGraphicsItem* parent = nullptr);
	QRectF boundingRect() const override;
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;
};

//******************************************************************************
class StructureScene : public QGraphicsScene {
	Q_OBJECT
public:

	enum class MeshVisibility {
		NONE,
		VERTICAL,
		HORIZONTAL,
		FULL
	};

	explicit StructureScene(QObject* parent = nullptr);
	~StructureScene() = default;

	void add(StructureEdge* edge);
	void add(StructurePolygon* polygon);
	void add(StructureMeshline* meshline);

	void clear_edges();
	void clear_polygons();
	void clear_meshlines();
	void clear();

	void set_mesh_visibility(MeshVisibility mesh_visibility);

//private: //TODO
	// TODO structure item layer + z value
	StructureGroup* edges;
	StructureGroup* polygons;
	StructureGroup* vertical_meshlines;
	StructureGroup* horizontal_meshlines;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
};

} // namespace ui::qt
