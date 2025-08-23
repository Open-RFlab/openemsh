///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QGuiApplication>
#include <QPainter>
#include <QPixmap>
#include <QString>
#include <QStyleHints>
#include <QTransform>
#include <QtGlobal>

#include "structure_view/structure_conflict_colinear_edges.hpp"
#include "structure_view/structure_conflict_too_close_meshline_policies.hpp"
#include "structure_view/structure_interval.hpp"
#include "structure_view/structure_meshline.hpp"
#include "structure_view/structure_meshline_policy.hpp"

#include "user_types.hpp"
#include "utils/concepts.hpp"
#include "utils/unreachable.hpp"

#include "icons.hpp"

namespace ui::qt {

//******************************************************************************
auto draw_icon_from_text(auto str) {
	return [=](int size, QPainter& painter) {
		painter.setFont(QFont("Arial", size / 2));
		painter.drawText(QRect(0, 0, size, size), Qt::AlignCenter, QString::fromUtf8(str));
	};
}

//******************************************************************************
template<InvocableR<void, int, QPainter&> Drawer>
QPixmap make_pixmap(Drawer const& draw) {
	int const size = 24;
	QPixmap icon(size, size);
	icon.fill(Qt::transparent);
	QPainter painter(&icon);
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
	switch(QGuiApplication::styleHints()->colorScheme()) {
	default: [[fallthrough]];
	case Qt::ColorScheme::Light: painter.setPen(Qt::black); break;
	case Qt::ColorScheme::Dark: painter.setPen(Qt::white); break;
	}
#else
	painter.setPen(Qt::black);
#endif
	draw(size, painter);
	return icon;
};

//******************************************************************************
static QTransform const rotate_90_cw = []() {
	QTransform t;
	t.rotate(90);
	return t;
} ();

//******************************************************************************
static QTransform const rotate_90_ccw = []() {
	QTransform t;
	t.rotate(-90);
	return t;
} ();

//******************************************************************************
QPixmap crop_to_original_size(QPixmap const& to_crop, QPixmap const& orig) {
	int xoffset = (to_crop.width() - orig.width()) / 2;
	int yoffset = (to_crop.height() - orig.height()) / 2;
	return to_crop.copy(xoffset, yoffset, orig.width(), orig.height());
}

//******************************************************************************
QPixmap apply(QTransform const& transform, QPixmap const& pixmap) {
	return crop_to_original_size(pixmap.transformed(transform), pixmap);
}

//******************************************************************************
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
#define PIXMAP_MAKER_DEF(NAME, FUNC) \
	QPixmap const& Icons::NAME() { \
		switch(QGuiApplication::styleHints()->colorScheme()) { \
		default: [[fallthrough]]; \
		case Qt::ColorScheme::Light: { \
			static QPixmap const light = FUNC; \
			return light; \
		} \
		case Qt::ColorScheme::Dark: { \
			static QPixmap const dark = FUNC; \
			return dark; \
		} \
		} \
	}
#else
#define PIXMAP_MAKER_DEF(NAME, FUNC) \
	QPixmap const& Icons::NAME() { \
		static QPixmap const light = FUNC; \
		return light; \
	}
#endif

//******************************************************************************
PIXMAP_MAKER_DEF(edge,              make_pixmap(draw_icon_from_text("/")))
PIXMAP_MAKER_DEF(polygon,           make_pixmap(draw_icon_from_text("▱")))
PIXMAP_MAKER_DEF(conflict_ce_v,     make_pixmap(draw_icon_from_text("┆")))
PIXMAP_MAKER_DEF(conflict_tcmlp_v,  make_pixmap(draw_icon_from_text("‖")))
PIXMAP_MAKER_DEF(interval_v,        make_pixmap(draw_icon_from_text("▥")))
PIXMAP_MAKER_DEF(meshline_v,        make_pixmap(draw_icon_from_text("|")))
PIXMAP_MAKER_DEF(meshline_policy_v, make_pixmap(draw_icon_from_text("⟊")))
PIXMAP_MAKER_DEF(conflict_ce_h,     apply(rotate_90_cw, conflict_ce_v()))
PIXMAP_MAKER_DEF(conflict_tcmlp_h,  apply(rotate_90_cw, conflict_tcmlp_v()))
PIXMAP_MAKER_DEF(interval_h,        apply(rotate_90_cw, interval_v()))
PIXMAP_MAKER_DEF(meshline_h,        apply(rotate_90_cw, meshline_v()))
PIXMAP_MAKER_DEF(meshline_policy_h, apply(rotate_90_cw, meshline_policy_v()))

//******************************************************************************
QPixmap const& Icons::select(QGraphicsItem const* item) {
	using namespace UserTypes;

	switch(item->type()) {
	case PROCESSING_EDGE: [[fallthrough]];
	case STRUCTURE_EDGE: return edge();
	case PROCESSING_POLYGON: [[fallthrough]];
	case STRUCTURE_POLYGON: return polygon();
	case PROCESSING_INTERVAL: return interval_v();
	case STRUCTURE_INTERVAL:
		switch(static_cast<StructureInterval const*>(item)->axis) {
		case domain::ViewAxis::H: return interval_h();
		case domain::ViewAxis::V: return interval_v();
		default: unreachable();
		}
	case PROCESSING_MESHLINE: return meshline_v();
	case STRUCTURE_MESHLINE:
		switch(static_cast<StructureMeshline const*>(item)->axis) {
		case domain::ViewAxis::H: return meshline_h();
		case domain::ViewAxis::V: return meshline_v();
		default: unreachable();
		}
	case PROCESSING_MESHLINE_POLICY: return meshline_policy_v();
	case STRUCTURE_MESHLINE_POLICY:
		switch(static_cast<StructureMeshlinePolicy const*>(item)->axis) {
		case domain::ViewAxis::H: return meshline_policy_h();
		case domain::ViewAxis::V: return meshline_policy_v();
		default: unreachable();
		}
	case PROCESSING_CONFLICT_CE: return conflict_ce_v();
	case STRUCTURE_CONFLICT_CE:
		switch(static_cast<StructureConflictColinearEdges const*>(item)->axis) {
		case domain::ViewAxis::H: return conflict_ce_h();
		case domain::ViewAxis::V: return conflict_ce_v();
		default: unreachable();
		}
	case PROCESSING_CONFLICT_TCMLP: return conflict_tcmlp_v();
	case STRUCTURE_CONFLICT_TCMLP:
		switch(static_cast<StructureConflictTooCloseMeshlinePolicies const*>(item)->axis) {
		case domain::ViewAxis::H: return conflict_tcmlp_h();
		case domain::ViewAxis::V: return conflict_tcmlp_v();
		default: unreachable();
		}
	case PROCESSING_CONFLICT_EIP: [[fallthrough]];
	case PROCESSING_AXIS: [[fallthrough]];
	case PROCESSING_PLANE: [[fallthrough]];
	default:
		static auto const empty = QPixmap();
		return empty;
	}
}

} // namespace ui::qt
