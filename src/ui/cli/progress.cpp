///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <iostream>

#include <indicators/block_progress_bar.hpp>

#include "progress.hpp"

using namespace std;

namespace ui::cli {

//******************************************************************************
size_t bar_width(size_t prefix_size, size_t postfix_size) {
	// indicators::option::BarWidth does not take into account percentage nor time
	// " 100% [000m:00s] " 17 chars
	return 100 - prefix_size - postfix_size - 19;
}

//******************************************************************************
ProgressBar::ProgressBar(size_t max, string const& prefix)
: bar(make_unique<indicators::BlockProgressBar>(
	// 2 padding for no suffix & no 'm' in elapsed time yet
	// 2 padding for '100%' vs '0%' regarding whether max is 0
	indicators::option::BarWidth { bar_width(prefix.size(), 0) + (max ? 4 : 2) },
	indicators::option::Start { "[" },
	indicators::option::End { "]" },
	indicators::option::ForegroundColor { indicators::Color::green },
	indicators::option::ShowPercentage { true },
	indicators::option::ShowElapsedTime { true },
	indicators::option::PrefixText { prefix },
	indicators::option::MaxProgress { max },
	indicators::option::Stream { cerr }
))
, prefix_size(prefix.size())
, max(to_string(max))
{}

//******************************************************************************
ProgressBar::~ProgressBar() = default;

//******************************************************************************
void ProgressBar::tick(size_t i) {
	string postfix(to_string(++i) + "/" + max);
	bar->set_option(indicators::option::BarWidth { bar_width(prefix_size, postfix.size()) });
	bar->set_option(indicators::option::PostfixText { postfix });
	bar->set_progress(i);
}

//******************************************************************************
void ProgressBar::tick(size_t i, size_t j) {
	string postfix(to_string(i) + "/" + to_string(j) + "/" + max);
	bar->set_option(indicators::option::BarWidth { bar_width(prefix_size, postfix.size()) });
	bar->set_option(indicators::option::PostfixText { postfix });
	bar->set_progress(j);
}

//******************************************************************************
void ProgressBar::complete() {
	bar->mark_as_completed();
}

} // namespace ui::cli
