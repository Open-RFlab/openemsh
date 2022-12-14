///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>
#include <initializer_list>
#include <string>

#include <CLI/CLI.hpp>
//#include <CLI/App.hpp>
//#include <CLI/Config.hpp>
//#include <CLI/Formatter.hpp>

#include "domain/global.hpp"

#include "cli.hpp"

// TODO CLI --board
// TODO CLI --structure -zx -yz -xy

namespace ui {

using namespace std;

string to_string(bool a) {
	return a ? "true" : "false";
}

//******************************************************************************
//class CliFormatter : public CLI::Formatter {
//public:
//	CliFormatter() : Formatter() {}
//	string make_option_opts(CLI::Option const*) const override { return " OPTION"; }
//	string make_option_name(CLI::Option const*, bool) const override {};
//};

//******************************************************************************
optional<CliParams> cli(int const argc, char* argv[]) {
	CliParams params;

//	auto fmt = make_shared<CliFormatter>();
	CLI::App app("OpenEMSH mesher", "OpenEMSH");
//	app.formatter(fmt);

	app.set_help_flag("-h,--help", "Display help and exit.");
	app.set_version_flag("--version", "0.0.0", "Display version and exit.");
//	app.add_option("-i,--input", , "Input CSX file.");

	app.add_option("--ground", params.grounds, "Declare properties to be ground planes ('Name' XML field).")->group("Input options");
	app.add_flag("--no-yz", [&params](size_t) { params.with_yz = false; }, "Don't process YZ plane.")->group("Input options");
	app.add_flag("--no-zx", [&params](size_t) { params.with_zx = false; }, "Don't process ZX plane.")->group("Input options");
	app.add_flag("--no-xy", [&params](size_t) { params.with_xy = false; }, "Don't process XY plane.")->group("Input options");

	// TODO processing options
	// --step-detect-edges-in-polygons
	// --step-detect-colinear-edges
	// --step-detect-non-conflicting-edges
	// --step-auto-solve-all-edge-in-polygon
	// --step-auto-solve-all-colinear-edges
	// --step-detect-and-solve-too-close-meshline-policies
	// --step-detect-intervals
	// --step-mesh

	app.add_flag("--no-x", [&params](size_t) { params.with_axis_x = false; }, "Don't include X axis meshlines in output.")->group("Output options");
	app.add_flag("--no-y", [&params](size_t) { params.with_axis_y = false; }, "Don't include Y axis meshlines in output.")->group("Output options");
	app.add_flag("--no-z", [&params](size_t) { params.with_axis_z = false; }, "Don't include Z axis meshlines in output.")->group("Output options");
	app.add_flag("--meshlines", params.with_meshlines, "Include regular meshlines in output.")->group("Output options")->default_str(to_string(params.with_meshlines));
	app.add_flag("--policy-lines", params.with_meshline_policies, "Include meshline policies in output.")->group("Output options")->default_str(to_string(params.with_meshline_policies));

//	CLI11_PARSE(app, argc, argv);
	try {
		app.parse(argc, argv);
	} catch(const CLI::ParseError &e) {
		app.exit(e);
		return nullopt;
	}

	return params;
}

} // namespace ui
