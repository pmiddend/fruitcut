#include <fruitapp/gui/ce/table/column.hpp>
#include <fruitapp/gui/ce/get_model.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/insert_to_fcppt_string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <iterator>
#include <fcppt/config/external_end.hpp>

namespace
{
fcppt::string const
to_human_readable_string(
	boost::posix_time::time_duration const &_duration)
{
	return
		fcppt::insert_to_fcppt_string(
			_duration.total_seconds());
}
}

fruitapp::gui::ce::get_model::get_model()
:
	row_added_(),
	row_removed_(),
	current_entry_set_()
{
}

fruitapp::gui::ce::table::column_sequence const
fruitapp::gui::ce::get_model::columns() const
{
	return
		fcppt::assign::make_container<fruitapp::gui::ce::table::column_sequence>
			(fruitapp::gui::ce::table::column(
				FCPPT_TEXT("Nr."),
				static_cast<fruitapp::gui::ce::table::column::width_type>(
					0.1)))
			(fruitapp::gui::ce::table::column(
				FCPPT_TEXT("Name"),
				static_cast<fruitapp::gui::ce::table::column::width_type>(
					0.5)))
			(fruitapp::gui::ce::table::column(
				FCPPT_TEXT("Score"),
				static_cast<fruitapp::gui::ce::table::column::width_type>(
					0.2)))
			(fruitapp::gui::ce::table::column(
				FCPPT_TEXT("Date"),
				static_cast<fruitapp::gui::ce::table::column::width_type>(
					0.2)));
}

fcppt::signal::auto_connection
fruitapp::gui::ce::get_model::row_added(
	fruitapp::gui::ce::table::row_added const &f)
{
	return
		row_added_.connect(
			f);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::get_model::row_removed(
	fruitapp::gui::ce::table::row_removed const &f)
{
	return
		row_removed_.connect(
			f);
}

void
fruitapp::gui::ce::get_model::reset(
	highscore::entry_set const &entries)
{
	for(
		highscore::entry_set::size_type i =
			static_cast<highscore::entry_set::size_type>(
				current_entry_set_.size()-1);
		i != static_cast<highscore::entry_set::size_type>(-1);
		--i)
	{
		row_removed_(
			static_cast<fruitapp::gui::ce::table::row_index::value_type>(
				i));
	}

	current_entry_set_ =
		entries;

	for(
		highscore::entry_set::const_iterator i =
			current_entry_set_.begin();
		i != current_entry_set_.end();
		++i)
	{
		typedef
		boost::date_time::c_local_adjustor<boost::posix_time::ptime>
		local_adjuster;

		fruitapp::gui::ce::table::row_index::value_type index =
			static_cast<fruitapp::gui::ce::table::row_index::value_type>(
				std::distance(
					current_entry_set_.begin(),
					i));

		fruitapp::gui::ce::table::row new_row;
		new_row.push_back(
			fcppt::insert_to_fcppt_string(
				index+1));
		new_row.push_back(
			i->name());
		new_row.push_back(
			fcppt::insert_to_fcppt_string(
				i->score()));
		new_row.push_back(
			to_human_readable_string(
				boost::posix_time::second_clock::local_time() -
				local_adjuster::utc_to_local(
					i->date_time())));
		row_added_(
			index,
			new_row);
	}
}

fruitapp::gui::ce::get_model::~get_model()
{
}
