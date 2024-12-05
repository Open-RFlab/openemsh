///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QGraphicsPathItem>

namespace ui::qt::nodegraph {

class Port;

//******************************************************************************
class Wire : public QGraphicsPathItem {
public:
	enum class Style { DIRECT, CURVED } style;

	explicit Wire(Port* begin, Port* end, QGraphicsItem* parent = nullptr);
	~Wire();

	void update_path();

	Wire* unwire();

protected:
	void paint(QPainter* painter, QStyleOptionGraphicsItem const* option, QWidget* widget = nullptr) override;

private:
	Port* begin;
	Port* end;
};

} // namespace ui::qt::nodegraph
