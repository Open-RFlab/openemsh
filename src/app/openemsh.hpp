///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <filesystem>
#include <memory>

#include "domain/board.hpp"
#include "domain/global.hpp"
#include "infra/parsers/parser_from_csx.hpp"
#include "infra/serializers/serializer_to_csx.hpp"

namespace app {

//******************************************************************************
class OpenEMSH {
public:
	struct Params final
	: public domain::Params
	, public ParserFromCsx::Params
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
	};

	explicit OpenEMSH(Params params);

	Params const& get_params() const;
	domain::Board const& get_board() const;

	// TODO implement validation checks on params here.
//	void check_x();

	void parse();
	void do_all_step();
	void do_x_step();
	void write() const;

private:
	Params params;
	std::shared_ptr<domain::Board> board;
};

} // namespace app
