///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "app/openemsh.hpp"

namespace ui::cli {

//******************************************************************************
app::OpenEMSH::Params cli(int argc, char* argv[]) noexcept(false);

} // namespace ui::cli
