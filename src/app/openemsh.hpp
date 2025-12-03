///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <expected>
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

		std::function<void (domain::Params&)> override_from_cli;
	};

	explicit OpenEMSH(Params params);

	Params const& get_params() const;
	domain::Board const& get_board() const;

	void set_input(std::filesystem::path const& path);
	void set_output(std::filesystem::path const& path);
	void set_output_format(Params::OutputFormat format);

	// TODO implement validation checks on params here.
//	void check_x();

	std::expected<void, std::string> parse();
	void run(std::set<Step> const& steps) const;
	void run_all_steps() const;
	void run_next_step() const;
	void run_from_step(Step step) const;
	void go_before(Step step) const;
	void go_before_previous_step() const;
	bool is_about_overwriting() const;
	std::expected<void, std::string> write() const;
	bool can_run_a_next_step() const;
	bool can_go_before() const;
	std::optional<Step> get_current_step() const;
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

//******************************************************************************
std::set<Step> that_and_after(Step step);

} // namespace app
