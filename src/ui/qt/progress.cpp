///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QLabel>
#include <QProgressBar>
#include <QStatusBar>

#include "progress.hpp"

using namespace std;

namespace ui::qt {

//******************************************************************************
ProgressBar::ProgressBar(QStatusBar* status_bar, size_t max, QString const& message)
: status_bar(status_bar)
, bar(new QProgressBar())
, label(new QLabel(message))
{
	bar->setRange(0, (int) max);
	status_bar->addWidget(bar);
	status_bar->addWidget(label);
}

//******************************************************************************
ProgressBar::~ProgressBar() {
	status_bar->removeWidget(label);
	status_bar->removeWidget(bar);
	delete label;
	delete bar;
}

//******************************************************************************
void ProgressBar::tick(size_t i) {
	bar->setValue((int) ++i);
}

//******************************************************************************
void ProgressBar::tick(size_t /*i*/, size_t j) {
	bar->setValue((int) ++j);
}

//******************************************************************************
void ProgressBar::complete() {
	status_bar->removeWidget(label);
	status_bar->removeWidget(bar);
}

} // namespace ui::qt
