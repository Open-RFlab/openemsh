///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "utils/logger.hpp"

class QStatusBar;
class QWidget;

namespace ui::qt {

//******************************************************************************
class LoggerSink final : public Logger::ISink {
private:
	QStatusBar* status_bar;
	QWidget* parent;
public:
	LoggerSink(QStatusBar* status_bar, QWidget* parent);
	Logger::UserAction log(Logger::LogEvent const& log) const override;
};

} // namespace ui::qt
