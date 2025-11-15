///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include "progress.hpp"

using namespace std;

//******************************************************************************
Progress::Bar::Bar(size_t max, string const& prefix) {
	for(auto& builder : Progress::singleton().impl_builders)
		impls.push_back(builder(max, prefix));
}

//******************************************************************************
Progress::Bar::~Bar() = default;

//******************************************************************************
void Progress::Bar::tick(size_t i) {
	for(auto& impl : impls)
		impl->tick(i);
}

//******************************************************************************
void Progress::Bar::tick(size_t i, size_t j) {
	for(auto& impl : impls)
		impl->tick(i, j);
}

//******************************************************************************
void Progress::Bar::complete() {
	for(auto& impl : impls)
		impl->complete();
}

//******************************************************************************
Progress& Progress::singleton() {
	static Progress p;
	return p;
}

//******************************************************************************
void Progress::register_impl_builder(function<unique_ptr<IBar> (size_t, string const&)> const& builder) {
	impl_builders.push_back(builder);
}
