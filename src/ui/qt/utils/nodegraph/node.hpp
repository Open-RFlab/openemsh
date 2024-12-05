///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsWidget>
#include <QList>

#include "port.hpp"

class QGraphicsLinearLayout;

namespace ui::qt::nodegraph {

class Text;

//******************************************************************************
class Node : public QGraphicsWidget {
public:
	explicit Node(QString title, QGraphicsItem* parent = nullptr);
	explicit Node(QGraphicsItem* parent = nullptr);
	~Node();

	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

	Port* add_input_port(QString const& text = QString(), Port::AnchorPoint anchor = Port::AnchorPoint::LEFT);
	Port* add_output_port(QString const& text = QString(), Port::AnchorPoint anchor = Port::AnchorPoint::RIGHT);

	using QGraphicsWidget::updateGeometry;

	QList<Port*> input_ports;
	QList<Port*> output_ports;
protected:
	QVariant itemChange(GraphicsItemChange change, QVariant const& value) override;

	QGraphicsLinearLayout* layout();

	Text* title;

private:
	using QGraphicsWidget::layout;
//	using QGraphicsWidget::setLayout;

	QGraphicsLinearLayout* const linear_layout; // TODO main_v_layout?
};

} // namespace ui::qt::nodegraph
