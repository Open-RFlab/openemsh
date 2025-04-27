///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QApplication>

#include <cstdlib>

#include "app/openemsh.hpp"
#include "ui/cli.hpp"
#include "ui/qt/main_window.hpp"

//******************************************************************************
int main(int argc, char* argv[]) {
	app::OpenEMSH oemsh(ui::cli(argc, argv));

	if(!oemsh.get_params().gui) {
		oemsh.parse();
		oemsh.run_all_steps();
		oemsh.write();
	} else {
		QApplication a(argc, argv);
		// Avoid a stold() bug introduced by QApplication() performing setlocale(LC_ALL, "")
		setlocale(LC_NUMERIC, "C");
		ui::qt::MainWindow w(oemsh);
		w.show();
		return QApplication::exec();
	}

	return EXIT_SUCCESS;
}
