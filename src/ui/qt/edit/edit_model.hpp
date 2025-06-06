///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <QStandardItemModel>

#include <map>

#include "app/steps.hpp"

namespace ui::qt {

namespace nodegraph {
class Node;
} // namespace nodegraph

//******************************************************************************
class EditModel : public QStandardItemModel {
	Q_OBJECT
public:
	static EditModel* make(nodegraph::Node* node, QObject* parent = nullptr);

	explicit EditModel(QObject* parent = nullptr);
	virtual void commit();

protected:
	static QStandardItem* make_property_item(QString const& str);
	static QStandardItem* make_bool_item(bool in);
	static bool is_true(bool const val) { return val; }
	static bool try_to_double(QString const& in, double& out);
	static bool try_to_ulong(QString const& in, std::size_t& out);
	static bool try_to_bool(Qt::CheckState const in, bool& out);
	template<typename O>
	static bool try_from_map(std::map<QString, O> const& map, QString const& in, O& out);

signals:
	void request_to_go_before(app::Step step);
	void edited(app::Step const redo_from);
};

//******************************************************************************
template<typename O>
bool EditModel::try_from_map(std::map<QString, O> const& map, QString const& in, O& out) {
	bool does_succeed = false;
	if(map.contains(in)) {
		out = map.at(in);
		does_succeed = true;
	}
	return does_succeed;
}

} // namespace ui::qt
