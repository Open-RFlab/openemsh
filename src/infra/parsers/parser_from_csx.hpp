///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <expected>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "domain/geometrics/polygon.hpp"
#include "domain/global.hpp"

namespace domain {

class Board;

} // namespace domain

//******************************************************************************
class ParserFromCsx {
public:
	struct Params {
		bool with_yz = true;
		bool with_zx = true;
		bool with_xy = true;
	};

	~ParserFromCsx();

	[[nodiscard]] static std::expected<std::shared_ptr<domain::Board>, std::string> run(std::filesystem::path const& input);
	[[nodiscard]] static std::expected<std::shared_ptr<domain::Board>, std::string> run(std::filesystem::path const& input, Params params);
	[[nodiscard]] static std::expected<std::shared_ptr<domain::Board>, std::string> run(std::filesystem::path const& input, Params params, std::function<void (domain::Params&)> const& override_domain_params);

private:
	ParserFromCsx(std::filesystem::path const& input);
	ParserFromCsx(std::filesystem::path const& input, Params params);

	std::expected<void, std::string> parse();
	[[nodiscard]] std::shared_ptr<domain::Board> output();

	std::filesystem::path const input;
	Params const parser_params;
	domain::Params domain_params; // Created from defaults + CSX.

//	std::vector<std::unique_ptr<Polygon>> polygons;
	class Pimpl;
	std::unique_ptr<Pimpl> pimpl;
};
