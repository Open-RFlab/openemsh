///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <cstddef>

//******************************************************************************
inline std::size_t generate_id() {
	static std::size_t i = 0;
	return i++;
}

//******************************************************************************
class IdGenerator {
private:
	std::size_t i = 0;

public:
	std::size_t generate_id() {
		return i++;
	}

	std::size_t operator()() {
		return generate_id();
	}
};
