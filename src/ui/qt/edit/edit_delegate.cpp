///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QEvent>
#include <QKeyEvent>
#include <QLineEdit>
#include <QListView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>

#include <array>

#include "domain/mesh/meshline_policy.hpp"
#include "infra/utils/to_string.hpp"
#include "utils/concepts.hpp"
#include "utils/enum_utils.hpp"
#include "utils/unconst.hpp"
#include "utils/variant_utils.hpp"
#include "edit_model_per_axis_per_criteria.hpp"
#include "edit_dialog_per_axis_per_criteria.hpp"
#include "edit_dialog.hpp"
#include "ui_edit_dialog.h"

#include "edit_delegate.hpp"

Q_DECLARE_METATYPE(domain::MeshlinePolicy::Normal)
Q_DECLARE_METATYPE(domain::MeshlinePolicy::Policy)

using PerAxisPerMaterialDouble = domain::Params::PerAxisPer<domain::Params::Mat, double>;
using PerAxisPerMaterialSizeT = domain::Params::PerAxisPer<domain::Params::Mat, std::size_t>;

Q_DECLARE_METATYPE(PerAxisPerMaterialDouble)
Q_DECLARE_METATYPE(PerAxisPerMaterialSizeT)

namespace ui::qt {

//******************************************************************************
using Normal = domain::MeshlinePolicy::Normal;
using Policy = domain::MeshlinePolicy::Policy;
using MaterialType = domain::Material::Type;
using OptionalAxis = std::optional<domain::Axis>;

//******************************************************************************
OptionalMaterialState state_of(OptionalMaterial const& mat) {
	if(mat.has_value())
		return std::visit(overloaded {
			[&](std::string const& s) { return s.empty() ? OptionalMaterialState::ALL : OptionalMaterialState::NAME; },
			[&](MaterialType const&) { return OptionalMaterialState::TYPE; }
		}, mat.value());
	else
		return OptionalMaterialState::ALL;
}

//******************************************************************************
static auto constexpr AllNormal = std::array {
	Normal::NONE,
	Normal::MIN,
	Normal::MAX
};

//******************************************************************************
static auto constexpr AllPolicy = std::array {
	Policy::ONELINE,
	Policy::HALFS,
	Policy::THIRDS
};

//******************************************************************************
static auto constexpr AllMaterialType = std::array {
	MaterialType::PORT,
	MaterialType::CONDUCTOR,
	MaterialType::DIELECTRIC,
	MaterialType::AIR
};

//******************************************************************************
static auto constexpr AllOptionalAxis = std::array<std::optional<domain::Axis>, 4> {
	domain::Axis::X,
	domain::Axis::Y,
	domain::Axis::Z,
	domain::Params::ALL
};

//******************************************************************************
template<typename E>
QString to_qstring(E e) {
	return QString::fromStdString(to_string(e));
}

//******************************************************************************
template<typename E, std::size_t N>
QStringList to_qstring(std::array<E, N> const& in) {
	QStringList out;
	for(auto& e : in)
		out.push_back(to_qstring(e));
	return out;
}

//******************************************************************************
static constexpr auto key(Normal normal) { return ::key(normal, AllNormal); }
static constexpr auto key(Policy policy) { return ::key(policy, AllPolicy); }
static constexpr auto key(MaterialType type) { return ::key(type, AllMaterialType); }
static constexpr auto key(OptionalAxis axis) { return ::key(axis, AllOptionalAxis); }

//******************************************************************************
static_assert(AllNormal[key(Normal::NONE)] == Normal::NONE);
static_assert(AllNormal[key(Normal::MIN)] == Normal::MIN);
static_assert(AllNormal[key(Normal::MAX)] == Normal::MAX);
static_assert(AllPolicy[key(Policy::ONELINE)] == Policy::ONELINE);
static_assert(AllPolicy[key(Policy::HALFS)] == Policy::HALFS);
static_assert(AllPolicy[key(Policy::THIRDS)] == Policy::THIRDS);
static_assert(AllMaterialType[key(MaterialType::PORT)] == MaterialType::PORT);
static_assert(AllMaterialType[key(MaterialType::CONDUCTOR)] == MaterialType::CONDUCTOR);
static_assert(AllMaterialType[key(MaterialType::DIELECTRIC)] == MaterialType::DIELECTRIC);
static_assert(AllMaterialType[key(MaterialType::AIR)] == MaterialType::AIR);
static_assert(AllOptionalAxis[key(domain::Axis::X)] == domain::Axis::X);
static_assert(AllOptionalAxis[key(domain::Axis::Y)] == domain::Axis::Y);
static_assert(AllOptionalAxis[key(domain::Axis::Z)] == domain::Axis::Z);
static_assert(AllOptionalAxis[key((OptionalAxis) domain::Params::ALL)] == domain::Params::ALL);

//******************************************************************************
QString const OptionalMaterialEditor::by_name_str = "By name…";

//******************************************************************************
OptionalMaterialEditor::OptionalMaterialEditor(QWidget* parent)
: QComboBox(parent)
{
	addItem("*", -1);
	addItems(to_qstring(AllMaterialType));
	addItem(by_name_str, -2);

	setEditable(false);

	connect(
		this, &QComboBox::activated,
		this, &OptionalMaterialEditor::on_activated);
}

//******************************************************************************
void OptionalMaterialEditor::on_activated(int index) {
	if(index == index_of_by_name()) {
		setEditable(true);
		clearEditText();
		lineEdit()->setPlaceholderText(by_name_str);
		connect(
			lineEdit(), &QLineEdit::editingFinished,
			this, &OptionalMaterialEditor::editing_finished,
			Qt::UniqueConnection);
	} else {
		setEditable(false);
		emit editing_finished();
	}
}

//******************************************************************************
void OptionalMaterialEditor::set_value(OptionalMaterial const& material) {
	blockSignals(true);
	setEditable(false);

	switch(state_of(material)) {
	case OptionalMaterialState::ALL:
		setCurrentIndex(index_of_wildcard());
		break;
	case OptionalMaterialState::TYPE:
		setCurrentIndex(key(std::get<MaterialType>(material.value())) + 1);
		break;
	case OptionalMaterialState::NAME:
		setCurrentIndex(index_of_by_name());
		setEditable(true);
		setEditText(QString::fromStdString(std::get<std::string>(material.value())));
		lineEdit()->setCursorPosition(lineEdit()->text().length());
		break;
	default: unreachable();
	}

	blockSignals(false);
}

//******************************************************************************
OptionalMaterial OptionalMaterialEditor::get_value() const {
	int index = currentIndex();
	if(index == index_of_wildcard()) {
		return domain::Params::ALL;
	} else if(index == index_of_by_name()) {
		QString text = currentText().trimmed();
		if(text.isEmpty())
			return domain::Params::ALL;
		else
			return text.toStdString();
	} else {
		return AllMaterialType[index - 1];
	}
}

//******************************************************************************
int OptionalMaterialEditor::index_of_wildcard() const {
//	return findData(-1);
	return 0;
}

//******************************************************************************
int OptionalMaterialEditor::index_of_by_name() const {
	return findData(-2);
}

//******************************************************************************
EditDelegate::EditDelegate(QObject* parent)
: QStyledItemDelegate(parent)
, normal_index(QModelIndex()) // Init at first paint().
, policy_index(QModelIndex()) // Init at first paint().
{}

//******************************************************************************
QWidget* EditDelegate::createEditor(QWidget* parent, QStyleOptionViewItem const& option, QModelIndex const& index) const {
	auto const type = index.data(Qt::UserRole + 1).typeId();

	auto const handle_enum = [&]<typename E, std::size_t N>(std::array<E, N> const& all) {
		auto* widget = new QComboBox(parent);
		widget->addItems(to_qstring(all));
		widget->setToolTip(index.data(Qt::ToolTipRole).toString());
		static_cast<QListView*>(widget->view())->setToolTip(index.data(Qt::ToolTipRole).toString());
		return widget;
	};

	auto const handle_per_axis_per_criteria = [&, parent]<typename T>(QString criteria_name, T::value_type const& default_value) {
		QString title = static_cast<QStandardItemModel const*>(index.model())->item(index.row(), 0)->text();
		QString tooltip = static_cast<QStandardItemModel const*>(index.model())->item(index.row(), 0)->toolTip();
		auto* model = new EditModelPerAxisPerCriteria<T>(criteria_name, default_value, parent);
		auto* dialog = new EditDialogPerAxisPerCriteria(model, title, tooltip, parent);
		model->setParent(dialog);
		connect(
			dialog, &QDialog::accepted,
			[this, dialog]() {
				emit unconst(this)->commitData(dialog);
				emit unconst(this)->closeEditor(dialog);
			}
		);
		connect(
			dialog, &QDialog::rejected,
			[this, dialog]() {
				emit unconst(this)->closeEditor(dialog);
			}
		);
		return dialog;
	};

	auto const handle_optional_material = [&]() {
		auto* widget = new OptionalMaterialEditor(parent);
		connect(
			widget, &OptionalMaterialEditor::editing_finished,
			[this, widget]() {
				emit unconst(this)->commitData(widget);
				emit unconst(this)->closeEditor(widget);
			}
		);
		return widget;
	};

	auto const bound_normal_choice_by_current_policy = [&](auto const* cb) {
		if(policy_index.isValid()) {
			auto policy = policy_index.data(Qt::UserRole + 1).value<Policy>();
			switch(policy) {
			case Policy::ONELINE: [[fallthrough]];
			case Policy::HALFS: {
				// Actually made uneditable since there is no choice,
				// in enforce_coherent_normal_regarding_current_policy().
				auto const* m = static_cast<QStandardItemModel*>(cb->model());
				m->item(key(Normal::NONE))->setEnabled(true);
				m->item(key(Normal::MIN))->setEnabled(false);
				m->item(key(Normal::MAX))->setEnabled(false);
			} break;
			case Policy::THIRDS: {
				auto const* m = static_cast<QStandardItemModel*>(cb->model());
				m->item(key(Normal::NONE))->setEnabled(false);
				m->item(key(Normal::MIN))->setEnabled(true);
				m->item(key(Normal::MAX))->setEnabled(true);
			} break;
			default: break;
			}
		}
	};

	if(type == qMetaTypeId<Normal>()) {
		auto* cb = handle_enum(AllNormal);
		bound_normal_choice_by_current_policy(cb);
		return cb;
	} else if(type == qMetaTypeId<Policy>()) {
		return handle_enum(AllPolicy);
	} else if(type == qMetaTypeId<OptionalAxis>()) {
		return handle_enum(AllOptionalAxis);
	} else if(type == qMetaTypeId<OptionalMaterial>()) {
		return handle_optional_material();
	} else if(type == qMetaTypeId<PerAxisPerMaterialDouble>()) {
		return handle_per_axis_per_criteria.operator()<PerAxisPerMaterialDouble>("Material", {{ domain::Params::ALL, domain::Params::ALL }, 0.0 });
	} else if(type == qMetaTypeId<PerAxisPerMaterialSizeT>()) {
		return handle_per_axis_per_criteria.operator()<PerAxisPerMaterialSizeT>("Material", {{ domain::Params::ALL, domain::Params::ALL }, 0 });
	} else {
		return QStyledItemDelegate::createEditor(parent, option, index);
	}
}

//******************************************************************************
void EditDelegate::setEditorData(QWidget* editor, QModelIndex const& index) const {
	auto const type = index.data(Qt::UserRole + 1).typeId();

	auto const handle_enum = [&]<typename E>() {
		auto* cb = static_cast<QComboBox*>(editor);
		cb->setCurrentIndex((int) key(index.data(Qt::UserRole + 1).value<E>()));
	};

	auto const handle_optional_material = [&]() {
		auto* om = static_cast<OptionalMaterialEditor*>(editor);
		om->set_value(index.data(Qt::UserRole + 1).value<OptionalMaterial>());
	};

	auto const handle_per_axis_per_criteria = [&]<typename T>() {
		auto* d = static_cast<EditDialogPerAxisPerCriteria*>(editor);
		auto* m = static_cast<EditModelPerAxisPerCriteria<T>*>(d->get_model());
		m->set(index.data(Qt::UserRole + 1).value<T>());
	};

	if(type == qMetaTypeId<Normal>()) {
		handle_enum.operator()<Normal>();
	} else if(type == qMetaTypeId<Policy>()) {
		handle_enum.operator()<Policy>();
	} else if(type == qMetaTypeId<OptionalAxis>()) {
		handle_enum.operator()<OptionalAxis>();
	} else if(type == qMetaTypeId<OptionalMaterial>()) {
		handle_optional_material();
		return; // Necessary!
	} else if(type == qMetaTypeId<PerAxisPerMaterialDouble>()) {
		handle_per_axis_per_criteria.operator()<PerAxisPerMaterialDouble>();
	} else if(type == qMetaTypeId<PerAxisPerMaterialSizeT>()) {
		handle_per_axis_per_criteria.operator()<PerAxisPerMaterialSizeT>();
	}

	QStyledItemDelegate::setEditorData(editor, index);
}

//******************************************************************************
void EditDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, QModelIndex const& index) const {
	auto const type = index.data(Qt::UserRole + 1).typeId();

	auto const handle_enum = [&]<typename E, std::size_t N>(std::array<E, N> const& all) {
		auto const* cb = static_cast<QComboBox*>(editor);
		model->setData(index, QVariant::fromValue(all[cb->currentIndex()]), Qt::UserRole + 1);
		model->setData(index, cb->currentText(), Qt::EditRole);
	};

	auto const handle_optional_material = [&]() {
		auto* om = qobject_cast<OptionalMaterialEditor*>(editor);
		model->setData(index, QVariant::fromValue(om->get_value()), Qt::UserRole + 1);
	};

	auto const handle_per_axis_per_criteria = [&]<typename T>() {
		auto* d = static_cast<EditDialogPerAxisPerCriteria*>(editor);
		auto* m = static_cast<EditModelPerAxisPerCriteria<T>*>(d->get_model());
		model->setData(index, QVariant::fromValue(m->get()), Qt::UserRole + 1);
	};

	auto const enforce_coherent_normal_regarding_current_policy = [&]() {
		if(normal_index.isValid()) {
			auto policy = model->data(index, Qt::UserRole + 1).value<Policy>();
			switch(policy) {
			case Policy::ONELINE: [[fallthrough]];
			case Policy::HALFS: {
				model->setData(normal_index, QVariant::fromValue(Normal::NONE), Qt::UserRole + 1);
				auto* item = static_cast<QStandardItemModel*>(model)->itemFromIndex(normal_index);
				item->setEditable(false);
			} break;
			case Policy::THIRDS: {
				if(model->data(normal_index, Qt::UserRole + 1).value<Normal>() == Normal::NONE) {
					model->setData(normal_index, QVariant::fromValue(Normal::MIN), Qt::UserRole + 1);
					auto* item = static_cast<QStandardItemModel*>(model)->itemFromIndex(normal_index);
					item->setEditable(true);
				}
			} break;
			default: break;
			}
		}
	};

	if(type == qMetaTypeId<Normal>()) {
		handle_enum(AllNormal);
	} else if(type == qMetaTypeId<Policy>()) {
		handle_enum(AllPolicy);
		enforce_coherent_normal_regarding_current_policy();
	} else if(type == qMetaTypeId<OptionalAxis>()) {
		handle_enum(AllOptionalAxis);
	} else if(type == qMetaTypeId<OptionalMaterial>()) {
		handle_optional_material();
		return; // Necessary!
	} else if(type == qMetaTypeId<PerAxisPerMaterialDouble>()) {
		handle_per_axis_per_criteria.operator()<PerAxisPerMaterialDouble>();
	} else if(type == qMetaTypeId<PerAxisPerMaterialSizeT>()) {
		handle_per_axis_per_criteria.operator()<PerAxisPerMaterialSizeT>();
	}

	QStyledItemDelegate::setModelData(editor, model, index);
}

