///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QMessageBox>
#include <QStatusBar>

#include "utils/unreachable.hpp"

#include "logger.hpp"

using namespace std;

namespace ui::qt {

//******************************************************************************
Logger::UserAction from_qt(int button) noexcept {
	switch(static_cast<QMessageBox::StandardButton>(button)) {
	default: [[fallthrough]];
	case QMessageBox::NoButton: return Logger::UserAction::NOTHING;
	case QMessageBox::Cancel: return Logger::UserAction::CANCEL;
	case QMessageBox::Ok: return Logger::UserAction::OK;
	case QMessageBox::Save: return Logger::UserAction::SAVE;
	case QMessageBox::Close: return Logger::UserAction::CLOSE;
	case QMessageBox::Abort: return Logger::UserAction::ABORT;
	}
}

//******************************************************************************
QMessageBox::StandardButton to_qt(Logger::UserAction action) noexcept {
	switch(action) {
	case Logger::UserAction::NOTHING: return QMessageBox::NoButton;
	case Logger::UserAction::CANCEL: return QMessageBox::Cancel;
	case Logger::UserAction::OK: return QMessageBox::Ok;
	case Logger::UserAction::SAVE: return QMessageBox::Save;
	case Logger::UserAction::CLOSE: return QMessageBox::Close;
	case Logger::UserAction::ABORT: return QMessageBox::Abort;
	default: ::unreachable();
	}
}

//******************************************************************************
QMessageBox::StandardButtons to_qt(set<Logger::UserAction> const& actions) noexcept {
	QMessageBox::StandardButtons buttons = QMessageBox::NoButton;
	for(auto const action : actions)
		buttons |= to_qt(action);
	return buttons;
}

//******************************************************************************
QMessageBox::Icon to_qt(Logger::Level level) noexcept {
	switch(level) {
	case Logger::Level::QUESTION: return QMessageBox::Question;
	case Logger::Level::INFO: return QMessageBox::Information;
	case Logger::Level::WARNING: return QMessageBox::Warning;
	case Logger::Level::ERROR: return QMessageBox::Critical;
	default: ::unreachable();
	}
}

//******************************************************************************
LoggerSink::LoggerSink(QStatusBar* status_bar, QWidget* parent)
: status_bar(status_bar)
, parent(parent)
{}

//******************************************************************************
Logger::UserAction LoggerSink::log(Logger::LogEvent const& log) const {
	if(log.level != Logger::Level::QUESTION
	&& (log.user_actions.empty() || log.user_actions.contains(Logger::UserAction::NOTHING))) {
		status_bar->showMessage(QString::fromStdString(log.message));
		return Logger::UserAction::NOTHING;
	} else {
		QMessageBox message(parent);
		message.setText(QString::fromStdString(log.message));
		message.setInformativeText(QString::fromStdString(log.informative));
		message.setDetailedText(QString::fromStdString(log.details));
		message.setIcon(to_qt(log.level));
		message.setStandardButtons(is_interactive
			? to_qt(log.user_actions)
			: QMessageBox::Ignore);
		if(log.default_user_action.has_value())
			message.setDefaultButton(to_qt(log.default_user_action.value()));

		return from_qt(message.exec());
	}
}

} // namespace ui::qt
