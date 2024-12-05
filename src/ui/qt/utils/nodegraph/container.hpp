///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QSizeF>

#include "node.hpp"

namespace ui::qt::nodegraph {

class Rect;

//******************************************************************************
class Container : public Node {
public:
	explicit Container(QSizeF margins, QGraphicsItem* parent = nullptr);
	Container(QString title, QSizeF margins, QGraphicsItem* parent = nullptr);
	~Container();

	void expand();
	void collapse();
	void fit();
	void add(QGraphicsItem* item);
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

protected:
	Rect* const nested_zone;
	QSizeF const margins;
};

} // namespace ui::qt::nodegraph
