///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QVariant>


#include <algorithm>
#include <array>
#include <format>
#include <tuple>

#include "app/steps.hpp"
#include "domain/global.hpp"
#include "infra/utils/to_string.hpp"
#include "utils/logger.hpp"

Q_DECLARE_METATYPE(domain::Material::Type)
Q_DECLARE_METATYPE(std::optional<domain::Params::Mat>)
Q_DECLARE_METATYPE(std::optional<domain::Axis>)

namespace ui::qt {

//******************************************************************************
template<typename Var>
EditModelPerAxisPerCriteria<Var>::EditModelPerAxisPerCriteria(QString const& criteria_name, Var::value_type const& default_value, QObject* parent)
: IEditModelPerAxisPerCriteria(parent)
, default_value(default_value)
{
	setColumnCount(3);
	setHorizontalHeaderLabels({ "Axis", criteria_name, "Value" });
}

//******************************************************************************
template<typename Var>
bool EditModelPerAxisPerCriteria<Var>::commit() {
	Var v;

	using MaterialType = domain::Material::Type;
	using OptionalMaterial = std::optional<domain::Params::Mat>;
	using OptionalAxis = std::optional<domain::Axis>;

	using Criteria = std::tuple_element_t<1, typename Var::key_type>;
	using Value = Var::mapped_type;

	bool is_there_default = false;

	for(int i = 0; i < rowCount(); ++i) {
		OptionalAxis k1 = item(i, 0)->data().template value<OptionalAxis>();
		Criteria k2 = item(i, 1)->data().template value<Criteria>();

		if(k1 == domain::Params::ALL && k2 == domain::Params::ALL)
			is_there_default = true;

		Value value;
		bool does_succeed = false;
		if constexpr(std::is_integral_v<Value>) {
			does_succeed = try_to_ulong(item(i, 2)->text(), value);
		} else if constexpr(std::is_floating_point_v<Value>) {
			does_succeed = try_to_double(item(i, 2)->text(), value);
		} else {
			// Rely on Qt meta type system.
			v.emplace(std::make_tuple(k1, k2), item(i, 2)->data().template value<Value>());
//			static_assert(false, "Usupported case");
		}

		if(does_succeed) {
			v.emplace(std::make_tuple(k1, k2), value);
		} else {
			log({
				.level = Logger::Level::WARNING,
				.user_actions = { Logger::UserAction::OK },
				.message = std::format("Invalid data at row {}: {{{}, {}}} = {}",
					i+1,
					to_string(k1),
					to_string(k2),
					item(i, 2)->text().toStdString())
			});
			return false;
		}
	}

	if(!is_there_default) {
		log({
			.level = Logger::Level::WARNING,
			.user_actions = { Logger::UserAction::OK },
			.message = "Default rule must be provided: {*, *} = ?"
		});
		return false;
	}

	var = v;
	return true;
}

//******************************************************************************
template<typename Var>
QList<QStandardItem*> EditModelPerAxisPerCriteria<Var>::make_row(Var::value_type const& value) const {
	auto const& [key, val] = value;
	auto const& [k1, k2] = key;

	auto* item1 = new QStandardItem();
	item1->setText(QString::fromStdString(to_string(k1)));
	item1->setData(QVariant::fromValue(k1));

	auto* item2 = new QStandardItem();
	item2->setText(QString::fromStdString(to_string(k2)));
	item2->setData(QVariant::fromValue(k2));

	auto* item3 = new QStandardItem();
	if constexpr(std::is_arithmetic_v<decltype(val)>) {
		item3->setText(QString::number(val));
	} else {
		// Rely on Qt meta type system.
		item3->setData(QVariant::fromValue(val));
//		static_assert(false, "Usupported case");
	}

	return { item1, item2, item3 };
}

//******************************************************************************
template<typename Var>
void EditModelPerAxisPerCriteria<Var>::set(Var const& v) {
	var = v;

	removeRows(0, rowCount());

	for(auto const& value : v) {
		appendRow(make_row(value));
	}
}

//******************************************************************************
template<typename Var>
Var const& EditModelPerAxisPerCriteria<Var>::get() {
	return var;
}

//******************************************************************************
template<typename Var>
void EditModelPerAxisPerCriteria<Var>::add_new_row() {
	appendRow(make_row(default_value));
}

} // namespace ui::qt
