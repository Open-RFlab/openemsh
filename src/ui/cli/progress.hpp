///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "utils/progress.hpp"

namespace indicators {
class BlockProgressBar;
} // namespace indicators

namespace ui::cli {

//******************************************************************************
class ProgressBar final : public Progress::IBar {
public:
	ProgressBar(std::size_t max, std::string const& prefix);
	~ProgressBar() override;

	void tick(std::size_t i) override;
	void tick(std::size_t i, std::size_t j) override;
	void complete() override;

private:
	std::unique_ptr<indicators::BlockProgressBar> bar;
	std::size_t prefix_size;
	std::string max;
};

} // namespace ui::cli
