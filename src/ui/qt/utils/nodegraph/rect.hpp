///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QBrush>
#include <QGraphicsLayoutItem>
#include <QGraphicsRectItem>
#include <QPen>

#include <functional>

#include "highlightable.hpp"

namespace ui::qt::nodegraph {

//******************************************************************************
class Rect
: public QGraphicsRectItem
, public QGraphicsLayoutItem
, public Highlightable
{
public:
	struct Params final {
		qreal opacity_regular = 1;
		qreal opacity_highlighted = opacity_regular;
		qreal opacity_selected = opacity_regular;
		qreal opacity_regular_hovered = opacity_regular;
		qreal opacity_highlighted_hovered = opacity_highlighted;
		qreal opacity_selected_hovered = opacity_selected;
		QBrush fill_regular = QBrush(Qt::gray);
		QBrush fill_highlighted = fill_regular;
		QBrush fill_selected = fill_regular;
		QBrush fill_regular_hovered = fill_regular;
		QBrush fill_highlighted_hovered = fill_highlighted;
		QBrush fill_selected_hovered = fill_selected;
		QPen contour_regular = QPen(fill_regular.color());
		QPen contour_highlighted = contour_regular;
		QPen contour_selected = contour_regular;
		QPen contour_regular_hovered = contour_regular;
		QPen contour_highlighted_hovered = contour_highlighted;
		QPen contour_selected_hovered = contour_selected;
	};

	std::function<Params const& ()> locate_rect_params;

	explicit Rect(QGraphicsItem* parent = nullptr);
	Rect(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent = nullptr);

	void setGeometry(QRectF const& geom) override;

protected:
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;
	QSizeF sizeHint(Qt::SizeHint which, QSizeF const& constraint = QSizeF()) const override;
};

} // namespace ui:qt::nodegraph
