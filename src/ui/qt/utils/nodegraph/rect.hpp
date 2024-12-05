///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsLayoutItem>
#include <QGraphicsRectItem>

namespace ui::qt::nodegraph {

//******************************************************************************
class Rect
: public QGraphicsRectItem
, public QGraphicsLayoutItem
{
public:
	explicit Rect(QGraphicsItem* parent = nullptr);
	Rect(qreal x, qreal y, qreal width, qreal height, QGraphicsItem* parent = nullptr);
	~Rect();

	void setGeometry(QRectF const& geom) override;

protected:
	QSizeF sizeHint(Qt::SizeHint which, QSizeF const& constraint = QSizeF()) const override;
};

} // namespace ui:qt::nodegraph
