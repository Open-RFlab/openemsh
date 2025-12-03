///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QApplication>

#include <cstdlib>
#include <iostream>
#include <format>

#include "app/openemsh.hpp"
#include "ui/cli/cli.hpp"
#include "ui/cli/progress.hpp"
#include "ui/qt/main_window.hpp"

//******************************************************************************
int main(int argc, char* argv[]) {
	app::OpenEMSH oemsh(ui::cli::cli(argc, argv));

	if(oemsh.get_params().verbose)
		Progress::singleton().register_impl_builder(
			[&oemsh](std::size_t max, std::string const& message) {
				return std::make_unique<ui::cli::ProgressBar>(max,
					std::format("{}/{} {}",
						app::index(oemsh.get_current_step()),
						app::index_max(),
						message));
			});

	if(!oemsh.get_params().gui) {
		if(auto res = oemsh.parse(); !res) {
			std::cerr
				<< std::format(
					"Error parsing file \"{}\" : {}",
					oemsh.get_params().input.generic_string(),
					res.error())
				<< std::endl;
			return EXIT_FAILURE;
		}
		if(oemsh.is_about_overwriting()) {
			std::cerr
				<< std::format(
					"Error: You are about overwriting the file \"{}\", "
					"use --force if that is what you want.",
					oemsh.get_params().output.generic_string())
				<< std::endl;
			return EXIT_FAILURE;
		}
		oemsh.run_all_steps();
		if(auto res = oemsh.write(); !res) {
			std::cerr
				<< std::format(
					"Error saving file \"{}\" : {}",
					oemsh.get_params().output.generic_string(),
					res.error())
				<< std::endl;
			return EXIT_FAILURE;
		}
	} else {
		QApplication a(argc, argv);

#ifdef OEMSH_PORTABILITY_TWEAKS
		a.setAttribute(Qt::AA_DontUseNativeDialogs);
#endif // OEMSH_PORTABILITY_TWEAKS

		// Avoid a stold() bug introduced by QApplication() performing setlocale(LC_ALL, "")
		setlocale(LC_NUMERIC, "C");
		ui::qt::MainWindow w(oemsh);
		w.show();
		w.parse_and_display();
		return QApplication::exec();
	}

	return EXIT_SUCCESS;
}
