///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsView>
#include <QObject>

#include "utils/state_management.hpp"
#include "processing_scene.hpp"

namespace domain {
class Board;
} // namespace domain

namespace ui::qt {

struct ProcessingState {
	ProcessingScene* scene;
};

//******************************************************************************
class ProcessingView : public QGraphicsView {
	Q_OBJECT
public:
	explicit ProcessingView(QWidget* parent = nullptr);
	void init(domain::Board const* _board);
	void clear();

	ProcessingStyleSelector style_selector;
	std::map<Timepoint*, ProcessingState> states;

	ProcessingState& get_current_state();
	void make_current_state();
	void go_to_current_state();

	ProcessingScene::DisplayMode get_display_mode();
	void set_display_mode(ProcessingScene::DisplayMode mode);
	void set_display_view_axes(domain::ViewAxisSpace<bool> const& axes);
	void set_display_plane(domain::Plane plane);
	void set_wire_style(nodegraph::Wire::Style style);

public slots:
	void fit();

protected:
	void wheelEvent(QWheelEvent* event) override;

private:
	domain::Board const* board;
	Timepoint* current_timepoint;

	nodegraph::Wire::Style wire_style;
	domain::Plane plane_displayed_on_structure_view;
	domain::ViewAxisSpace<bool> axes_displayed_on_structure_view;

	void populate(ProcessingScene* scene);
};

} // namespace ui::qt
