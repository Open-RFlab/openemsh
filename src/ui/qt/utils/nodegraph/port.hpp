///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsLayoutItem>
#include <QGraphicsSimpleTextItem>

namespace ui::qt::nodegraph {

class Wire;

//******************************************************************************
class Port
: public QGraphicsSimpleTextItem
, public QGraphicsLayoutItem
{
public:
	enum class AnchorPoint { TOP, BOTTOM, LEFT, RIGHT };

	Port(QString const& text, AnchorPoint anchor, QGraphicsItem* parent = nullptr);
	~Port();

	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;
	void setGeometry(QRectF const& geom) override;

	QPointF attach_pos() const;

protected:
	QVariant itemChange(GraphicsItemChange change, QVariant const& value) override;
	QSizeF sizeHint(Qt::SizeHint which, QSizeF const& constraint = QSizeF()) const override;

private:
	friend class Wire;

	static qreal constexpr radius = 5;
	AnchorPoint const anchor;
	Wire* wire; // TODO QList<Wire*>
};

} // namespace ui:qt::nodegraph
