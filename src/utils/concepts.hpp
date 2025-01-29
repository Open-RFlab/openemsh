///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <type_traits>

//******************************************************************************
template<typename T> concept Enum = std::is_enum_v<T>;
