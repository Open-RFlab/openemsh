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
	void on_tb_horizontal_layout_clicked();
	void on_tb_vertical_layout_clicked();

public:
	MainWindow(app::OpenEMSH& oemsh, QWidget* parent = nullptr);
	~MainWindow();
};

} // namespace ui::qt
