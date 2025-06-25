///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <CLI/CLI.hpp>
//#include <CLI/App.hpp>
//#include <CLI/Config.hpp>
//#include <CLI/Formatter.hpp>
//#include <CLI/Validators.hpp>

#include <cstdlib>
#include <map>
#include <string>

#include "utils/concepts.hpp"
#include "utils/unreachable.hpp"

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

// TODO capture_default_str does not work, because of type?

//******************************************************************************
struct BoundExclusiveInclusive : CLI::Validator {
	BoundExclusiveInclusive(double min, double max) {
		name_ = "Exclusive-Inclusive Bound";
		func_ = [min, max](string const& input) {
			using CLI::detail::lexical_cast;
			double val;
			bool converted = lexical_cast(input, val);
			if((!converted) || (val <= min || val > max)) {
				stringstream out;
				out << "Value " << input << " not in range ]"
				    << min << " - " << max << "]";
				return out.str();
			}
			return std::string();
		};
		desc_function_ = [min, max]() {
			stringstream description;
			description << "Bounded to ]" << min << " - " << max << "]";
			return description.str();
		};
	}
};

//******************************************************************************
struct FutureConditional : CLI::Validator {
	FutureConditional(bool const& cond, string const& error_message) {
		name_ = "FutureConditional";
		func_ = [&cond, error_message](string const&) {
			if(cond)
				return std::string();
			else
				return error_message;
		};
	}
};

//******************************************************************************
template<InvocableR<void> F>
struct JustDo : CLI::Validator {
	JustDo(F const& func) {
		name_ = "JustDo";
		func_ = [func](string const&) {
			func();
			return std::string();
		};
	}
};

//******************************************************************************
template<auto Member>
auto make_overrider(auto& overrides_collector) {
	return [&overrides_collector](auto const& value) { // This is CLI::add_option_function callback.
		overrides_collector.emplace_back(
			[value](auto& to_override) { // This is to be executed to actually apply override.
				to_override.*Member = value;
			});
	};
}

