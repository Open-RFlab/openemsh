///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <memory>

#include <QMainWindow>
#include <QObject>

#include "app/openemsh.hpp"
#include "style.hpp"

namespace Ui {
class MainWindow;
} // namespace Ui

namespace ui::qt {

//******************************************************************************
class MainWindow : public QMainWindow {
	Q_OBJECT
private:
	std::unique_ptr<Ui::MainWindow> ui;
	app::OpenEMSH& oemsh;

	bool dock_layout_order;

	void set_style(Style const& style);

	void update_navigation_visibility();
	void go_to_current_state();
	void make_current_state_view();
	void go_to_or_make_current_state();
	void edit_global_params();

private slots:
	void on_a_about_triggered();
	void on_a_doc_oems_meshing_triggered() const;
	void on_ag_styles_triggered(QAction* const action);
	void on_rb_plane_xy_toggled(bool const is_checked);
	void on_rb_plane_yz_toggled(bool const is_checked);
	void on_rb_plane_zx_toggled(bool const is_checked);
	void on_tb_anchor_clicked(bool const is_checked);
	void on_a_reset_triggered();
	void on_a_horizontal_layout_triggered();
	void on_a_vertical_layout_triggered();
	void on_tb_show_all_mesh_clicked();
	void on_tb_show_horizontal_mesh_clicked();
	void on_tb_show_vertical_mesh_clicked();
	void on_tb_show_no_mesh_clicked();
	void on_tb_show_selected_clicked();
	void on_tb_show_displayed_clicked();
	void on_tb_show_everything_clicked();
	void on_tb_curved_wires_clicked();
	void on_tb_direct_wires_clicked();
	void on_tb_structure_rotate_cw_clicked();
	void on_tb_structure_rotate_ccw_clicked();
	void on_tb_structure_zoom_in_clicked();
	void on_tb_structure_zoom_out_clicked();
	void on_tb_processing_zoom_in_clicked();
	void on_tb_processing_zoom_out_clicked();
	void on_a_edit_triggered();
	void on_a_mesh_prev_triggered();
	void on_a_mesh_next_triggered();
	void on_a_undo_triggered();
	void on_a_redo_triggered();

	void handle_edition(app::Step const redo_from = app::Step::DETECT_CONFLICT_EIP);

public:
	MainWindow(app::OpenEMSH& oemsh, QWidget* parent = nullptr);
	~MainWindow() override;

	void parse_and_display();
	void clear();

protected:
	void keyPressEvent(QKeyEvent* event) override;
};

} // namespace ui::qt
