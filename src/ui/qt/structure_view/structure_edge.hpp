///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsLineItem>
#include <QPen>

#include <functional>

#include "ui/qt/user_types.hpp"

namespace domain {
class Edge;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class StructureEdge : public QGraphicsLineItem {
public:
	enum { Type = UserTypes::STRUCTURE_EDGE };

	struct Params final {
		QPen regular = QPen(Qt::NoPen);
		QPen selected = QPen(Qt::red, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		QPen regular_hovered = regular;
		QPen selected_hovered = selected;
	};

	std::function<Params const& ()> locate_structure_edge_params;

	explicit StructureEdge(domain::Edge const* edge, QGraphicsItem* parent = nullptr);

	int type() const override;
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

private:
	domain::Edge const* const edge;
};

} // namespace ui::qt
