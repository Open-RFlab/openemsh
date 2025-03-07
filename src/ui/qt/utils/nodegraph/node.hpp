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

#include "utils/down_up_cast.hpp"
#include "highlightable.hpp"
#include "port.hpp"

class QGraphicsLinearLayout;
class Entity;

namespace ui::qt::nodegraph {

class Container;
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

	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

	Port* add_input_port(QString const& text = QString(), Port::AnchorPoint anchor = Port::AnchorPoint::LEFT);
	Port* add_output_port(QString const& text = QString(), Port::AnchorPoint anchor = Port::AnchorPoint::RIGHT);

	QList<Node*> get_chain() const;
	static void traverse_up(QSet<Node*>& out, Node const* node);
	static void traverse_down(QSet<Node*>& out, Node const* node);

	QList<Node*> traverse(Port const* port) const;
	QList<Node*> traverse(QList<Port*> const& ports) const;
	QList<Node*> get_input_nodes() const;
	QList<Node*> get_output_nodes() const;

	Container* get_surrounding_container() const;
	void show_after_parents();

	void set_highlighted(bool is_highlighted, QGraphicsItem const* by_item) override;

	using QGraphicsWidget::updateGeometry;

	QList<Port*> input_ports;
	QList<Port*> output_ports;

protected:
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	QVariant itemChange(GraphicsItemChange change, QVariant const& value) override;

	// Convenience function that's costly: don't use it often.
	// It is better to populate propagate_highlight satically.
	//**************************************************************************
	template<typename ...T>
	requires (sizeof...(T) > 0)
	void retrieve_highlightable_children() {
		for(auto* item : childItems())
			if(auto* highlightable = down_up_cast<Highlightable, T...>(item)
			; highlightable)
				propagate_highlight.insert(highlightable);
	}
	void retrieve_highlightable_children();

	QGraphicsLinearLayout* layout();

	QSet<Highlightable*> propagate_highlight;
	Text* title;

private:
	using QGraphicsWidget::layout;
//	using QGraphicsWidget::setLayout;

	QGraphicsLinearLayout* const linear_layout; // TODO main_v_layout?
};

} // namespace ui::qt::nodegraph
