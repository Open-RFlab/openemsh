///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "utils/logger.hpp"

namespace ui::cli {

//******************************************************************************
class LoggerSink final : public Logger::ISink {
public:
	explicit LoggerSink(bool verbose);
	Logger::UserAction log(Logger::LogEvent const& log) const override;

private:
	bool verbose;
};


} // namespace ui::cli
