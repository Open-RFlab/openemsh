///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QBrush>
#include <QGraphicsLayoutItem>
#include <QGraphicsSimpleTextItem>
#include <QList>
#include <QPen>

#include "highlightable.hpp"

namespace ui::qt::nodegraph {

class Node;
class Wire;

//******************************************************************************
class Port
: public QGraphicsSimpleTextItem
, public QGraphicsLayoutItem
, public Highlightable
{
public:
	enum class AnchorPoint { TOP, BOTTOM, LEFT, RIGHT };

	struct Params final {
		qreal radius = 5;
		QPen text_regular = QPen(Qt::white);
		QPen text_highlighted = text_regular;
		QPen text_selected = text_regular;
		QPen text_regular_hovered = text_regular;
		QPen text_highlighted_hovered = text_highlighted;
		QPen text_selected_hovered = text_selected;
		QPen contour_regular = QPen(Qt::black);
		QPen contour_highlighted = contour_regular;
		QPen contour_selected = contour_regular;
		QPen contour_regular_hovered = contour_regular;
		QPen contour_highlighted_hovered = contour_highlighted;
		QPen contour_selected_hovered = contour_selected;
		QBrush fill_regular = QBrush(Qt::yellow);
		QBrush fill_highlighted = fill_regular;
		QBrush fill_selected = fill_regular;
		QBrush fill_regular_hovered = fill_regular;
		QBrush fill_highlighted_hovered = fill_highlighted;
		QBrush fill_selected_hovered = fill_selected;
	};

	std::function<Params const& ()> locate_port_params;

	Port(QString const& text, AnchorPoint anchor, QGraphicsItem* parent = nullptr);
	~Port() override;

	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;
	void setGeometry(QRectF const& geom) override;

	QPointF attach_pos() const;
	bool is_wired() const;
	bool is_wired_to(Port const* port) const;
	QList<Wire*> const& get_wires() const;
	Node* get_node() const;
	QList<Port*> traverse() const;
	QList<Node*> traverse_to_nodes() const;

protected:
	QVariant itemChange(GraphicsItemChange change, QVariant const& value) override;
	QSizeF sizeHint(Qt::SizeHint which, QSizeF const& constraint = QSizeF()) const override;

private:
	friend class Wire;

	AnchorPoint const anchor;
	QList<Wire*> wires;
};

} // namespace ui:qt::nodegraph
