///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsRectItem>
#include <QBrush>

#include <functional>

#include "domain/geometrics/space.hpp"
#include "ui/qt/user_types.hpp"

namespace domain {
class ConflictDiagonalOrCircularZone;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class StructureConflictDiagonalOrCircularZone : public QGraphicsRectItem {
public:
	enum { Type = UserTypes::STRUCTURE_CONFLICT_DOCZ };

	struct Params final {
		QBrush regular = Qt::transparent;
		QBrush selected = QColor(255, 0, 0, 100);
		QBrush regular_hovered = regular;
		QBrush selected_hovered = selected;
	};

	std::function<Params const& ()> locate_structure_conflict_docz_params;

	StructureConflictDiagonalOrCircularZone(domain::ViewAxis axis, domain::ConflictDiagonalOrCircularZone const* conflict, QRectF const& scene_rect, QGraphicsItem* parent = nullptr);

	int type() const override;

	domain::ViewAxis const axis;

protected:
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;
	QVariant itemChange(GraphicsItemChange change, QVariant const& value) override;

private:
	domain::ConflictDiagonalOrCircularZone const* const conflict;
};

} // namespace ui::qt
