///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <expected>
#include <filesystem>
#include <optional>
#include <string>

#include "domain/utils/entity_visitor.hpp"

//******************************************************************************
class SerializerToCsx final : public domain::EntityVisitor {
public:
	struct Params {
//		bool does_overwrite = false;
//		bool does_append = false;
		bool with_meshlines = true;
		bool with_meshline_policies = false;
		bool with_axis_x = true;
		bool with_axis_y = true;
		bool with_axis_z = true;
		bool with_oemsh_params = false;
	};

	static std::expected<void, std::string> run(
		domain::Board& board,
		std::filesystem::path const& input,
		std::filesystem::path const& output);
	static std::expected<void, std::string> run(
		domain::Board& board,
		std::filesystem::path const& input,
		std::filesystem::path const& output,
		Params params);

private:
	friend class domain::Board;

	void visit(domain::Board& board) override;

	SerializerToCsx(
		std::filesystem::path const& input,
		std::filesystem::path const& output,
		Params params);

	Params const params;
	std::filesystem::path const input;
	std::filesystem::path const output;
	std::optional<std::string> error;
};
