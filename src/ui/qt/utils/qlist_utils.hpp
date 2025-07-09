///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#include <QList>

#include <type_traits>

namespace ui::qt {

// Downcast
//******************************************************************************
template<typename O, typename I>
requires std::is_base_of_v<
	std::remove_reference_t<std::remove_pointer_t<I>>,
	std::remove_reference_t<std::remove_pointer_t<O>>>
QList<O> dynamic_to_qlist(QList<I> const& list_i) {
	QList<O> list_o;
	for(I i : list_i)
		if(O o = dynamic_cast<O>(i); o)
			list_o.emplace_back(o);
	return list_o;
}

// Upcast
//******************************************************************************
template<typename O, typename I>
//requires std::is_base_of_v<
//	std::remove_reference_t<std::remove_pointer_t<O>>,
//	std::remove_reference_t<std::remove_pointer_t<I>>>
QList<O> to_qlist(QList<I> const& list_i) {
	QList<O> list_o;
	for(I i : list_i)
		list_o.emplace_back(static_cast<O>(i));
	return list_o;
}

} // namespace ui::qt
