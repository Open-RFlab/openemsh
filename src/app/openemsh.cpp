///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <fstream>
#include <iostream>

#include "infra/serializers/serializer_to_plantuml.hpp"
#include "infra/serializers/serializer_to_prettyprint.hpp"
#include "utils/unreachable.hpp"

#include "openemsh.hpp"

namespace app {

using namespace std;

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
void OpenEMSH::parse() {
	board = ParserFromCsx::run(params.input, dynamic_cast<ParserFromCsx::Params const&>(params));
	board->params = dynamic_cast<domain::Params const&>(params);
}

//******************************************************************************
void OpenEMSH::do_all_step() {
	board->auto_mesh();
}

//******************************************************************************
void OpenEMSH::do_x_step() {
}

//******************************************************************************
void OpenEMSH::write() const {
	switch(params.output_format) {
	case Params::OutputFormat::CSX:
		SerializerToCsx::run(*board, params.input, params.output, dynamic_cast<SerializerToCsx::Params const&>(params));
		break;
	case Params::OutputFormat::PLANTUML: {
//		SerializerToPlantuml::run(*board, dynamic_cast<SerializerToPlantuml::Params const&>(params));
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

} // namespace app
