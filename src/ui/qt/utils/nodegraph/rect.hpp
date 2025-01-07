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

namespace ui::qt::nodegraph {

//******************************************************************************
class Rect
: public QGraphicsRectItem
, public QGraphicsLayoutItem
{
public:
	struct Params final {
		qreal opacity_regular = 1;
		qreal opacity_highlighted = opacity_regular;
		qreal opacity_selected = opacity_regular;
		qreal opacity_selected_highlighted = opacity_highlighted;
		QBrush fill_regular = QBrush(Qt::gray);
		QBrush fill_highlighted = fill_regular;
		QBrush fill_selected = fill_regular;
		QBrush fill_selected_highlighted = fill_highlighted;
		QPen contour_regular = QPen(fill_regular.color());
		QPen contour_highlighted = contour_regular;
		QPen contour_selected = contour_regular;
		QPen contour_selected_highlighted = contour_highlighted;
	};

	std::function<Params const& ()> locate_rect_params;

	explicit Rect(QGraphicsItem* parent = nullptr);
	Rect(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent = nullptr);
	~Rect();

	void setGeometry(QRectF const& geom) override;

protected:
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;
	QSizeF sizeHint(Qt::SizeHint which, QSizeF const& constraint = QSizeF()) const override;
};

} // namespace ui:qt::nodegraph
