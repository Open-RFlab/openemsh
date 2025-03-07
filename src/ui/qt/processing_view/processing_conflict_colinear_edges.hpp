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
class ConflictColinearEdges;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class ProcessingConflictColinearEdges : public nodegraph::Node {
public:
	enum { Type = UserTypes::PROCESSING_CONFLICT_CE };

	struct Params final {
		nodegraph::Node::Params const& node = default_locator<nodegraph::Node::Params>();
		nodegraph::Text::Params const& title = default_locator<nodegraph::Text::Params>();
		nodegraph::Port::Params const& port_normal = default_locator<nodegraph::Port::Params>();
		nodegraph::Port::Params const& port_enabled = default_locator<nodegraph::Port::Params>();
		nodegraph::Port::Params const& port_disabled = default_locator<nodegraph::Port::Params>();
	};

	std::function<Params const& ()> locate_processing_conflict_ce_params;

	explicit ProcessingConflictColinearEdges(domain::ConflictColinearEdges const* conflict, QGraphicsItem* parent = nullptr);

	int type() const override;

private:
	domain::ConflictColinearEdges const* const conflict;
};

} // namespace ui::qt
