///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QDesktopServices>
#include <QMarginsF>

#include "domain/geometrics/space.hpp"
#include "processing_view/processing_view.hpp"
#include "structure_view/structure_view.hpp"
#include "about_dialog.hpp"

#include "ui_main_window.h"
#include "main_window.hpp"

namespace ui::qt {

//******************************************************************************
MainWindow::MainWindow(app::OpenEMSH& oemsh, QWidget* parent)
: QMainWindow(parent)
, ui(std::make_unique<Ui::MainWindow>())
, oemsh(oemsh)
, dock_layout_order(false)
{
	setWindowState(Qt::WindowMaximized);
	setWindowIcon(QPixmap(":/openemsh.ico"));
	ui->setupUi(this);
	ui->structure_view->setup(ui->s_structure_zoom, ui->s_structure_rotation);
	ui->processing_view->setup(ui->s_processing_zoom);

	for(auto const& style : Style::available_styles) {
		auto* const action = new QAction(style.name, ui->ag_styles);
		action->setCheckable(true);
		if(style.name == "Main") {
			action->setChecked(true);
			set_style(style);
		}
		ui->m_style->addAction(action);
	}

	for(domain::Plane const plane : domain::AllPlane) {
		connect(
			ui->processing_view->processing_scene, &ProcessingScene::selection_changed,
			ui->structure_view->scenes[plane], &StructureScene::select_counterparts);
		connect(
			ui->structure_view->scenes[plane], &StructureScene::selection_changed,
			ui->processing_view->processing_scene, &ProcessingScene::select_counterparts);
	}

	oemsh.parse();
	oemsh.do_all_step();

	update_structure();
	update_processing();

	ui->structure_view->setScene(ui->structure_view->scenes[domain::XY]);
	ui->processing_view->processing_scene->set_display_plane(domain::XY);
	ui->processing_view->processing_scene->set_display_view_axes({ true, true });
}

//******************************************************************************
MainWindow::~MainWindow() = default;

//******************************************************************************
void MainWindow::update_processing() {
	for(domain::Plane const plane : domain::AllPlane) {
		auto* processing_plane = ui->processing_view->processing_scene->add(plane);

		for(auto const& polygon : oemsh.get_board().get_polygons(plane)) {
			auto* processing_polygon = ui->processing_view->processing_scene->add(polygon.get(), processing_plane);

			for(auto const& edge : polygon->edges)
				ui->processing_view->processing_scene->add(edge.get(), processing_polygon);

			processing_polygon->fit();
		}

		for(auto const& conflict : oemsh.get_board().get_conflicts_edge_in_polygons(plane))
			ui->processing_view->processing_scene->add(conflict.get(), processing_plane);

		processing_plane->fit();
	}

	for(domain::Axis const axis : domain::AllAxis) {
		auto* processing_axis = ui->processing_view->processing_scene->add(axis);

		for(auto const& conflict : oemsh.get_board().get_conflicts_colinear_edges(axis))
			ui->processing_view->processing_scene->add(conflict.get(), processing_axis);

		for(auto const& conflict : oemsh.get_board().get_conflicts_too_close_meshline_policies(axis))
			ui->processing_view->processing_scene->add(conflict.get(), processing_axis);

		for(auto const& policy : oemsh.get_board().get_meshline_policies(axis))
			ui->processing_view->processing_scene->add(policy.get(), processing_axis);

		for(auto const& interval : oemsh.get_board().get_intervals(axis))
			ui->processing_view->processing_scene->add(interval.get(), processing_axis);

		for(auto const& meshline : oemsh.get_board().get_meshlines(axis))
			ui->processing_view->processing_scene->add(meshline.get(), processing_axis);

		processing_axis->fit();
	}

	for(auto* edge : ui->processing_view->processing_scene->edges)
		ui->processing_view->processing_scene->wire_to_destination_first_output_port(edge);

	for(auto* conflict : ui->processing_view->processing_scene->conflict_colinear_edges)
		ui->processing_view->processing_scene->wire_to_destination_first_output_port(conflict);

	for(auto* conflict : ui->processing_view->processing_scene->conflict_too_close_meshline_policies)
		ui->processing_view->processing_scene->wire_to_destination_first_output_port(conflict);

	for(auto* conflict : ui->processing_view->processing_scene->conflict_edge_in_polygons)
		ui->processing_view->processing_scene->wire_to_destination_first_output_port(conflict);

	for(auto* policy : ui->processing_view->processing_scene->meshline_policies)
		ui->processing_view->processing_scene->wire_to_destination_first_output_port(policy);

	for(auto* meshline : ui->processing_view->processing_scene->meshlines)
		ui->processing_view->processing_scene->wire_to_destination_first_output_port(meshline);

	for(auto* interval : ui->processing_view->processing_scene->intervals)
		ui->processing_view->processing_scene->wire_to_destination_first_output_port(interval);

	ui->processing_view->processing_scene->fit_scene();
}

//******************************************************************************
void MainWindow::update_structure() {
	for(domain::Plane const plane : domain::AllPlane) {
		ui->structure_view->scenes[plane]->clear();

		for(auto const& polygon : oemsh.get_board().get_polygons(plane)) {
			ui->structure_view->scenes[plane]->add(polygon.get());

			for(auto const& edge : polygon->edges)
				ui->structure_view->scenes[plane]->add(edge.get());
		}

		QRectF const scene_rect(ui->structure_view->scenes[plane]->sceneRect());

		for(domain::Axis const axis : domain::Axes[plane]) {
			if(auto const view_axis = domain::transpose(plane, axis); view_axis) {
				for(auto const& meshline : oemsh.get_board().get_meshlines(axis))
					ui->structure_view->scenes[plane]->add(meshline.get(), view_axis.value(), scene_rect);

				for(auto const& policy : oemsh.get_board().get_meshline_policies(axis))
					ui->structure_view->scenes[plane]->add(policy.get(), view_axis.value(), scene_rect);

				for(auto const& interval : oemsh.get_board().get_intervals(axis))
					ui->structure_view->scenes[plane]->add(interval.get(), view_axis.value(), scene_rect);
			}
		}
	}
}

//******************************************************************************
void MainWindow::set_style(Style const& style) {
	ui->processing_view->setBackgroundBrush(style.processing.background);
	ui->structure_view->setBackgroundBrush(style.structure.background);
	ui->processing_view->processing_scene->style_selector = style.processing;
	ui->structure_view->style_selector = style.structure;
	ui->processing_view->processing_scene->update();
	if(auto* scene = ui->structure_view->scene(); scene)
		scene->update();
}

//******************************************************************************
void MainWindow::on_a_about_triggered() {
	AboutDialog about(this);
	about.exec();
}

//******************************************************************************
void MainWindow::on_a_doc_oems_meshing_triggered() {
	QDesktopServices::openUrl(QUrl(OEMSH_OEMS_MESHING));
}

//******************************************************************************
void MainWindow::on_ag_styles_triggered(QAction* const action) {
	if(auto style = Style::find_style(action->text()); style) {
		set_style(style.value());
	}
}

//******************************************************************************
void MainWindow::on_rb_plane_xy_toggled(bool const is_checked) {
	if(is_checked) {
		ui->structure_view->setScene(ui->structure_view->scenes[domain::XY]);
		ui->structure_view->centerOn(ui->structure_view->scenes[domain::XY]->polygons->boundingRect().center());
		ui->processing_view->processing_scene->set_display_plane(domain::XY);
	}
}

//******************************************************************************
void MainWindow::on_rb_plane_yz_toggled(bool const is_checked) {
	if(is_checked) {
		ui->structure_view->setScene(ui->structure_view->scenes[domain::YZ]);
		ui->structure_view->centerOn(ui->structure_view->scenes[domain::YZ]->polygons->boundingRect().center());
		ui->processing_view->processing_scene->set_display_plane(domain::YZ);
	}
}

//******************************************************************************
void MainWindow::on_rb_plane_zx_toggled(bool const is_checked) {
	if(is_checked) {
		ui->structure_view->setScene(ui->structure_view->scenes[domain::ZX]);
		ui->structure_view->centerOn(ui->structure_view->scenes[domain::ZX]->polygons->boundingRect().center());
		ui->processing_view->processing_scene->set_display_plane(domain::ZX);
	}
}

//******************************************************************************
void MainWindow::on_s_processing_zoom_valueChanged(int const /*value*/) {
	ui->processing_view->transform_view();
}

//******************************************************************************
void MainWindow::on_s_structure_rotation_valueChanged(int const /*value*/) {
	ui->structure_view->transform_view();
}

//******************************************************************************
void MainWindow::on_s_structure_zoom_valueChanged(int const /*value*/) {
	ui->structure_view->transform_view();
}

//******************************************************************************
void MainWindow::on_tb_anchor_clicked(bool const is_checked) {
	if(is_checked) {
		ui->structure_view->setTransformationAnchor(QGraphicsView::AnchorViewCenter);
	} else {
		ui->structure_view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	}
}

//******************************************************************************
void MainWindow::on_tb_reset_clicked() {
	ui->processing_view->processing_scene->fit_containers();
	ui->processing_view->processing_scene->fit_scene();
	ui->processing_view->fit();

	ui->s_structure_rotation->setValue((ui->s_structure_rotation->minimum() + ui->s_structure_rotation->maximum()) / 2);

	// TODO zoom slider must be updated too
	// fitInView operates on the transform matrix
	// https://code.qt.io/cgit/qt/qtbase.git/tree/src/widgets/graphicsview/qgraphicsview.cpp?h=6.8#n2014

	// TODO Fit twice because of possible scrollbars apparition after first fit
	ui->structure_view->fitInView(static_cast<StructureScene*>(ui->structure_view->scene())->polygons->boundingRect() + QMarginsF(5, 5, 5, 5), Qt::KeepAspectRatio);
//	ui->structure_view->fitInView(dynamic_cast<StructureScene*>(ui->structure_view->scene())->polygons->boundingRect() + QMarginsF(5, 5, 5, 5), Qt::KeepAspectRatio);
}

//******************************************************************************
void MainWindow::on_tb_horizontal_layout_clicked() {
	if(dock_layout_order) {
		addDockWidget(Qt::BottomDockWidgetArea, ui->dw_structure);
		addDockWidget(Qt::TopDockWidgetArea, ui->dw_processing);
	} else {
		addDockWidget(Qt::TopDockWidgetArea, ui->dw_structure);
		addDockWidget(Qt::BottomDockWidgetArea, ui->dw_processing);
	}
	dock_layout_order = !dock_layout_order;
}

//******************************************************************************
void MainWindow::on_tb_vertical_layout_clicked() {
	if(dock_layout_order) {
		addDockWidget(Qt::RightDockWidgetArea, ui->dw_structure);
		addDockWidget(Qt::LeftDockWidgetArea, ui->dw_processing);
	} else {
		addDockWidget(Qt::LeftDockWidgetArea, ui->dw_structure);
		addDockWidget(Qt::RightDockWidgetArea, ui->dw_processing);
	}
	dock_layout_order = !dock_layout_order;
}

//******************************************************************************
void MainWindow::on_tb_show_all_mesh_clicked() {
	ui->structure_view->set_mesh_visibility(StructureScene::MeshVisibility::FULL);
	ui->processing_view->processing_scene->set_display_view_axes({ true, true });
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_horizontal_mesh_clicked() {
	ui->structure_view->set_mesh_visibility(StructureScene::MeshVisibility::HORIZONTAL);
	ui->processing_view->processing_scene->set_display_view_axes({ true, false });
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_vertical_mesh_clicked() {
	ui->structure_view->set_mesh_visibility(StructureScene::MeshVisibility::VERTICAL);
	ui->processing_view->processing_scene->set_display_view_axes({ false, true });
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_no_mesh_clicked() {
	ui->structure_view->set_mesh_visibility(StructureScene::MeshVisibility::NONE);
	ui->processing_view->processing_scene->set_display_view_axes({ false, false });
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_selected_clicked() {
	ui->processing_view->processing_scene->set_display(ProcessingScene::DisplayMode::SELECTED_CHAIN);
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_displayed_clicked() {
	ui->processing_view->processing_scene->set_display(ProcessingScene::DisplayMode::STRUCTURE_VIEW);
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_everything_clicked() {
	ui->processing_view->processing_scene->set_display(ProcessingScene::DisplayMode::EVERYTHING);
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_curved_wires_clicked() {
	ui->processing_view->processing_scene->set_wire_style(nodegraph::Wire::Style::CURVED);
}

//******************************************************************************
void MainWindow::on_tb_direct_wires_clicked() {
	ui->processing_view->processing_scene->set_wire_style(nodegraph::Wire::Style::DIRECT);
}

} // namespace ui::qt
