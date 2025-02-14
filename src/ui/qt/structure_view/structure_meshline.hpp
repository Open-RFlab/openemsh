///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsLineItem>
#include <QPen>

#include <functional>

#include "domain/geometrics/space.hpp"
#include "ui/qt/user_types.hpp"

namespace domain {
class Meshline;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class StructureMeshline : public QGraphicsLineItem {
public:
	enum { Type = UserTypes::STRUCTURE_MESHLINE };

	struct Params final {
		QPen regular = QPen(Qt::black, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		QPen selected = QPen(Qt::red, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		QPen regular_hovered = regular;
		QPen selected_hovered = selected;
	};

	std::function<Params const& ()> locate_structure_meshline_params;

	StructureMeshline(domain::ViewAxis axis, domain::Meshline const* meshline, QRectF const& scene_rect, QGraphicsItem* parent = nullptr);

	int type() const override;
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

	domain::ViewAxis const axis;

private:
	domain::Meshline const* const meshline;
};

} // namespace ui::qt