//******************************************************************************
void EditDelegate::paint(QPainter* painter, QStyleOptionViewItem const& option, QModelIndex const& index) const {
	auto const type = index.data(Qt::UserRole + 1).typeId();

	auto const handle_enum = [&]<typename E>() {
		auto* model = unconst(index.model());
		model->setData(
			index,
			to_qstring(index.data(Qt::UserRole + 1).value<E>()),
			Qt::DisplayRole);
	};

	auto const handle_optional_material = [&]() {
		auto* model = unconst(static_cast<QStandardItemModel const*>(index.model()));
		model->setData(
			index,
			to_qstring(index.data(Qt::UserRole + 1).value<OptionalMaterial>()),
			Qt::DisplayRole);
	};

	if(type == qMetaTypeId<Normal>()) {
		handle_enum.operator()<Normal>();
		unconst(this)->normal_index = index;
	} else if(type == qMetaTypeId<Policy>()) {
		handle_enum.operator()<Policy>();
		unconst(this)->policy_index = index;
	} else if(type == qMetaTypeId<OptionalAxis>()) {
		handle_enum.operator()<OptionalAxis>();
	} else if(type == qMetaTypeId<OptionalMaterial>()) {
		handle_optional_material();
	} else if(type == qMetaTypeId<PerAxisPerMaterialDouble>()
	       || type == qMetaTypeId<PerAxisPerMaterialSizeT>()) {
		auto* model = unconst(index.model());
		model->setData(index, "...", Qt::DisplayRole);
	}

	QStyledItemDelegate::paint(painter, option, index);
}

