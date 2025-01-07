///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QPen>
#include <QGraphicsLayoutItem>
#include <QGraphicsSimpleTextItem>

#include <functional>

namespace ui::qt::nodegraph {

//******************************************************************************
class Text
: public QGraphicsSimpleTextItem
, public QGraphicsLayoutItem
{
public:
	struct Params final {
		QPen regular = QPen(Qt::white);
		QPen highlighted = regular;
		QPen selected = regular;
		QPen selected_highlighted = selected;
	};

	std::function<Params const& ()> locate_text_params;

	explicit Text(QString const& text, QGraphicsItem* parent = nullptr);
	~Text();

	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;
	void setGeometry(QRectF const& geom) override;

protected:
	QSizeF sizeHint(Qt::SizeHint which, QSizeF const& constraint = QSizeF()) const override;
};

} // namespace ui::qt::nodegraph
