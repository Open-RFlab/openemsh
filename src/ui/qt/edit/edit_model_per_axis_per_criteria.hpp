///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include "edit_model.hpp"

namespace ui::qt {

//******************************************************************************
class IEditModelPerAxisPerCriteria : public EditModel {
	Q_OBJECT
public:
	explicit IEditModelPerAxisPerCriteria(QObject* parent = nullptr);

public slots:
	virtual void add_new_row() = 0;
};


//******************************************************************************
template<typename Var>
class EditModelPerAxisPerCriteria : public IEditModelPerAxisPerCriteria {
public:
	EditModelPerAxisPerCriteria(QString const& criteria_name, Var::value_type const& default_value, QObject* parent = nullptr);
	bool commit() override;
	void add_new_row() override;

	void set(Var const& v);
	Var const& get();

private:
	Var::value_type const default_value;
	Var var;

	QList<QStandardItem*> make_row(Var::value_type const& value) const;
};

} // namespace ui::qt

#include "edit_model_per_axis_per_criteria.ipp"
