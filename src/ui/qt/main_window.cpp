///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QMarginsF>

#include "domain/geometrics/space.hpp"
#include "domain/geometrics/edge.hpp"
#include "domain/geometrics/polygon.hpp"
#include "domain/mesh/meshline.hpp"
#include "utils/unreachable.hpp"

#include "structure_view/structure_view.hpp"
#include "structure_view/structure_edge.hpp"
#include "structure_view/structure_polygon.hpp"
#include "structure_view/structure_meshline.hpp"
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

	oemsh.parse();
	oemsh.do_all_step();

	update_structure();

	ui->structure_view->setScene(&(ui->structure_view->scenes[domain::XY]));
}

//******************************************************************************
MainWindow::~MainWindow() = default;

//******************************************************************************
void MainWindow::update_structure() {
	for(domain::Plane const plane : domain::AllPlane) {
		ui->structure_view->scenes[plane].clear();

		for(auto const& polygon : oemsh.get_board().get_polygons(plane)) {
			ui->structure_view->scenes[plane].add(new StructurePolygon(polygon.get()));
		}

		for(auto const& polygon : oemsh.get_board().get_polygons(plane)) {
			for(auto const& edge : polygon->edges) {
				ui->structure_view->scenes[plane].add(new StructureEdge(edge.get()));
			}
		}

		QRectF const scene_rect(ui->structure_view->scenes[plane].sceneRect());

		for(domain::Axis const axis : domain::Axes[plane]) {
			for(auto const& meshline : oemsh.get_board().get_meshlines(axis)) {
				if(auto const view_axis = domain::transpose(plane, axis); view_axis) {
					ui->structure_view->scenes[plane].add(new StructureMeshline(reverse(view_axis.value()), meshline.get(), scene_rect));
				}
			}
		}
	}
}

//******************************************************************************
void MainWindow::on_a_about_triggered() {
	AboutDialog about(this);
	about.exec();
}

//******************************************************************************
void MainWindow::on_rb_plane_xy_toggled(bool const is_checked) {
	if(is_checked) {
		ui->structure_view->setScene(&(ui->structure_view->scenes[domain::XY]));
		ui->structure_view->centerOn(ui->structure_view->scenes[domain::XY].polygons->boundingRect().center());
	}
}

//******************************************************************************
void MainWindow::on_rb_plane_yz_toggled(bool const is_checked) {
	if(is_checked) {
		ui->structure_view->setScene(&(ui->structure_view->scenes[domain::YZ]));
		ui->structure_view->centerOn(ui->structure_view->scenes[domain::YZ].polygons->boundingRect().center());
	}
}

//******************************************************************************
void MainWindow::on_rb_plane_zx_toggled(bool const is_checked) {
	if(is_checked) {
		ui->structure_view->setScene(&(ui->structure_view->scenes[domain::ZX]));
		ui->structure_view->centerOn(ui->structure_view->scenes[domain::ZX].polygons->boundingRect().center());
	}
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
	ui->s_structure_rotation->setValue((ui->s_structure_rotation->minimum() + ui->s_structure_rotation->maximum()) / 2);

	// TODO zoom slider must be updated too
	// fitInView operates on the transform matrix
	// https://code.qt.io/cgit/qt/qtbase.git/tree/src/widgets/graphicsview/qgraphicsview.cpp?h=6.8#n2014

	// TODO Fit twice because of possible scrollbars apparition after first fit
	ui->structure_view->fitInView(dynamic_cast<StructureScene*>(ui->structure_view->scene())->polygons->boundingRect() + QMarginsF(5, 5, 5, 5), Qt::KeepAspectRatio);
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
}

//******************************************************************************
void MainWindow::on_tb_show_horizontal_mesh_clicked() {
	ui->structure_view->set_mesh_visibility(StructureScene::MeshVisibility::HORIZONTAL);
}

//******************************************************************************
void MainWindow::on_tb_show_vertical_mesh_clicked() {
	ui->structure_view->set_mesh_visibility(StructureScene::MeshVisibility::VERTICAL);
}

//******************************************************************************
void MainWindow::on_tb_show_no_mesh_clicked() {
	ui->structure_view->set_mesh_visibility(StructureScene::MeshVisibility::NONE);
}

} // namespace ui::qt
