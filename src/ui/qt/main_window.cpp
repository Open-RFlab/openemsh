///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QDesktopServices>
#include <QFileDialog>
#include <QGuiApplication>
#include <QMarginsF>
#include <QMessageBox>
#include <QToolButton>

#include <format>

#include "domain/geometrics/space.hpp"
#include "edit/edit_dialog.hpp"
#include "edit/edit_model.hpp"
#include "edit/edit_model_global.hpp"
#include "processing_view/processing_view.hpp"
#include "structure_view/structure_view.hpp"
#include "utils/state_management.hpp"
#include "utils/unreachable.hpp"
#include "about_dialog.hpp"
#include "progress.hpp"
#include "settings.hpp"

#include "ui_main_window.h"
#include "main_window.hpp"

namespace ui::qt {

//******************************************************************************
using DisplayMode = ProcessingScene::DisplayMode;
using MeshVisibility = StructureScene::MeshVisibility;

//******************************************************************************
MainWindow::MainWindow(app::OpenEMSH& oemsh, QWidget* parent)
: QMainWindow(parent)
, ui(std::make_unique<Ui::MainWindow>())
, oemsh(oemsh)
, dock_layout_order(false)
, csx_file(oemsh.get_params().input.empty()
	? QString()
	: QString::fromStdString(oemsh.get_params().input.generic_string()))
{
	setWindowState(Qt::WindowMaximized);
	ui->setupUi(this);

	// TODO Init StructureView & ProcessingView stuff from buttons default values

	ui->statusBar->addPermanentWidget(ui->l_cell_number);

	Progress::singleton().register_impl_builder(
		[this](std::size_t max, std::string const& message) {
			return std::make_unique<ProgressBar>(ui->statusBar, max,
				QString::fromStdString(std::format("{}/{} {}",
					app::index(this->oemsh.get_current_step()),
					app::index_max(),
					message)));
		});

	for(auto const& style : Style::available_styles) {
		auto* const action = new QAction(style.name, ui->ag_styles);
		action->setCheckable(true);
		if(style.name == "Main") {
			action->setChecked(true);
			set_style(style);
		}
		ui->m_style->addAction(action);
	}

	Settings::singleton() = {
		.does_use_material_color = ui->a_does_use_csx_properties_color->isChecked()
	};
}

//******************************************************************************
MainWindow::~MainWindow() = default;

//******************************************************************************
bool MainWindow::parse_and_display() {
	if(csx_file.isEmpty())
		return false;

	QGuiApplication::setOverrideCursor(Qt::WaitCursor);

	if(auto res = oemsh.parse()
	; !res) {
		QGuiApplication::restoreOverrideCursor();
		ui->statusBar->showMessage("Error parsing file \"" + csx_file + "\"" + " : " + QString::fromStdString(res.error()));
		return false;
	}

	update_title();
	ui->structure_view->init(&oemsh.get_board());
	ui->processing_view->init(&oemsh.get_board());
	run();
	QGuiApplication::restoreOverrideCursor();
	return true;
}

//******************************************************************************
void MainWindow::update_cell_number(bool reset) {
	static QString const base_str(ui->l_cell_number->text());
	// TODO space padding every 10^3
	ui->l_cell_number->setText(reset
		? base_str
		: base_str + QString::number(oemsh.get_board().get_mesh_cell_number()));
}

//******************************************************************************
void MainWindow::update_title() {
	static QString const base_title(windowTitle());

	if(!csx_file.isEmpty())
		setWindowTitle(csx_file + " - " + base_title);
}

//******************************************************************************
void MainWindow::clear() {
	ui->structure_view->clear();
	ui->processing_view->clear();
	ui->statusBar->clearMessage();
	update_cell_number(true);
}

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
void MainWindow::on_tb_plane_xy_clicked() {
	ui->structure_view->set_display_plane(domain::XY);
	ui->processing_view->set_display_plane(domain::XY);
}

//******************************************************************************
void MainWindow::on_tb_plane_yz_clicked() {
	ui->structure_view->set_display_plane(domain::YZ);
	ui->processing_view->set_display_plane(domain::YZ);
}

//******************************************************************************
void MainWindow::on_tb_plane_zx_clicked() {
	ui->structure_view->set_display_plane(domain::ZX);
	ui->processing_view->set_display_plane(domain::ZX);
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
void MainWindow::on_a_fit_triggered() {
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
	ui->structure_view->set_mesh_visibility(MeshVisibility::FULL);
	ui->processing_view->set_display_view_axes({ true, true });
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_horizontal_mesh_clicked() {
	ui->structure_view->set_mesh_visibility(MeshVisibility::HORIZONTAL);
	ui->processing_view->set_display_view_axes({ true, false });
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_vertical_mesh_clicked() {
	ui->structure_view->set_mesh_visibility(MeshVisibility::VERTICAL);
	ui->processing_view->set_display_view_axes({ false, true });
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_no_mesh_clicked() {
	ui->structure_view->set_mesh_visibility(MeshVisibility::NONE);
	ui->processing_view->set_display_view_axes({ false, false });
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_selected_clicked() {
	ui->processing_view->set_display_mode(DisplayMode::SELECTED_CHAIN);
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_displayed_clicked() {
	ui->processing_view->set_display_mode(DisplayMode::STRUCTURE_VIEW);
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_show_everything_clicked() {
	ui->processing_view->set_display_mode(DisplayMode::EVERYTHING);
	ui->processing_view->fit();
}

//******************************************************************************
void MainWindow::on_tb_curved_wires_clicked() {
	ui->processing_view->set_wire_style(nodegraph::Wire::Style::CURVED);
}

//******************************************************************************
void MainWindow::on_tb_direct_wires_clicked() {
	ui->processing_view->set_wire_style(nodegraph::Wire::Style::DIRECT);
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
static QString const format_filter_csx("OpenEMS CSX file (*.csx *.xml)");

//******************************************************************************
void MainWindow::on_a_file_open_triggered() {
	QFileDialog dialog(this, ui->a_file_open->toolTip());
	dialog.setAcceptMode(QFileDialog::AcceptOpen);
	dialog.setFileMode(QFileDialog::ExistingFile);
	dialog.setNameFilter(format_filter_csx);
	dialog.setDirectory(csx_file.isEmpty() ? QString(".") : QFileInfo(csx_file).path());
	if(dialog.exec()) {
		csx_file = dialog.selectedFiles().first();

		clear();
		oemsh.set_input(csx_file.toStdString());
		if(!parse_and_display())
			return;

		on_a_fit_triggered();
	}
}

//******************************************************************************
void MainWindow::save_csx_file() {
	oemsh.set_output_format(app::OpenEMSH::Params::OutputFormat::CSX);

	if(oemsh.is_about_overwriting()) {
		auto res = QMessageBox::warning(this,
			"",
			QString("You are about overwriting the file \"%1\", do you want to continue?")
				.arg(oemsh.get_params().output.generic_string()),
			QMessageBox::Cancel | QMessageBox::Save);
		if(res != QMessageBox::Save) {
			return;
		}
	}

	QGuiApplication::setOverrideCursor(Qt::WaitCursor);
	if(auto res = oemsh.write()
	; res)
		ui->statusBar->showMessage("Saved file \"" + csx_file + "\"");
	else
		ui->statusBar->showMessage("Failed to save file \"" + csx_file + "\"" + " : " + QString::fromStdString(res.error()));
	QGuiApplication::restoreOverrideCursor();
}

//******************************************************************************
void MainWindow::on_a_file_save_triggered() {
	if(oemsh.get_params().output.empty()) {
		oemsh.set_output(csx_file.toStdString());
	} else if(csx_file != oemsh.get_params().output.generic_string()) {
		auto res = QMessageBox::question(this,
			"",
			QString("You are about saving to the file \"%1\" which is different from the input file \"%2\", do you want to continue?")
				.arg(oemsh.get_params().output.generic_string(), oemsh.get_params().input.generic_string()),
			QMessageBox::Cancel | QMessageBox::Save);
		if(res == QMessageBox::Save) {
			csx_file = QString::fromStdString(oemsh.get_params().output.generic_string());
		} else {
			return;
		}
	}

	// TODO deduce format from csx_file suffix
	save_csx_file();
}

//******************************************************************************
void MainWindow::on_a_file_save_as_triggered() {
	QFileDialog dialog(this, ui->a_file_save_as->toolTip());
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setNameFilter(format_filter_csx);
	dialog.setDefaultSuffix(".csx");
	dialog.setDirectory(csx_file.isEmpty() ? QString(".") : QFileInfo(csx_file).path());
	if(dialog.exec()) {
		csx_file = dialog.selectedFiles().first();
		update_title();

		// TODO deduce from filter selected by user
		// dialog.selectedNameFilter(); // TODO check actual suffix with that ?
		oemsh.set_output(csx_file.toStdString());
		save_csx_file();
	}
}

//******************************************************************************
void MainWindow::on_a_edit_triggered() {
	auto* widget = static_cast<QToolButton*>(ui->toolBar->widgetForAction(ui->a_edit));
	widget->setDown(true);
	ui->processing_view->get_current_state().scene->edit_selected_nodes(widget->mapToGlobal(widget->rect().bottomLeft()));
	widget->setDown(false);
}

//******************************************************************************
void MainWindow::edit_global_params() {
	EditModelGlobal model(oemsh.get_board().global_params.get());
	EditDialog edit(&model, "global parameters");
	connect(
		&model, &EditModel::edit_from,
		this, &MainWindow::handle_edition_from);
	edit.exec();
}

//******************************************************************************
void MainWindow::on_a_mesh_prev_triggered() {
	QGuiApplication::setOverrideCursor(Qt::WaitCursor);
	oemsh.go_before_previous_step();
	go_to_or_make_current_state();
	QGuiApplication::restoreOverrideCursor();
}

// TODO require some processing fit()
//******************************************************************************
void MainWindow::on_a_mesh_next_triggered() {
	QGuiApplication::setOverrideCursor(Qt::WaitCursor);
	oemsh.run_next_step();
	go_to_or_make_current_state();
	QGuiApplication::restoreOverrideCursor();
}

//******************************************************************************
void MainWindow::on_a_undo_triggered() {
	QGuiApplication::setOverrideCursor(Qt::WaitCursor);
	Caretaker::singleton().undo();
	go_to_or_make_current_state();
	QGuiApplication::restoreOverrideCursor();
}

//******************************************************************************
void MainWindow::on_a_redo_triggered() {
	QGuiApplication::setOverrideCursor(Qt::WaitCursor);
	Caretaker::singleton().redo();
	go_to_or_make_current_state();
	QGuiApplication::restoreOverrideCursor();
}

//******************************************************************************
void MainWindow::on_a_does_use_csx_properties_color_triggered() {
	Settings::singleton().does_use_material_color = ui->a_does_use_csx_properties_color->isChecked();
	ui->structure_view->viewport()->update();
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
	update_navigation_buttons_visibility();
	update_cell_number();
	update_show_buttons_pressing();
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

	connect(
		ui->processing_view->states[t].scene, &ProcessingScene::edit_global_params,
		this, &MainWindow::edit_global_params);

	connect(
		ui->processing_view->states[t].scene, &ProcessingScene::edit_from,
		this, &MainWindow::handle_edition_from);

	update_navigation_buttons_visibility();
	update_cell_number();
}

//******************************************************************************
void MainWindow::run(app::Step from) {
	oemsh.run_from_step(from);
	make_current_state_view();
}

//******************************************************************************
void MainWindow::run() {
	oemsh.run_all_steps();
	make_current_state_view();
}

//******************************************************************************
void MainWindow::handle_edition_from(app::Step from, std::function<void ()> const& edit) {
	oemsh.go_before(from);
	edit();
	run(from);
}

//******************************************************************************
void MainWindow::update_navigation_buttons_visibility() {
	ui->a_undo->setEnabled(Caretaker::singleton().can_undo());
	ui->a_redo->setEnabled(Caretaker::singleton().can_redo());
	ui->a_mesh_prev->setEnabled(oemsh.can_go_before());
	ui->a_mesh_next->setEnabled(oemsh.can_run_a_next_step());
};

//******************************************************************************
void MainWindow::update_show_buttons_pressing() {
	switch(ui->structure_view->get_mesh_visibility()) {
	case MeshVisibility::NONE: ui->tb_show_no_mesh->setChecked(true); break;
	case MeshVisibility::VERTICAL: ui->tb_show_vertical_mesh->setChecked(true); break;
	case MeshVisibility::HORIZONTAL: ui->tb_show_horizontal_mesh->setChecked(true); break;
	case MeshVisibility::FULL: ui->tb_show_all_mesh->setChecked(true); break;
	default: unreachable();
	}

	switch(ui->processing_view->get_display_mode()) {
	case DisplayMode::EVERYTHING: ui->tb_show_everything->setChecked(true); break;
	case DisplayMode::STRUCTURE_VIEW: ui->tb_show_displayed->setChecked(true); break;
	case DisplayMode::SELECTED_CHAIN: ui->tb_show_selected->setChecked(true); break;
	default: unreachable();
	}
}

//******************************************************************************
void MainWindow::keyPressEvent(QKeyEvent* event) {
	if(event->key() == Qt::Key_E || event->key() == Qt::Key_Space) {
		on_a_edit_triggered();
	} else if(event->key() == Qt::Key_F) {
		on_a_fit_triggered();
	} else if(event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_O) {
		on_a_file_open_triggered();
	} else if(event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_S) {
		if(event->modifiers() & Qt::ShiftModifier) {
			on_a_file_save_as_triggered();
		} else {
			on_a_file_save_triggered();
		}
	} else if(event->modifiers() & Qt::ControlModifier && event->key() == Qt::Key_Z) {
		if(event->modifiers() & Qt::ShiftModifier) {
			on_a_redo_triggered();
		} else {
			on_a_undo_triggered();
		}
	} else if(event->key() == Qt::Key_Greater) {
		on_a_mesh_next_triggered();
	} else if(event->key() == Qt::Key_Less) {
		on_a_mesh_prev_triggered();
	} else if(event->key() == Qt::Key_1) {
		ui->tb_show_selected->click();
	} else if(event->key() == Qt::Key_2) {
		ui->tb_show_displayed->click();
	} else if(event->key() == Qt::Key_3) {
		ui->tb_show_everything->click();
	} else if(event->key() == Qt::Key_C) {
		ui->tb_curved_wires->click();
	} else if(event->key() == Qt::Key_D) {
		ui->tb_direct_wires->click();
	} else if(event->key() == Qt::Key_X) {
		ui->tb_show_all_mesh->click();
	} else if(event->key() == Qt::Key_V) {
		ui->tb_show_vertical_mesh->click();
	} else if(event->key() == Qt::Key_H) {
		ui->tb_show_horizontal_mesh->click();
	} else if(event->key() == Qt::Key_Period) {
		ui->tb_show_no_mesh->click();
	} else if(event->key() == Qt::Key_PageUp) {
		if(auto const* b = ui->bg_plane->checkedButton()
		; b == ui->tb_plane_xy) {
			ui->tb_plane_zx->click();
		} else if(b == ui->tb_plane_zx) {
			ui->tb_plane_yz->click();
		} else if(b == ui->tb_plane_yz) {
			ui->tb_plane_xy->click();
		}
	} else if(event->key() == Qt::Key_PageDown) {
		if(auto const* b = ui->bg_plane->checkedButton()
		; b == ui->tb_plane_xy) {
			ui->tb_plane_yz->click();
		} else if(b == ui->tb_plane_zx) {
			ui->tb_plane_xy->click();
		} else if(b == ui->tb_plane_yz) {
			ui->tb_plane_zx->click();
		}
	} else if(event->key() == Qt::Key_Escape) {
		// Will be forwarded to the 3 StructureScenes
		if(auto* scene = ui->processing_view->scene(); scene)
			scene->clearSelection();
	} else {
		QWidget::keyPressEvent(event);
	}
}

} // namespace ui::qt
