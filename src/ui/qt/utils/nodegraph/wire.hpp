///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QPen>
#include <QGraphicsPathItem>

#include <functional>

namespace ui::qt::nodegraph {

class Port;

//******************************************************************************
class Wire
: public QGraphicsPathItem
{
public:
	enum class Style { DIRECT, CURVED } style;

	struct Params final {
		QPen regular = QPen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap);
		QPen highlighted = QPen(QColor(Qt::green).lighter(), 4, Qt::SolidLine, Qt::RoundCap);
		QPen selected = regular;
		QPen regular_hovered = regular;
		QPen highlighted_hovered = highlighted;
		QPen selected_hovered = selected;
	};

	std::function<Params const& ()> locate_wire_params;

	Wire(Port* begin, Port* end, QGraphicsItem* parent = nullptr);

	void update_path();

	Wire* unwire(Port const* called_from_dtor_of = nullptr);
	Port* traverse(Port const* port) const;

protected:
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;
	QVariant itemChange(GraphicsItemChange change, QVariant const& value) override;

private:
	Port* begin;
	Port* end;
};

} // namespace ui::qt::nodegraph
