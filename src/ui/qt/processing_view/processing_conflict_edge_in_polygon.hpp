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
class ConflictEdgeInPolygon;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class ProcessingConflictEdgeInPolygon : public nodegraph::Node {
public:
	enum { Type = UserTypes::PROCESSING_CONFLICT_EIP };

	struct Params final {
		nodegraph::Node::Params const& node = default_locator<nodegraph::Node::Params>();
		nodegraph::Text::Params const& title = default_locator<nodegraph::Text::Params>();
		nodegraph::Port::Params const& port = default_locator<nodegraph::Port::Params>();
		nodegraph::Text::Params const& main = default_locator<nodegraph::Text::Params>();
		nodegraph::Text::Params const& enabled = default_locator<nodegraph::Text::Params>();
		nodegraph::Text::Params const& disabled = default_locator<nodegraph::Text::Params>();
	};

	std::function<Params const& ()> locate_processing_conflict_eip_params;

	explicit ProcessingConflictEdgeInPolygon(domain::ConflictEdgeInPolygon const* conflict, QGraphicsItem* parent = nullptr);
	~ProcessingConflictEdgeInPolygon();

	int type() const override;

private:
	domain::ConflictEdgeInPolygon const* const conflict;
};

} // namespace ui::qt
