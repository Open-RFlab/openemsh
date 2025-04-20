///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

namespace domain {

class MeshlinePolicy;

//******************************************************************************
struct IMeshLineOriginState {
	MeshlinePolicy* meshline_policy = nullptr;
};

//******************************************************************************
class IMeshLineOrigin {
public:
	virtual ~IMeshLineOrigin() = default;
};

} // namespace domain
