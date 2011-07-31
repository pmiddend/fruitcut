#include <fruitapp/highscore/post_model.hpp>
#include <fruitapp/highscore/provider/object_base.hpp>
#include <fruitapp/highscore/provider/connection_base.hpp>
#include <fruitapp/gui/table/column.hpp>
#include <fruitapp/gui/table/row.hpp>
#include <fruitapp/gui/table/row_index.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/text.hpp>
#include <fcppt/move.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/lexical_cast.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/tr1/functional.hpp>
#include <iterator>

fruitapp::highscore::post_model::connection_wrapper::connection_wrapper(
	provider::connection_base_ptr _connection,
	fcppt::signal::shared_connection const _message_received,
	fcppt::signal::shared_connection const _error_received,
	fcppt::signal::shared_connection const _rank_received)
:
	connection_(
		fcppt::move(
			_connection)),
	message_received_(
		_message_received),
	error_received_(
		_error_received),
	rank_received_(
		_rank_received)
{
}

fruitapp::highscore::provider::connection_base &
fruitapp::highscore::post_model::connection_wrapper::connection()
{
	return 
		*connection_;
}

fruitapp::highscore::post_model::connection_wrapper::~connection_wrapper()
{
}

fruitapp::highscore::post_model::post_model(
	highscore::provider_sequence &_providers)
:
	providers_(
		_providers),
	message_received_(),
	error_received_(),
	row_added_(),
	row_removed_(),
	connections_()
{
}

fcppt::signal::auto_connection
fruitapp::highscore::post_model::message_received(
	callbacks::message_received const &f)
{
	return 
		message_received_.connect(
			f);
}

fcppt::signal::auto_connection
fruitapp::highscore::post_model::error_received(
	callbacks::error_received const &f)
{
	return 
		error_received_.connect(
			f);
}

void
fruitapp::highscore::post_model::post(
	highscore::name const &_name,
	highscore::score const &_score)
{
	for(
		gui::table::row_index::value_type i = 
			static_cast<gui::table::row_index::value_type>(connections_.size()-1);
		i != static_cast<gui::table::row_index::value_type>(-1);
		--i)
		row_removed_(
			i);
	
	for(
		highscore::provider_sequence::iterator i = 
			providers_.begin(); 
		i != providers_.end(); 
		++i)
	{
		provider::connection_base_ptr new_connection(
			i->create_connection());

		gui::table::row new_row;
		new_row.push_back(
			i->identifier());
		new_row.push_back(
			FCPPT_TEXT(""));

		gui::table::row_index::value_type const new_row_index = 
			static_cast<gui::table::row_index::value_type>(
				std::distance(
					providers_.begin(),
					i));

		row_added_(
			new_row_index,
			new_row);

		fcppt::container::ptr::push_back_unique_ptr(
			connections_,
			fcppt::make_unique_ptr<connection_wrapper>(
				fcppt::move(
					new_connection),
				fcppt::signal::shared_connection(
					new_connection->message_received(
						std::tr1::bind(
							&post_model::message_received_internal,
							this,
							fcppt::cref(
								*i),
							std::tr1::placeholders::_1))),
				fcppt::signal::shared_connection(
					new_connection->error_received(
						std::tr1::bind(
							&post_model::error_received_internal,
							this,
							fcppt::cref(
								*i),
							std::tr1::placeholders::_1))),
				fcppt::signal::shared_connection(
					new_connection->rank_received(
						std::tr1::bind(
							&post_model::rank_received_internal,
							this,
							fcppt::cref(
								*i),
							new_row_index,
							std::tr1::placeholders::_1)))));

		connections_.back().connection().post_rank(
			_name,
			_score);
	}
}

void
fruitapp::highscore::post_model::update()
{
	for(
		connection_sequence::iterator i = 
			connections_.begin();
		i != connections_.end();
		++i)
		i->connection().update();
}

fruitapp::gui::table::column_sequence const
fruitapp::highscore::post_model::columns() const
{
	return 
		fcppt::assign::make_container<gui::table::column_sequence>
			(gui::table::column(
				FCPPT_TEXT("Provider"),
				static_cast<gui::table::column::width_type>(
					0.9)))
			(gui::table::column(
				FCPPT_TEXT("Rank"),
				static_cast<gui::table::column::width_type>(
					0.1)));
}

fcppt::signal::auto_connection
fruitapp::highscore::post_model::row_added(
	gui::table::row_added const &f)
{
	return 
		row_added_.connect(
			f);
}

fcppt::signal::auto_connection
fruitapp::highscore::post_model::row_removed(
	gui::table::row_removed const &f)
{
	return 
		row_removed_.connect(
			f);
}

fruitapp::highscore::post_model::~post_model()
{
}

void
fruitapp::highscore::post_model::message_received_internal(
	provider::object_base const &_provider,
	fcppt::string const &_message)
{
	message_received_(
		_provider.identifier()+FCPPT_TEXT(": ")+_message);
}

void
fruitapp::highscore::post_model::error_received_internal(
	provider::object_base const &_provider,
	fcppt::string const &_message)
{
	error_received_(
		_provider.identifier()+FCPPT_TEXT(": ")+_message);
}

void
fruitapp::highscore::post_model::rank_received_internal(
	provider::object_base const &_provider,
	gui::table::row_index::value_type const _row_index,
	highscore::rank const &_rank)
{
	row_removed_(
		_row_index);
	
	gui::table::row new_row;
	new_row.push_back(
		_provider.identifier());
	new_row.push_back(
		_rank
		?
			fcppt::lexical_cast<fcppt::string>(
				*_rank)
		:
			fcppt::string(
				FCPPT_TEXT("-")));

	gui::table::row_index::value_type const new_row_index = 
		static_cast<gui::table::row_index::value_type>(
			connections_.size());

	row_added_(
		new_row_index,
		new_row);
}
