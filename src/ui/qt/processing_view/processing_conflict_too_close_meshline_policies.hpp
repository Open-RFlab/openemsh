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
class ConflictTooCloseMeshlinePolicies;
} // namespace domain

namespace ui::qt {

class EditModel;
class ProcessingMeshlinePolicy;

//******************************************************************************
class ProcessingConflictTooCloseMeshlinePolicies : public nodegraph::Node {
public:
	enum { Type = UserTypes::PROCESSING_CONFLICT_TCMLP };

	struct Params final {
		nodegraph::Node::Params const& node = default_locator<nodegraph::Node::Params>();
		nodegraph::Text::Params const& title = default_locator<nodegraph::Text::Params>();
		nodegraph::Port::Params const& port = default_locator<nodegraph::Port::Params>();
	};

	std::function<Params const& ()> locate_processing_conflict_tcmlp_params;

	explicit ProcessingConflictTooCloseMeshlinePolicies(domain::ConflictTooCloseMeshlinePolicies const* conflict, QGraphicsItem* parent = nullptr);

	int type() const override;

	QList<ProcessingMeshlinePolicy*> get_mlp_origins() const;
	std::size_t count_tcmlp_mlp_deepness() const;

private:
	friend EditModel;

	domain::ConflictTooCloseMeshlinePolicies const* const conflict;
};

} // namespace ui::qt
