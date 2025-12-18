///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <format>
#include <iostream>
#include <print>
#include <ranges>

#include "utils/unreachable.hpp"

#include "logger.hpp"

using namespace std;

namespace ui::cli {

//******************************************************************************
string to_string(Logger::Level level) noexcept {
	switch(level) {
	case Logger::Level::QUESTION: return "Question";
	case Logger::Level::INFO: return "Info";
	case Logger::Level::WARNING: return "Warning";
	case Logger::Level::ERROR: return "Error";
	default: ::unreachable();
	}
}

//******************************************************************************
string to_emoji(Logger::Level level) noexcept {
	switch(level) {
//	case Logger::Level::QUESTION: return "❔";
	case Logger::Level::QUESTION: return "❓";
	case Logger::Level::INFO: return "ℹ️ ";
	case Logger::Level::WARNING: return "⚠️ ";
	case Logger::Level::ERROR: return "⛔";
//	case Logger::Level::QUESTION: return "\u2754";
//	case Logger::Level::QUESTION: return "\u2753";
//	case Logger::Level::INFO: return "\u2139";
//	case Logger::Level::WARNING: return "\u26A0";
//	case Logger::Level::ERROR: return "\u26D4";
	default: ::unreachable();
	}
}

//******************************************************************************
string to_string(Logger::UserAction action) noexcept {
	switch(action) {
	case Logger::UserAction::NOTHING: return "";
	case Logger::UserAction::CANCEL: return "n";
	case Logger::UserAction::OK: return "y";
	case Logger::UserAction::SAVE: return "s";
	case Logger::UserAction::ABORT: return "a";
	default: ::unreachable();
	}
}

//******************************************************************************
string to_string(set<Logger::UserAction> const& actions) noexcept {
	return format("[{}]",
		actions
		| views::transform([](auto const& action) { return to_string(action); })
		| views::join_with("/"s)
		| ranges::to<string>());
}

//******************************************************************************
Logger::UserAction from_string(string const& str) noexcept {
	if(str == "n") return Logger::UserAction::CANCEL;
	else if(str == "y") return Logger::UserAction::OK;
	else if(str == "s") return Logger::UserAction::SAVE;
	else if(str == "a") return Logger::UserAction::ABORT;
	else return Logger::UserAction::NOTHING;
}

//******************************************************************************
LoggerSink::LoggerSink(bool verbose)
: verbose(verbose)
{}

//******************************************************************************
Logger::UserAction LoggerSink::log(Logger::LogEvent const& log) const {
	if(!(log.level == Logger::Level::INFO && !verbose))
		print(cerr,
			"{} {}: {}{}{}",
			to_emoji(log.level),
			to_string(log.level),
			log.message,
			(log.informative.empty() ? string() : format(" {}", log.informative)),
			(log.details.empty() ? string() : format("\n{}", log.details)));

	if(is_interactive
	&& log.level == Logger::Level::QUESTION
	&& !log.user_actions.empty()) {
		Logger::UserAction res = Logger::UserAction::NOTHING;
		do {
			print(cerr, " {} ? ", to_string(log.user_actions));
			string ln;
			getline(cin, ln);
			res = from_string(ln);
		} while(res == Logger::UserAction::NOTHING);
		return res;
	} else {
		if(!(log.level == Logger::Level::INFO && !verbose))
			println(cerr);
		return Logger::UserAction::NOTHING;
	}
}

} // namespace ui::cli
