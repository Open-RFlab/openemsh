///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "meshline_manager.hpp"

//******************************************************************************
MeshlineManager::MeshlineManager(Params& _params, double const _coord, bool const _is_enabled, double const _res_factor)
: params(_params)
, coord(_coord)
, is_enabled(_is_enabled)
, res_factor(_res_factor)
{}
