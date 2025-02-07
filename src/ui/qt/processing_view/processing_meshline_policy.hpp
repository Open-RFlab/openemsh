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
class MeshlinePolicy;
} // namespace domain

namespace ui::qt {

class ProcessingConflictTooCloseMeshlinePolicies;

//******************************************************************************
class ProcessingMeshlinePolicy : public nodegraph::Node {
public:
	enum { Type = UserTypes::PROCESSING_MESHLINE_POLICY };

	struct Params final {
		nodegraph::Node::Params const& node = default_locator<nodegraph::Node::Params>();
		nodegraph::Port::Params const& port = default_locator<nodegraph::Port::Params>();
		nodegraph::Text::Params const& title = default_locator<nodegraph::Text::Params>();
		nodegraph::Text::Params const& main = default_locator<nodegraph::Text::Params>();
		nodegraph::Text::Params const& enabled = default_locator<nodegraph::Text::Params>();
		nodegraph::Text::Params const& enabled_for_sure = default_locator<nodegraph::Text::Params>();
		nodegraph::Text::Params const& disabled = default_locator<nodegraph::Text::Params>();
	};

	std::function<Params const& ()> locate_processing_meshline_policy_params;

	explicit ProcessingMeshlinePolicy(domain::MeshlinePolicy const* meshline_policy, QGraphicsItem* parent = nullptr);
	~ProcessingMeshlinePolicy();

	int type() const override;

	QList<ProcessingConflictTooCloseMeshlinePolicies*> get_tcmlp_origins() const;
	std::size_t count_mlp_tcmlp_deepness() const;

private:
	domain::MeshlinePolicy const* const meshline_policy;
};

} // namespace ui::qt
