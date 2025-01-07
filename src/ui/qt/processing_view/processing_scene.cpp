///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "ui/qt/data_keys.hpp"
#include "processing_axis.hpp"
#include "processing_plane.hpp"
#include "processing_polygon.hpp"

#include "processing_scene.hpp"

namespace ui::qt {

//******************************************************************************
ProcessingScene::ProcessingScene(QObject* parent)
: QGraphicsScene(parent)
, style_selector(ProcessingStyleSelector::available_styles[0])
{
	connect(
		this, &QGraphicsScene::selectionChanged,
		this, &ProcessingScene::on_selectionChanged);
}

//******************************************************************************
ProcessingScene::~ProcessingScene() {
	disconnect(
		this, &QGraphicsScene::selectionChanged,
		this, &ProcessingScene::on_selectionChanged);
}

//******************************************************************************
void ProcessingScene::set_wire_style(nodegraph::Wire::Style style) {
	for(auto* wire : wires) {
		wire->style = style;
		wire->update_path();
	}
}

//******************************************************************************
void ProcessingScene::fit_containers() {
	for(auto* container : polygons)
		container->fit();
	for(auto* container : planes)
		container->fit();
	for(auto* container : axes)
		container->fit();
}

//******************************************************************************
void ProcessingScene::on_selectionChanged() {
	// Avoid infinite recursion by being synchronized back.
	is_select_counterparts_locked = true;
	emit selection_changed(selectedItems());
	is_select_counterparts_locked = false;
}

//******************************************************************************
void ProcessingScene::select_counterparts(QList<QGraphicsItem*> foreign_items) {
	if(!is_select_counterparts_locked) {
		clearSelection();
		for(auto* foreign_item : foreign_items) {
			Entity const* entity = static_cast<Entity const*>(foreign_item->data(DataKeys::ENTITY).value<void const*>());
			if(index.contains(entity)) {
				index.at(entity)->setSelected(true);
			}
		}
	}
}

} // namespace ui::qt
