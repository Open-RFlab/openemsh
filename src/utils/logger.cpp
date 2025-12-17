///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <utility>

#include "logger.hpp"

using namespace std;

//******************************************************************************
Logger::Id Logger::id(string_view const& id) {
	return hash<string_view>{}(id);
}

//******************************************************************************
void Logger::ISink::set_interactive(bool is_the_interactive) {
	is_interactive = is_the_interactive;
}

//******************************************************************************
Logger& Logger::singleton() {
	static Logger logger;
	return logger;
}

//******************************************************************************
void Logger::set_as_the_interactive(Id id) {
	if(id == 0) {
		interactive_sink = nullptr;
		for(auto& [_, sink] : sinks)
			sink->set_interactive(false);
	} else if(sinks.contains(id)) {
		interactive_sink = sinks.at(id).get();
		interactive_sink->set_interactive(true);
		for(auto& [_, sink] : sinks)
			if(sink.get() != interactive_sink)
				sink->set_interactive(false);
	}
}

//******************************************************************************
void Logger::register_sink(Id id, unique_ptr<ISink>&& sink, bool is_the_interactive) {
	sinks.emplace(id, std::move(sink));
	if(is_the_interactive)
		set_as_the_interactive(id);
}

//******************************************************************************
Logger::UserAction Logger::log(LogEvent const& log) const {
	if(log.destination && sinks.contains(log.destination)) {
		return sinks.at(log.destination)->log(log);
	} else {
		UserAction ret = UserAction::NOTHING;
		for(auto const& [_, sink] : sinks) {
			if(auto res = sink->log(log)
			; sink.get() == interactive_sink)
				ret = res;
		}
		return ret;
	}
}
