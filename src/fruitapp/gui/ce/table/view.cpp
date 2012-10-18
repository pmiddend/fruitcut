#include <fruitapp/gui/ce/table/model.hpp>
#include <fruitapp/gui/ce/table/view.hpp>
#include <sge/cegui/to_cegui_string.hpp>
#include <sge/charconv/system_fwd.hpp>
#include <fcppt/insert_to_fcppt_string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assert/pre_message.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <CEGUI/widgets/ListboxTextItem.h>
#include <CEGUI/widgets/MultiColumnList.h>
#include <iostream>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::ce::table::view::view(
	sge::charconv::system &_charconv_system,
	CEGUI::Window &_impl,
	table::model &_model)
:
	charconv_system_(
		_charconv_system),
	impl_(
		dynamic_cast<CEGUI::MultiColumnList &>(
			_impl)),
	model_(
		_model),
	row_added_connection_(
		model_.row_added(
			std::tr1::bind(
				&view::row_added,
				this,
				std::tr1::placeholders::_1,
				std::tr1::placeholders::_2))),
	row_removed_connection_(
		model_.row_removed(
			std::tr1::bind(
				&view::row_removed,
				this,
				std::tr1::placeholders::_1)))
{
	// Make a copy to prevent the begin/end error
	column_sequence const columns =
		model_.columns();

	for(
		column_sequence::const_iterator i =
			columns.begin();
		i != columns.end();
		++i)
		impl_.addColumn(
			sge::cegui::to_cegui_string(
				i->name(),
				charconv_system_),
			impl_.getColumnCount(),
			CEGUI::UDim(
				i->width(),
				0));
}

fruitapp::gui::ce::table::view::~view()
{
}

void
fruitapp::gui::ce::table::view::row_added(
	table::row_index::value_type const &_index,
	table::row const &_row)
{
	FCPPT_ASSERT_PRE_MESSAGE(
		static_cast<CEGUI::uint>(_row.size()) == impl_.getColumnCount(),
		FCPPT_TEXT("Got ")+
		fcppt::insert_to_fcppt_string(
			_row.size())+
		FCPPT_TEXT(" columns in row_added, expected ")+
		fcppt::insert_to_fcppt_string(
			impl_.getColumnCount()));

	CEGUI::uint const new_row_index =
		impl_.insertRow(
			static_cast<CEGUI::uint>(
				_index),
			static_cast<CEGUI::uint>(
				_index));

	for(
		table::row::size_type i =
			0;
		i != _row.size();
		++i)
	{
		// I don't think the interface allows for non-automatic deletion
		impl_.setItem(
			new CEGUI::ListboxTextItem(
				sge::cegui::to_cegui_string(
					_row[i],
					charconv_system_)),
			static_cast<CEGUI::uint>(
				i),
			new_row_index);
	}
}

void
fruitapp::gui::ce::table::view::row_removed(
	table::row_index::value_type const &_index)
{
	FCPPT_ASSERT_PRE_MESSAGE(
		_index < impl_.getRowCount(),
		FCPPT_TEXT("There are ")+
		fcppt::insert_to_fcppt_string(
			impl_.getRowCount())+
		FCPPT_TEXT(" rows in the table. Got removal index ")+
		fcppt::insert_to_fcppt_string(
			_index));
	impl_.removeRow(
		static_cast<CEGUI::uint>(
			_index));
}
