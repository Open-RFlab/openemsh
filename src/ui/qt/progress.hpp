///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QString>

#include <cstdint>

#include "utils/progress.hpp"

class QLabel;
class QProgressBar;
class QStatusBar;

namespace ui::qt {

//******************************************************************************
class ProgressBar final : public Progress::IBar {
public:
	ProgressBar(QStatusBar* status_bar, std::size_t max, QString const& message);
	~ProgressBar() override;

	void tick(std::size_t i) override;
	void tick(std::size_t i, std::size_t j) override;
	void complete() override;

private:
	QStatusBar* status_bar;
	QProgressBar* bar;
	QLabel* label;
};

} // namespace ui::qt
