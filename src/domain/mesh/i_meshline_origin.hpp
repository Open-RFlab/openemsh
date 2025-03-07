///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

namespace domain {

class MeshlinePolicy;

//******************************************************************************
class IMeshLineOrigin {
public:
	MeshlinePolicy* meshline_policy;

	IMeshLineOrigin();
	virtual ~IMeshLineOrigin() = default;
};

} // namespace domain
