///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "ui/qt/user_types.hpp"
#include "ui/qt/utils/nodegraph/node.hpp"
#include "ui/qt/utils/nodegraph/text.hpp"
#include "utils/default_locator.hpp"

namespace domain {
class Edge;
} // namespace domain

namespace ui::qt {

class EditModel;

//******************************************************************************
class ProcessingEdge : public nodegraph::Node {
public:
	enum { Type = UserTypes::PROCESSING_EDGE };

	struct Params final {
		nodegraph::Node::Params const& node = default_locator<nodegraph::Node::Params>();
		nodegraph::Port::Params const& port = default_locator<nodegraph::Port::Params>();
		nodegraph::Text::Params const& title = default_locator<nodegraph::Text::Params>();
		nodegraph::Text::Params const& main = default_locator<nodegraph::Text::Params>();
		nodegraph::Text::Params const& enabled = default_locator<nodegraph::Text::Params>();
		nodegraph::Text::Params const& enabled_for_sure = default_locator<nodegraph::Text::Params>();
		nodegraph::Text::Params const& disabled = default_locator<nodegraph::Text::Params>();
	};

	std::function<Params const& ()> locate_processing_edge_params;

	explicit ProcessingEdge(domain::Edge const* edge, QGraphicsItem* parent = nullptr);

	int type() const override;

private:
	friend EditModel;

	domain::Edge const* const edge;
};

} // namespace ui::qt
