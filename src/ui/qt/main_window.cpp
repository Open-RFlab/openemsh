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
#include "utils/state_management.hpp"
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

	oemsh.parse();
	oemsh.run_all_steps();

	ui->structure_view->init(&oemsh.get_board());
	ui->processing_view->init(&oemsh.get_board());
	handle_edition();

	ui->structure_view->set_display_plane(domain::XY);
	ui->processing_view->get_current_state().scene->set_display_plane(domain::XY);
	ui->processing_view->get_current_state().scene->set_display_view_axes({ true, true });
//	ui->structure_view->setup_scale_max(ui->structure_view->scenes[domain::XY].polygons->boundingRect());
//	on_tb_reset_clicked();
}

//******************************************************************************
MainWindow::~MainWindow() = default;

//******************************************************************************
void MainWindow::set_style(Style const& style) {
	ui->processing_view->setBackgroundBrush(style.processing.background);
	ui->structure_view->setBackgroundBrush(style.structure.background);
	ui->processing_view->style_selector = style.processing;
	ui->structure_view->style_selector = style.structure;
}

//******************************************************************************
void MainWindow::on_a_about_triggered() {
	AboutDialog about(this);
	about.exec();
}

//******************************************************************************
void MainWindow::on_a_doc_oems_meshing_triggered() const {
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
		ui->structure_view->set_display_plane(domain::XY);
		ui->processing_view->get_current_state().scene->set_display_plane(domain::XY);
	}
}

//******************************************************************************
void MainWindow::on_rb_plane_yz_toggled(bool const is_checked) {
	if(is_checked) {
		ui->structure_view->set_display_plane(domain::YZ);
		ui->processing_view->get_current_state().scene->set_display_plane(domain::YZ);
	}
}

//******************************************************************************
void MainWindow::on_rb_plane_zx_toggled(bool const is_checked) {
	if(is_checked) {
		ui->structure_view->set_display_plane(domain::ZX);
		ui->processing_view->get_current_state().scene->set_display_plane(domain::ZX);
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
void MainWindow::on_a_reset_triggered() {
	ui->processing_view->get_current_state().scene->fit_containers();
	ui->processing_view->get_current_state().scene->fit_scene();
	ui->processing_view->fit();

	ui->structure_view->reset_rotation();
	ui->structure_view->fit();
}

//******************************************************************************
void MainWindow::on_a_horizontal_layout_triggered() {
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
void MainWindow::on_a_vertical_layout_triggered() {
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
	ui->processing_view->get_current_state().scene->set_display_view_axes({ true, true });
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_horizontal_mesh_clicked() {
	ui->structure_view->set_mesh_visibility(StructureScene::MeshVisibility::HORIZONTAL);
	ui->processing_view->get_current_state().scene->set_display_view_axes({ true, false });
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_vertical_mesh_clicked() {
	ui->structure_view->set_mesh_visibility(StructureScene::MeshVisibility::VERTICAL);
	ui->processing_view->get_current_state().scene->set_display_view_axes({ false, true });
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_no_mesh_clicked() {
	ui->structure_view->set_mesh_visibility(StructureScene::MeshVisibility::NONE);
	ui->processing_view->get_current_state().scene->set_display_view_axes({ false, false });
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_selected_clicked() {
	ui->processing_view->get_current_state().scene->set_display(ProcessingScene::DisplayMode::SELECTED_CHAIN);
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_displayed_clicked() {
	ui->processing_view->get_current_state().scene->set_display(ProcessingScene::DisplayMode::STRUCTURE_VIEW);
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_everything_clicked() {
	ui->processing_view->get_current_state().scene->set_display(ProcessingScene::DisplayMode::EVERYTHING);
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_curved_wires_clicked() {
	ui->processing_view->get_current_state().scene->set_wire_style(nodegraph::Wire::Style::CURVED);
}

//******************************************************************************
void MainWindow::on_tb_direct_wires_clicked() {
	ui->processing_view->get_current_state().scene->set_wire_style(nodegraph::Wire::Style::DIRECT);
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

//******************************************************************************
void MainWindow::on_a_mesh_prev_triggered() {
	setCursor(Qt::WaitCursor);
	oemsh.go_before_previous_step();
	go_to_or_make_current_state();
	unsetCursor();
}

// TODO require some processing fit()
//******************************************************************************
void MainWindow::on_a_mesh_next_triggered() {
	setCursor(Qt::WaitCursor);
	oemsh.run_next_step();
	go_to_or_make_current_state();
	unsetCursor();
}

//******************************************************************************
void MainWindow::on_a_undo_triggered() {
	setCursor(Qt::WaitCursor);
	Caretaker::singleton().undo();
	go_to_or_make_current_state();
	unsetCursor();
}

//******************************************************************************
void MainWindow::on_a_redo_triggered() {
	setCursor(Qt::WaitCursor);
	Caretaker::singleton().redo();
	go_to_or_make_current_state();
	unsetCursor();
}

//******************************************************************************
void MainWindow::go_to_or_make_current_state() {
	auto* t = Caretaker::singleton().get_current_timepoint();
	if(ui->structure_view->states.contains(t)
	&& ui->processing_view->states.contains(t))
		go_to_current_state();
	else
		make_current_state_view();
}

//******************************************************************************
void MainWindow::go_to_current_state() {
	ui->structure_view->go_to_current_state();
	ui->processing_view->go_to_current_state();
	update_navigation_visibility();
	// TODO handle passing selection from a scene to its own future couterpart
}

//******************************************************************************
void MainWindow::make_current_state_view() {
	Caretaker::singleton().pin_current_timepoint();
	ui->structure_view->make_current_state();
	ui->processing_view->make_current_state();

	auto* t = Caretaker::singleton().get_current_timepoint();
	for(domain::Plane const plane : domain::AllPlane) {
		connect(
			ui->processing_view->states[t].scene, &ProcessingScene::selection_changed,
			ui->structure_view->states[t].scenes[plane], &StructureScene::select_counterparts);
		connect(
			ui->structure_view->states[t].scenes[plane], &StructureScene::selection_changed,
			ui->processing_view->states[t].scene, &ProcessingScene::select_counterparts);
	}

	update_navigation_visibility();
}

//******************************************************************************
void MainWindow::handle_edition(std::set<app::Step> const& to_redo) {
//	Caretaker::singleton().remember_current_timepoint();

//	if(ui->a_mesh_auto->is_checked()) // TODO may not be that useful since editing at once two things from different steps will discard newer objects edits??
		oemsh.run(to_redo);
	Caretaker::singleton().remember_current_timepoint();

	make_current_state_view();
}

//******************************************************************************
void MainWindow::update_navigation_visibility() {
	ui->a_undo->setEnabled(Caretaker::singleton().can_undo());
	ui->a_redo->setEnabled(Caretaker::singleton().can_redo());
	ui->a_mesh_prev->setEnabled(oemsh.can_go_before());
	ui->a_mesh_next->setEnabled(oemsh.can_run_a_next_step());
};

} // namespace ui::qt
