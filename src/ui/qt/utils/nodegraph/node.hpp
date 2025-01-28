///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QBrush>
#include <QGraphicsWidget>
#include <QList>
#include <QSet>

#include <functional>

#include "highlightable.hpp"
#include "port.hpp"

class QGraphicsLinearLayout;
class Entity;

namespace ui::qt::nodegraph {

class Text;

//******************************************************************************
class Node
: public QGraphicsWidget
, public Highlightable
{
public:
	struct Params final {
		qreal radius = 1;
		QBrush title_background_regular = QBrush(Qt::black);
		QBrush title_background_highlighted = title_background_regular;
		QBrush title_background_selected = title_background_regular;
		QBrush title_background_regular_hovered = title_background_regular;
		QBrush title_background_highlighted_hovered = title_background_highlighted;
		QBrush title_background_selected_hovered = title_background_selected;
		QBrush background_regular = QBrush(Qt::darkGray);
		QBrush background_highlighted = background_regular;
		QBrush background_selected = background_regular;
		QBrush background_regular_hovered = background_regular;
		QBrush background_highlighted_hovered = background_highlighted;
		QBrush background_selected_hovered = background_selected;
	};

	std::function<Params const& ()> locate_node_params;

	explicit Node(QString title, QGraphicsItem* parent = nullptr);
	explicit Node(QGraphicsItem* parent = nullptr);
	~Node();

	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

	Port* add_input_port(QString const& text = QString(), Port::AnchorPoint anchor = Port::AnchorPoint::LEFT);
	Port* add_output_port(QString const& text = QString(), Port::AnchorPoint anchor = Port::AnchorPoint::RIGHT);

	QList<Node*> get_chain() const;
	static void traverse_up(QSet<Node*>& out, Node const* node);
	static void traverse_down(QSet<Node*>& out, Node const* node);

	void set_highlighted(bool is_highlighted, QGraphicsItem const* by_item) override;

	using QGraphicsWidget::updateGeometry;

	QList<Port*> input_ports;
	QList<Port*> output_ports;

protected:
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	QVariant itemChange(GraphicsItemChange change, QVariant const& value) override;

	QGraphicsLinearLayout* layout();

	Text* title;

private:
	using QGraphicsWidget::layout;
//	using QGraphicsWidget::setLayout;

	QGraphicsLinearLayout* const linear_layout; // TODO main_v_layout?
};

} // namespace ui::qt::nodegraph
