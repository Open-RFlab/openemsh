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

private slots:
	void on_a_about_triggered();
	void on_rb_plane_xy_toggled(bool const is_checked);
	void on_rb_plane_yz_toggled(bool const is_checked);
	void on_rb_plane_zx_toggled(bool const is_checked);
	void on_s_structure_rotation_valueChanged(int const value);
	void on_s_structure_zoom_valueChanged(int const value);
	void on_tb_anchor_clicked(bool const is_checked);
	void on_tb_reset_clicked();
	void on_tb_horizontal_layout_clicked();
	void on_tb_vertical_layout_clicked();
	void on_tb_show_all_mesh_clicked();
	void on_tb_show_horizontal_mesh_clicked();
	void on_tb_show_vertical_mesh_clicked();
	void on_tb_show_no_mesh_clicked();

public:
	MainWindow(app::OpenEMSH& oemsh, QWidget* parent = nullptr);
	~MainWindow();

	void update_structure();
};

} // namespace ui::qt