//******************************************************************************
app::OpenEMSH::Params cli(int const argc, char* argv[]) {
	app::OpenEMSH::Params params;
	vector<function<void (domain::Params&)>> domain_overrides;

//	auto fmt = make_shared<CliFormatter>();
	CLI::App app("OpenEMSH mesher", argv[0]);
//	app.formatter(fmt);

	app.set_help_flag("-h,--help", "Display help and exit.");
	app.set_version_flag("--version", OEMSH_VERSION, "Display version and exit.");
	app.add_flag("-v,--verbose", params.verbose, "Verbose mode.")->capture_default_str();
	auto* g = app.add_flag("-G", params.gui, "GUI mode.");
	auto* i = app.add_option("-i,--input", params.input, "Input CSX file.")->check(CLI::ExistingFile)->required();
	g->trigger_on_parse()->check(JustDo([i]() { i->required(false); }));
//	app.add_option("-o,--output", params.output, "Output CSX file. If different from input, will copy and extend it.")->check((!CLI::ExistingFile)|FutureConditional(params.force,"Cannot overwrite a file without --force"));
	app.add_option("-o,--output", params.output, "Output CSX file. If different from input, will copy and extend it.")->check(CLI::Validator((!CLI::ExistingFile)|FutureConditional(params.force,"Cannot overwrite a file without --force"), "FILE", "KO"));
	app.add_flag("-f,--force", params.force, "Allow overwriting a file.")->trigger_on_parse();

	static std::map<std::string, app::OpenEMSH::Params::OutputFormat> const map {
		{ "csx", app::OpenEMSH::Params::OutputFormat::CSX },
		{ "plantuml", app::OpenEMSH::Params::OutputFormat::PLANTUML },
		{ "prettyprint", app::OpenEMSH::Params::OutputFormat::PRETTYPRINT }
	};
	app.add_option("--output-format", params.output_format, "Output format.")->transform(CLI::CheckedTransformer(map, CLI::ignore_case));

	app.add_option("--ground", params.grounds, "Declare properties to be ground planes ('Name' XML field).")->group("Input options");
	app.add_flag("--no-yz", [&params](size_t) { params.with_yz = false; }, "Don't process YZ plane.")->group("Input options");
	app.add_flag("--no-zx", [&params](size_t) { params.with_zx = false; }, "Don't process ZX plane.")->group("Input options");
	app.add_flag("--no-xy", [&params](size_t) { params.with_xy = false; }, "Don't process XY plane.")->group("Input options");

	// TODO processing options
//	app.add_flag("--step-detect-edges-in-polygons", params.with_step_detect_edges_in_polygons, "Do EdgeInPolygon conflicts detection step.")->group("Processing options");
//	app.add_flag("--step-detect-colinear-edges", params.with_step_detect_colinear_edges, "Do ColinearEdges conflicts detection step.")->group("Processing options");
//	app.add_flag("--step-detect-non-conflicting-edges", params.with_step_detect_non_conflicting_edges, "Do non-conflicting edges detection step.")->group("Processing options");
//	app.add_flag("--step-auto-solve-all-edge-in-polygon", params.with_step_auto_solve_all_edge_in_polygon, "Do EdgeInPolygon conflicts autosolving step.")->group("Processing options");
//	app.add_flag("--step-auto-solve-all-colinear-edges", params.with_step_auto_solve_all_colinear_edges, "Do ColinearEdges conflicts autosolving step.")->group("Processing options");
//	app.add_flag("--step-detect-and-solve-too-close-meshline-policies", params.with_step_detect_and_solve_too_close_meshline_policies, "Do TooCloseMeshlinePolicies conflicts detection and autosolving step.")->group("Processing options");
//	app.add_flag("--step-detect-intervals", params.with_step_detect_intervals, "Do intervals detection step.")->group("Processing options");
//	app.add_flag("--step-mesh", params.with_step_mesh, "Do intervals meshing step.")->group("Processing options");

	// Mesher options
	app.add_option_function<decltype(domain::Params::metal_res)>("--metal_res",
		make_overrider<&domain::Params::metal_res>(domain_overrides),
		"Desired mesh resolution for metal regions."
	)->group("Mesher options");

//	app.add_option_function<decltype(domain::Params::air_res)>("--air_res",
//		make_overrider<&domain::Params::air_res>(domain_overrides),
//		"Desired mesh resolution for air regions."
//	)->group("Mesher options");

	app.add_option_function<decltype(domain::Params::substrate_res)>("--substrate_res",
		make_overrider<&domain::Params::substrate_res>(domain_overrides),
		"Desired mesh resolution for substrate / ground plane regions."
	)->group("Mesher options");

	app.add_option_function<decltype(domain::Params::proximity_limit)>("--proximity_limit",
		make_overrider<&domain::Params::proximity_limit>(domain_overrides),
		"Distance under which two adjacent lines trigger a conflict."
	)->group("Mesher options");

	app.add_option_function<decltype(domain::Params::dmax)>("--dmax",
		make_overrider<&domain::Params::dmax>(domain_overrides),
		"Maximum distance between two adjacent lines."
	)->group("Mesher options"); // TODO is in fact mres / sres / ares

	app.add_option_function<decltype(domain::Params::lmin)>("--lmin",
		make_overrider<&domain::Params::lmin>(domain_overrides),
		"Minimum line number per interval half."
	)->group("Mesher options");

	app.add_option_function<decltype(domain::Params::lambda)>("--lambda",
		make_overrider<&domain::Params::lambda>(domain_overrides),
		"Smoothness factor ]1;2]."
	)->group("Mesher options")->check(BoundExclusiveInclusive(1.0, 2.0));

	app.add_flag("--no-x", [&params](size_t) { params.with_axis_x = false; }, "Don't include X axis meshlines in output.")->group("Output options");
	app.add_flag("--no-y", [&params](size_t) { params.with_axis_y = false; }, "Don't include Y axis meshlines in output.")->group("Output options");
	app.add_flag("--no-z", [&params](size_t) { params.with_axis_z = false; }, "Don't include Z axis meshlines in output.")->group("Output options");
	app.add_flag("--meshlines", params.with_meshlines, "Include regular meshlines in output.")->group("Output options")->default_str(to_string(params.with_meshlines));
	app.add_flag("--policy-lines", params.with_meshline_policies, "Include meshline policies in output.")->group("Output options")->default_str(to_string(params.with_meshline_policies));
//	app.add_flag("--policy-lines", params.with_meshline_policies, "Include meshline policies in output.")->group("Output options")->capture_default_str();

	try {
		app.parse(argc, argv);
	} catch(CLI::Success const& e) {
		app.exit(e);
		exit(EXIT_SUCCESS);
	} catch(CLI::ParseError const& e) {
		app.exit(e);
		exit(EXIT_FAILURE);
	}

	params.override_from_cli = [domain_overrides](domain::Params& to_override) {
		for(auto const& apply : domain_overrides)
			apply(to_override);
	};

	return params;
}

} // namespace ui
