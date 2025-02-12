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

	ui->structure_view->populate(&oemsh.get_board());
	ui->processing_view->populate(&oemsh.get_board());

	ui->structure_view->setScene(ui->structure_view->scenes[domain::XY]);
	ui->processing_view->processing_scene->set_display_plane(domain::XY);
	ui->processing_view->processing_scene->set_display_view_axes({ true, true });
}

//******************************************************************************
MainWindow::~MainWindow() = default;

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

	ui->structure_view->reset_rotation();
	ui->structure_view->fit();
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

//******************************************************************************
void MainWindow::on_tb_structure_rotate_cw_clicked() {
	ui->structure_view->rotate_view(-5);
}

//******************************************************************************
void MainWindow::on_tb_structure_rotate_ccw_clicked() {
	ui->structure_view->rotate_view(5);
}

//******************************************************************************
void MainWindow::on_tb_structure_zoom_in_clicked() {
	ui->structure_view->scale(1.2, 1.2);
}

//******************************************************************************
void MainWindow::on_tb_structure_zoom_out_clicked() {
	ui->structure_view->scale(1 / 1.2, 1 / 1.2);
}

//******************************************************************************
void MainWindow::on_tb_processing_zoom_in_clicked() {
	ui->processing_view->scale(1.2, 1.2);
}

//******************************************************************************
void MainWindow::on_tb_processing_zoom_out_clicked() {
	ui->processing_view->scale(1 / 1.2, 1 / 1.2);
}

} // namespace ui::qt