//******************************************************************************
bool EditDelegate::eventFilter(QObject* object, QEvent* event) {
	QWidget* editor = qobject_cast<QWidget*>(object);
	if(!editor)
		return QStyledItemDelegate::eventFilter(object, event);

	if(auto* d = qobject_cast<EditDialog*>(editor); d) {
		if(event->type() == QEvent::KeyPress) {
			switch(static_cast<QKeyEvent*>(event)->key()) {
			case Qt::Key_Enter: [[fallthrough]];
			case Qt::Key_Return:
				d->on_dbb_ok_accepted();
				return true; // Block
			default:
				break; // Handled by QStyledItemDelegate::eventFilter
			}
		} else if(event->type() == QEvent::Show) {
			return false; // Propagate further
		} else if(event->type() == QEvent::Hide) {
			return false; // Propagate further
		} else {
			// Handled by QStyledItemDelegate::eventFilter
		}
	} else if(auto* om = qobject_cast<OptionalMaterialEditor*>(editor); om) {
		if(event->type() == QEvent::KeyPress) {
			switch(static_cast<QKeyEvent*>(event)->key()) {
			case Qt::Key_Enter: [[fallthrough]];
			case Qt::Key_Return:
				emit om->editing_finished();
				return true; // Block
			default:
				break; // Handled by QStyledItemDelegate::eventFilter
			}
		}
	}

	return QStyledItemDelegate::eventFilter(object, event);
}

} // namespace ui::qt
