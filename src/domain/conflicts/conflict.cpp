///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "conflict.hpp"

namespace domain {

//******************************************************************************
Conflict::Conflict(Kind const kind)
: IMeshLineOrigin()
, kind(kind)
{}

} // namespace domain
