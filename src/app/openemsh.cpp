///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <fstream>
#include <iostream>

#include "infra/serializers/serializer_to_plantuml.hpp"
#include "infra/serializers/serializer_to_prettyprint.hpp"
#include "utils/concepts.hpp"
#include "utils/unreachable.hpp"

#include "openemsh.hpp"

namespace app {

using namespace std;

//******************************************************************************
optional<Step> next(Step step) {
	switch(step) {
	case Step::DETECT_CONFLICT_EIP:
		return Step::DETECT_CONFLICT_CE;
	case Step::DETECT_CONFLICT_CE:
		return Step::DETECT_NON_CONFLICTING_EDGES;
	case Step::DETECT_NON_CONFLICTING_EDGES:
		return Step::ADD_FIXED_MLP;
	case Step::ADD_FIXED_MLP:
		return Step::SOLVE_ALL_EIP;
	case Step::SOLVE_ALL_EIP:
		return Step::SOLVE_ALL_CE;
	case Step::SOLVE_ALL_CE:
		return Step::DETECT_AND_SOLVE_TCMLP;
	case Step::DETECT_AND_SOLVE_TCMLP:
		return Step::DETECT_INTERVALS;
	case Step::DETECT_INTERVALS:
		return Step::MESH;
	case Step::MESH:
		return nullopt;
	default:
		unreachable();
	}
}

//******************************************************************************
set<Step> that_and_after(Step step) {
	set<Step> out;
	switch(step) {
	case Step::DETECT_CONFLICT_EIP:
		out.emplace(Step::DETECT_CONFLICT_EIP);
		[[fallthrough]];
	case Step::DETECT_CONFLICT_CE:
		out.emplace(Step::DETECT_CONFLICT_CE);
		[[fallthrough]];
	case Step::DETECT_NON_CONFLICTING_EDGES:
		out.emplace(Step::DETECT_NON_CONFLICTING_EDGES);
		[[fallthrough]];
	case Step::ADD_FIXED_MLP:
		out.emplace(Step::ADD_FIXED_MLP);
		[[fallthrough]];
	case Step::SOLVE_ALL_EIP:
		out.emplace(Step::SOLVE_ALL_EIP);
		[[fallthrough]];
	case Step::SOLVE_ALL_CE:
		out.emplace(Step::SOLVE_ALL_CE);
		[[fallthrough]];
	case Step::DETECT_AND_SOLVE_TCMLP:
		out.emplace(Step::DETECT_AND_SOLVE_TCMLP);
		[[fallthrough]];
	case Step::DETECT_INTERVALS:
		out.emplace(Step::DETECT_INTERVALS);
		[[fallthrough]];
	case Step::MESH:
		out.emplace(Step::MESH);
		break;
	default:
		unreachable();
	}
	return out;
}

//******************************************************************************
OpenEMSH::OpenEMSH(Params params)
: params(std::move(params))
{}

//******************************************************************************
OpenEMSH::Params const& OpenEMSH::get_params() const {
	return params;
}

//******************************************************************************
domain::Board const& OpenEMSH::get_board() const {
	return *board;
}

//******************************************************************************
void OpenEMSH::set_input(std::filesystem::path const& path) {
	params.input = path;
}

//******************************************************************************
void OpenEMSH::set_output(std::filesystem::path const& path) {
	params.output = path;
}

//******************************************************************************
void OpenEMSH::set_output_format(Params::OutputFormat format) {
	params.output_format = format;
}

//******************************************************************************
void OpenEMSH::parse() {
	Caretaker::singleton().reset();
	board = ParserFromCsx::run(params.input, static_cast<ParserFromCsx::Params const&>(params), params.override_from_cli);
	Caretaker::singleton().remember_current_timepoint();
}

//******************************************************************************
void OpenEMSH::write() const {
	switch(params.output_format) {
	case Params::OutputFormat::CSX:
		SerializerToCsx::run(*board, params.input, params.output, static_cast<SerializerToCsx::Params const&>(params));
		break;
	case Params::OutputFormat::PLANTUML: {
//		SerializerToPlantuml::run(*board, static_cast<SerializerToPlantuml::Params const&>(params));
		ofstream out(params.output);
		out << SerializerToPlantuml::run(*board);
	} break;
	case Params::OutputFormat::PRETTYPRINT:
		cerr << SerializerToPrettyprint::run(*board);
		break;
	default:
		unreachable();
	};
}

//******************************************************************************
void OpenEMSH::run(std::set<Step> const& steps) const {
	auto const handle = [&]<InvocableR<void> F>(Step step, F const& func) {
		if(steps.contains(step)) {
			Caretaker::singleton().annotate_current_timepoint(make_unique<Annotation>(step));
			func();
		}
	};

	using enum Step;
	handle(DETECT_CONFLICT_EIP,          [&] { board->detect_edges_in_polygons(); });
	handle(DETECT_CONFLICT_CE,           [&] { board->detect_colinear_edges(); });
	handle(DETECT_NON_CONFLICTING_EDGES, [&] { board->detect_non_conflicting_edges(); });
	handle(ADD_FIXED_MLP,                [&] { board->add_fixed_meshline_policies(); });
	handle(SOLVE_ALL_EIP,                [&] { board->auto_solve_all_edge_in_polygon(); });
	handle(SOLVE_ALL_CE,                 [&] { board->auto_solve_all_colinear_edges(); });
	handle(DETECT_AND_SOLVE_TCMLP,       [&] { board->detect_and_solve_too_close_meshline_policies(); });
	handle(DETECT_INTERVALS,             [&] { board->detect_intervals(); });
	handle(MESH,                         [&] { board->mesh(); });

	Caretaker::singleton().remember_current_timepoint();
}

//******************************************************************************
void OpenEMSH::run_all_steps() const {
	run({
		Step::DETECT_CONFLICT_EIP,
		Step::DETECT_CONFLICT_CE,
		Step::DETECT_NON_CONFLICTING_EDGES,
		Step::ADD_FIXED_MLP,
		Step::SOLVE_ALL_EIP,
		Step::SOLVE_ALL_CE,
		Step::DETECT_AND_SOLVE_TCMLP,
		Step::DETECT_INTERVALS,
		Step::MESH
	});
}

//******************************************************************************
void OpenEMSH::run_next_step() const {
	auto& c = Caretaker::singleton();
	auto* t = c.find_first_ancestor_with_annotation();
	if(auto const* a = c.get_annotation(t); a) {
		if(optional<Step> step = next(static_cast<Annotation const*>(a)->before_step)
		; step)
			run({ step.value() });
	} else {
		run({ Step::DETECT_CONFLICT_EIP });
	}
}

//******************************************************************************
void OpenEMSH::run_from_step(Step step) const {
	run(that_and_after(step));
}

//******************************************************************************
void OpenEMSH::go_before(Step step) const {
	auto& c = Caretaker::singleton();
	c.go_without_remembering(
		c.find_first_ancestor_with_annotation_that(
			[&step](IAnnotation const* annotation) {
				return static_cast<Annotation const*>(annotation)->before_step == step;
			}));
}

//******************************************************************************
void OpenEMSH::go_before_previous_step() const {
	auto& c = Caretaker::singleton();
	c.go_and_remember(c.find_first_ancestor_with_annotation());
}

//******************************************************************************
bool OpenEMSH::can_run_a_next_step() const {
	auto& c = Caretaker::singleton();
	auto* t = c.find_first_ancestor_with_annotation();
	if(auto const* a = c.get_annotation(t); a)
		return next(static_cast<Annotation const*>(a)->before_step).has_value();
	else
		return true;
}

//******************************************************************************
bool OpenEMSH::can_go_before() const {
	return Caretaker::singleton().find_first_ancestor_with_annotation();
}

//******************************************************************************
Annotation::Annotation(Step before_step)
: before_step(before_step)
{}

} // namespace app
