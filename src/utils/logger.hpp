///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>

//******************************************************************************
class Logger {
public:
	// https://doc.qt.io/qt-6/qmessagebox.html#Icon-enum
	enum class Level {
		QUESTION,
		INFO,
		WARNING,
		ERROR
	};

	// https://doc.qt.io/qt-6/qmessagebox.html#StandardButton-enum
	enum class UserAction {
		NOTHING,
		CANCEL,
		OK,
		SAVE,
		ABORT
	};

	using Id = std::size_t;

	struct LogEvent {
		Id destination = 0;
		Level level;
		std::set<UserAction> user_actions;
		std::optional<UserAction> default_user_action;
		std::string message;
		std::string informative;
		std::string details;
	};

	class ISink {
	public:
		virtual ~ISink() = default;
		virtual UserAction log(LogEvent const& log) const = 0;
		friend Logger;
	private:
		void set_interactive(bool is_the_interactive);
	protected:
		bool is_interactive = false;
	};

	static Id id(std::string_view const& id);

	static Logger& singleton();

	void set_as_the_interactive(Id id);

	void register_sink(Id id, std::unique_ptr<ISink>&& sink, bool is_the_interactive = false);

	UserAction log(LogEvent const& log) const;

private:
	ISink* interactive_sink = nullptr;
	std::map<Id, std::unique_ptr<ISink>> sinks;
};

//******************************************************************************
inline Logger::UserAction log(Logger::LogEvent const& log) {
	return Logger::singleton().log(log);
}
