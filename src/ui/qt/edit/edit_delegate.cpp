///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QComboBox>
#include <QEvent>
#include <QKeyEvent>
#include <QListView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>

#include <array>

#include "domain/mesh/meshline_policy.hpp"
#include "infra/utils/to_string.hpp"
#include "utils/concepts.hpp"
#include "utils/unconst.hpp"
#include "utils/unreachable.hpp"

#include "edit_delegate.hpp"

Q_DECLARE_METATYPE(domain::MeshlinePolicy::Normal)
Q_DECLARE_METATYPE(domain::MeshlinePolicy::Policy)

namespace ui::qt {

//******************************************************************************
using Normal = domain::MeshlinePolicy::Normal;
using Policy = domain::MeshlinePolicy::Policy;

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
template<Enum E>
QString convert(E e) {
	return QString::fromStdString(to_string(e));
}

//******************************************************************************
template<Enum E, std::size_t N>
QStringList convert(std::array<E, N> const& in) {
	QStringList out;
	for(auto& e : in)
		out.push_back(convert(e));
	return out;
}

//******************************************************************************
template<Enum E, std::size_t N>
constexpr std::size_t key(E e, std::array<E, N> const& all) {
	for(std::size_t i = 0; i < all.size(); ++i)
		if(all[i] == e)
			return i;
	unreachable();
}

//******************************************************************************
static constexpr auto key(Normal normal) { return key(normal, AllNormal); }
static constexpr auto key(Policy policy) { return key(policy, AllPolicy); }

//******************************************************************************
static_assert(AllNormal[key(Normal::NONE)] == Normal::NONE);
static_assert(AllNormal[key(Normal::MIN)] == Normal::MIN);
static_assert(AllNormal[key(Normal::MAX)] == Normal::MAX);
static_assert(AllPolicy[key(Policy::ONELINE)] == Policy::ONELINE);
static_assert(AllPolicy[key(Policy::HALFS)] == Policy::HALFS);
static_assert(AllPolicy[key(Policy::THIRDS)] == Policy::THIRDS);

//******************************************************************************
EditDelegate::EditDelegate(QObject* parent)
: QStyledItemDelegate(parent)
, normal_index(QModelIndex()) // Init at first paint().
, policy_index(QModelIndex()) // Init at first paint().
{}

//******************************************************************************
QWidget* EditDelegate::createEditor(QWidget* parent, QStyleOptionViewItem const& option, QModelIndex const& index) const {
	auto const type = index.data(Qt::UserRole + 1).typeId();

	auto const handle_enum = [&]<Enum E, std::size_t N>(std::array<E, N> const& all) {
		auto* widget = new QComboBox(parent);
		widget->addItems(convert(all));
		widget->setToolTip(index.data(Qt::ToolTipRole).toString());
		static_cast<QListView*>(widget->view())->setToolTip(index.data(Qt::ToolTipRole).toString());
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
	} else {
		return QStyledItemDelegate::createEditor(parent, option, index);
	}
}

//******************************************************************************
void EditDelegate::setEditorData(QWidget* editor, QModelIndex const& index) const {
	auto const type = index.data(Qt::UserRole + 1).typeId();

	auto const handle_enum = [&]<Enum E>() {
		auto* cb = static_cast<QComboBox*>(editor);
		cb->setCurrentIndex((int) key(index.data(Qt::UserRole + 1).value<E>()));
	};

	if(type == qMetaTypeId<Normal>()) {
		handle_enum.operator()<Normal>();
	} else if(type == qMetaTypeId<Policy>()) {
		handle_enum.operator()<Policy>();
	}

	QStyledItemDelegate::setEditorData(editor, index);
}

//******************************************************************************
void EditDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, QModelIndex const& index) const {
	auto const type = index.data(Qt::UserRole + 1).typeId();

	auto const handle_enum = [&]<Enum E, std::size_t N>(std::array<E, N> const& all) {
		auto const* cb = static_cast<QComboBox*>(editor);
		model->setData(index, QVariant::fromValue(all[cb->currentIndex()]), Qt::UserRole + 1);
		model->setData(index, cb->currentText(), Qt::EditRole);
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
	}

	QStyledItemDelegate::setModelData(editor, model, index);
}

//******************************************************************************
void EditDelegate::paint(QPainter* painter, QStyleOptionViewItem const& option, QModelIndex const& index) const {
	auto const type = index.data(Qt::UserRole + 1).typeId();

	auto const handle_enum = [&]<Enum E>() {
		auto* model = unconst(index.model());
		model->setData(
			index,
			convert(index.data(Qt::UserRole + 1).value<E>()),
			Qt::DisplayRole);
	};

	if(type == qMetaTypeId<Normal>()) {
		handle_enum.operator()<Normal>();
		unconst(this)->normal_index = index;
	} else if(type == qMetaTypeId<Policy>()) {
		handle_enum.operator()<Policy>();
		unconst(this)->policy_index = index;
	}

	QStyledItemDelegate::paint(painter, option, index);
}

//******************************************************************************
bool EditDelegate::eventFilter(QObject* object, QEvent* event) {
	QWidget* editor = qobject_cast<QWidget*>(object);
	if(!editor)
		return QStyledItemDelegate::eventFilter(object, event);

	if(event->type() == QEvent::KeyPress) {
		switch(static_cast<QKeyEvent*>(event)->key()) {
		case Qt::Key_Enter: [[fallthrough]];
		case Qt::Key_Return:
			if(qobject_cast<QComboBox*>(editor)) {
				emit commitData(editor);
				emit closeEditor(editor);
				return true;
			}
		}
	}

	return QStyledItemDelegate::eventFilter(object, event);
}

} // namespace ui::qt
