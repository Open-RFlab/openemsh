///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QBrush>
#include <QGraphicsLayoutItem>
#include <QGraphicsSimpleTextItem>
#include <QPen>

namespace ui::qt::nodegraph {

class Wire;

//******************************************************************************
class Port
: public QGraphicsSimpleTextItem
, public QGraphicsLayoutItem
{
public:
	enum class AnchorPoint { TOP, BOTTOM, LEFT, RIGHT };

	struct Params final {
		qreal radius = 5;
		QPen text_regular = QPen(Qt::white);
		QPen text_highlighted = text_regular;
		QPen text_selected = text_regular;
		QPen text_selected_highlighted = text_selected;
		QPen contour_regular = QPen(Qt::black);
		QPen contour_highlighted = contour_regular;
		QPen contour_selected = contour_regular;
		QPen contour_selected_highlighted = contour_selected;
		QBrush fill_regular = QBrush(Qt::yellow);
		QBrush fill_highlighted = fill_regular;
		QBrush fill_selected = fill_regular;
		QBrush fill_selected_highlighted = fill_selected;
	};

	std::function<Params const& ()> locate_port_params;

	Port(QString const& text, AnchorPoint anchor, QGraphicsItem* parent = nullptr);
	~Port();

	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;
	void setGeometry(QRectF const& geom) override;

	QPointF attach_pos() const;
	bool is_wired() const;
	bool is_wired_to(Port const* port) const;

protected:
	QVariant itemChange(GraphicsItemChange change, QVariant const& value) override;
	QSizeF sizeHint(Qt::SizeHint which, QSizeF const& constraint = QSizeF()) const override;

private:
	friend class Wire;

	AnchorPoint const anchor;
	Wire* wire; // TODO QList<Wire*>
};

} // namespace ui:qt::nodegraph
