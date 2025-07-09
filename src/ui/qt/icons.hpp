///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QPixmap>

class QGraphicsItem;

namespace ui::qt {

//******************************************************************************
struct Icons {
	Icons() = delete;

	static QPixmap const& select(QGraphicsItem const* item);

	static QPixmap const& edge();
	static QPixmap const& polygon();
	static QPixmap const& interval_h();
	static QPixmap const& interval_v();
	static QPixmap const& meshline_policy_h();
	static QPixmap const& meshline_policy_v();
	static QPixmap const& conflict_ce_h();
	static QPixmap const& conflict_ce_v();
	static QPixmap const& conflict_tcmlp_h();
	static QPixmap const& conflict_tcmlp_v();
	static QPixmap const& meshline_h();
	static QPixmap const& meshline_v();
};

} // namespace ui::qt
