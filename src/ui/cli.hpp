///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <optional>

#include "domain/global.hpp"
#include "infra/parsers/parser_from_csx.hpp"
#include "infra/serializers/serializer_to_csx.hpp"

namespace ui {

//******************************************************************************
struct CliParams final
: public domain::Params
, public ParserFromCsx::Params
, public SerializerToCsx::Params
{};

//******************************************************************************
std::optional<CliParams> cli(int argc, char* argv[]);

} // namespace ui
