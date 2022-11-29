///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <filesystem>
#include <string>

#include "utils/entity.hpp"

//******************************************************************************
class SerializerToCsx final : public EntityVisitor {
public:
	struct Params {
//		bool does_overwrite = false;
//		bool does_append = false;
		bool with_meshlines = true;
		bool with_meshline_policies = false;
		bool with_axis_x = true;
		bool with_axis_y = true;
		bool with_axis_z = true;
	};

	static void run(
		Board& board,
		std::filesystem::path const& input,
		std::filesystem::path const& output);
	static void run(
		Board& board,
		std::filesystem::path const& input,
		std::filesystem::path const& output,
		Params params);

private:
	friend class Board;

	void visit(Board& board) override;

	SerializerToCsx(
		std::filesystem::path const& input,
		std::filesystem::path const& output,
		Params params);

	Params const params;
	std::filesystem::path const input;
	std::filesystem::path const output;
};
