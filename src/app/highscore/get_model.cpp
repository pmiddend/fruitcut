#include "get_model.hpp"
#include "../gui/table/column.hpp"
#include <fcppt/assign/make_container.hpp>
#include <fcppt/lexical_cast.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/text.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <iterator>

fruitcut::app::highscore::get_model::get_model()
:
	row_added_(),
	row_removed_(),
	current_entry_set_()
{
}

fruitcut::app::gui::table::column_sequence const
fruitcut::app::highscore::get_model::columns() const
{
	return
		fcppt::assign::make_container<gui::table::column_sequence>
			(gui::table::column(
				FCPPT_TEXT("Nr."),
				static_cast<gui::table::column::width_type>(
					0.1)))
			(gui::table::column(
				FCPPT_TEXT("Name"),
				static_cast<gui::table::column::width_type>(
					0.5)))
			(gui::table::column(
				FCPPT_TEXT("Score"),
				static_cast<gui::table::column::width_type>(
					0.2)))
			(gui::table::column(
				FCPPT_TEXT("Date"),
				static_cast<gui::table::column::width_type>(
					0.2)));
}

fcppt::signal::auto_connection
fruitcut::app::highscore::get_model::row_added(
	gui::table::row_added const &f)
{
	return
		row_added_.connect(
			f);
}

fcppt::signal::auto_connection
fruitcut::app::highscore::get_model::row_removed(
	gui::table::row_removed const &f)
{
	return
		row_removed_.connect(
			f);
}

void
fruitcut::app::highscore::get_model::reset(
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
			static_cast<gui::table::row_index::value_type>(
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

		gui::table::row_index::value_type index = 
			static_cast<gui::table::row_index::value_type>(
				std::distance(
					current_entry_set_.begin(),
					i));

		gui::table::row new_row;
		new_row.push_back(
			fcppt::lexical_cast<fcppt::string>(
				index+1));
		new_row.push_back(
			i->name());
		new_row.push_back(
			fcppt::lexical_cast<fcppt::string>(
				i->score()));
		new_row.push_back(
			fcppt::from_std_string(
				boost::posix_time::to_simple_string(
					local_adjuster::utc_to_local(
						i->date_time()))));
		row_added_(
			index,
			new_row);
	}
}

fruitcut::app::highscore::get_model::~get_model()
{
}
