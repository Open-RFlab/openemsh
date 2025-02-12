///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsView>
#include <QObject>

#include "processing_scene.hpp"

class QSlider;

namespace domain {
class Board;
} // namespace domain

namespace ui::qt {

//******************************************************************************
class ProcessingView : public QGraphicsView {
	Q_OBJECT
public:
	explicit ProcessingView(QWidget* parent = nullptr);
	~ProcessingView();

	void set(domain::Board const* board);

	ProcessingScene* processing_scene;

public slots:
	void fit();

protected:
	void wheelEvent(QWheelEvent* event) override;

private:
	domain::Board const* board;
};

} // namespace ui::qt
