///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <cstdlib>

#include "app/openemsh.hpp"
#include "ui/cli.hpp"

//******************************************************************************
int main(int argc, char* argv[]) {
	app::OpenEMSH oemsh(ui::cli(argc, argv));

	if(!oemsh.get_params().gui) {
		oemsh.parse();
		oemsh.do_all_step();
		oemsh.write();
	}

	return EXIT_SUCCESS;
}
