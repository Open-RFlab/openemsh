///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QSizeF>

#include <functional>

#include "node.hpp"

namespace ui::qt::nodegraph {

class Rect;

//******************************************************************************
class Container : public Node {
public:
	enum class Spacement { PACKED_UP, PACKED_DOWN, UNPACKED_MAX, UNPACKED_REGULAR } spacement;

	explicit Container(QSizeF margins, QGraphicsItem* parent = nullptr);
	Container(QString title, QSizeF margins, QGraphicsItem* parent = nullptr);
	~Container();

	void expand();
	void collapse();
	void fit();
	void add(QGraphicsItem* item);
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

	void set_highlighted(bool is_highlighted, QGraphicsItem const* by_item) override;

protected:
	bool does_contains_visible_items() const;
	QList<QGraphicsItem*> visible_contained_items() const;

	std::function<std::size_t (QGraphicsItem const*)> get_column;
	Rect* const nested_zone;
	QSizeF const margins;
};

} // namespace ui::qt::nodegraph
