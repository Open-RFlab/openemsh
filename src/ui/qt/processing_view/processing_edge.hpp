///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "ui/qt/user_types.hpp"
#include "ui/qt/utils/nodegraph/node.hpp"

namespace domain {
class Edge;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class ProcessingEdge : public nodegraph::Node {
public:
	enum { Type = UserTypes::PROCESSING_EDGE };

	explicit ProcessingEdge(domain::Edge const* edge, QGraphicsItem* parent = nullptr);
	~ProcessingEdge();

	int type() const override;

	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

private:
	domain::Edge const* const edge;
};

} // namespace ui::qt
