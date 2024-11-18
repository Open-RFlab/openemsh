///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <memory>
#include <vector>
#include <iostream>
#include <fstream>

#include "domain/board.hpp"
#include "infra/parsers/parser_from_csx.hpp"
#include "infra/serializers/serializer_to_csx.hpp"
#include "infra/serializers/serializer_to_plantuml.hpp"
#include "infra/serializers/serializer_to_prettyprint.hpp"
#include "ui/cli.hpp"

using namespace domain;
using namespace std;
using namespace ui;

//******************************************************************************
int main(int argc, char* argv[]) {
	optional<CliParams> const params = cli(argc, argv);
	if(!params)
		return 1;

	return 0;
}
