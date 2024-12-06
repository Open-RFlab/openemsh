///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "ui/qt/user_types.hpp"
#include "ui/qt/utils/nodegraph/node.hpp"

namespace domain {
class ConflictColinearEdges;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class ProcessingConflictColinearEdges : public nodegraph::Node {
public:
	enum { Type = UserType + UserTypes::PROCESSING_CONFLICT_CE };

	explicit ProcessingConflictColinearEdges(domain::ConflictColinearEdges const* conflict, QGraphicsItem* parent = nullptr);
	~ProcessingConflictColinearEdges();

	int type() const override;

	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

private:
	domain::ConflictColinearEdges const* const conflict;
};

} // namespace ui::qt
