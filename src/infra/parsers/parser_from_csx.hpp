///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

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
		std::vector<std::string> grounds;
	};

	~ParserFromCsx();

	static std::unique_ptr<domain::Board> run(std::filesystem::path const& input);
	static std::unique_ptr<domain::Board> run(std::filesystem::path const& input, Params params);
	static std::unique_ptr<domain::Board> run(std::filesystem::path const& input, std::vector<std::string> ground_names, Params params);

private:
	ParserFromCsx(std::filesystem::path const& input);
	ParserFromCsx(std::filesystem::path const& input, Params params);

	void parse();
	std::unique_ptr<domain::Board> output();

	std::filesystem::path const input;
	Params const parser_params;
	domain::Params params;

//	std::vector<std::unique_ptr<Polygon>> polygons;
	class Pimpl;
	std::unique_ptr<Pimpl> pimpl;

};
