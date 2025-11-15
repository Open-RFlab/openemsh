///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <indicators/block_progress_bar.hpp>

#include "progress.hpp"

using namespace std;

namespace ui::cli {

//******************************************************************************
ProgressBar::ProgressBar(size_t max, string const& prefix)
: bar(make_unique<indicators::BlockProgressBar>(
//	indicators::option::BarWidth { 100 },
	indicators::option::Start { "[" },
	indicators::option::End { "]" },
	indicators::option::ForegroundColor { indicators::Color::green },
	indicators::option::ShowPercentage { true },
	indicators::option::ShowElapsedTime { true },
	indicators::option::PrefixText { prefix },
	indicators::option::MaxProgress { max }
))
, max(to_string(max))
, prefix(prefix)
{}

//******************************************************************************
ProgressBar::~ProgressBar() = default;

//******************************************************************************
void ProgressBar::tick(size_t i) {
	string postfix(to_string(++i) + "/" + max);
	resize_to_term(postfix);
	bar->set_option(indicators::option::PostfixText { postfix });
	bar->set_progress(i);
}

//******************************************************************************
void ProgressBar::tick(size_t i, size_t j) {
	string postfix(to_string(i) + "/" + to_string(j) + "/" + max);
	resize_to_term(postfix);
	bar->set_option(indicators::option::PostfixText { postfix });
	bar->set_progress(j);
}

//******************************************************************************
void ProgressBar::resize_to_term(string const& postfix) {
	// TODO This does not take into account percentage nor time
	// " 100% [000m:00s] " 17 chars
	bar->set_option(indicators::option::BarWidth { 100 - prefix.size() - postfix.size() - 19 });
}

//******************************************************************************
void ProgressBar::complete() {
	bar->mark_as_completed();
}

} // namespace ui::cli
