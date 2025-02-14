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
class ConflictColinearEdges;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class StructureConflictColinearEdges : public QGraphicsLineItem {
public:
	enum { Type = UserTypes::STRUCTURE_CONFLICT_CE };

	struct Params final {
		QPen regular = Qt::NoPen;
		QPen selected = QPen(Qt::red, 0, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		QPen regular_hovered = regular;
		QPen selected_hovered = selected;
	};

	std::function<Params const& ()> locate_structure_conflict_ce_params;

	StructureConflictColinearEdges(domain::ViewAxis axis, domain::ConflictColinearEdges const* conflict, QRectF const& scene_rect, QGraphicsItem* parent = nullptr);

	int type() const override;
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

	domain::ViewAxis const axis;

private:
	domain::ConflictColinearEdges const* const conflict;
};

} // namespace ui::qt
