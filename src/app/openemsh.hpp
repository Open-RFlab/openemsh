///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <set>

#include "domain/board.hpp"
#include "domain/global.hpp"
#include "infra/parsers/parser_from_csx.hpp"
#include "infra/serializers/serializer_to_csx.hpp"
#include "utils/state_management.hpp"
#include "steps.hpp"

namespace app {

//******************************************************************************
class OpenEMSH {
public:
	struct Params final
	: public ParserFromCsx::Params
	, public SerializerToCsx::Params
	{
		std::filesystem::path input;
		std::filesystem::path output;
		bool force = false;
		bool verbose = false;
		bool gui = false;

		enum class OutputFormat {
			CSX,
			PLANTUML,
			PRETTYPRINT
		} output_format = OutputFormat::CSX;

		bool with_step_detect_edges_in_polygons = true;
		bool with_step_detect_colinear_edges = true;
		bool with_step_detect_non_conflicting_edges = true;
		bool with_step_auto_solve_all_edge_in_polygon = true;
		bool with_step_auto_solve_all_colinear_edges = true;
		bool with_step_detect_and_solve_too_close_meshline_policies = true;
		bool with_step_detect_intervals = true;
		bool with_step_mesh = true;

		std::function<void (domain::Params&)> override_from_cli;
	};

	explicit OpenEMSH(Params params);

	Params const& get_params() const;
	domain::Board const& get_board() const;

	// TODO implement validation checks on params here.
//	void check_x();

	void parse();
	void run(std::set<Step> const& steps) const;
	void run_all_steps() const;
	void run_next_step() const;
	void go_before(Step step) const;
	void go_before_previous_step() const;
	void write() const;

	bool can_run_a_next_step() const;
	bool can_go_before() const;
private:
	Params params;
	std::shared_ptr<domain::Board> board;
};

//******************************************************************************
class Annotation : public IAnnotation {
public:
	Step const before_step;
	explicit Annotation(Step before_step);
};

//******************************************************************************
std::optional<Step> next(Step step);

} // namespace app
