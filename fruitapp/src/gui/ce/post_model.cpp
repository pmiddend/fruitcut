#include <fruitapp/gui/ce/post_model.hpp>
#include <fruitapp/gui/ce/table/column.hpp>
#include <fruitapp/gui/ce/table/row.hpp>
#include <fruitapp/gui/ce/table/row_index.hpp>
#include <fruitapp/highscore/callbacks/error_received.hpp>
#include <fruitapp/highscore/callbacks/message_received.hpp>
#include <fruitapp/highscore/callbacks/rank_received.hpp>
#include <fruitapp/highscore/provider/connection_base.hpp>
#include <fruitapp/highscore/provider/object_base.hpp>
#include <fcppt/insert_to_fcppt_string.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/optional/maybe.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <iterator>
#include <utility>
#include <fcppt/config/external_end.hpp>


fruitapp::gui::ce::post_model::connection_wrapper::connection_wrapper(
	fruitapp::highscore::provider::connection_base_ptr &&_connection,
	fcppt::signal::auto_connection &&_message_received,
	fcppt::signal::auto_connection &&_error_received,
	fcppt::signal::auto_connection &&_rank_received)
:
	connection_(
		std::move(
			_connection)),
	message_received_(
		std::move(
			_message_received)),
	error_received_(
		std::move(
			_error_received)),
	rank_received_(
		std::move(
			_rank_received))
{
}

fruitapp::highscore::provider::connection_base &
fruitapp::gui::ce::post_model::connection_wrapper::connection()
{
	return
		*connection_;
}

fruitapp::gui::ce::post_model::connection_wrapper::~connection_wrapper()
{
}

fruitapp::gui::ce::post_model::post_model(
	fruitapp::highscore::provider_sequence &_providers)
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
fruitapp::gui::ce::post_model::message_received(
	fruitapp::highscore::callbacks::message_received const &f)
{
	return
		message_received_.connect(
			f);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::post_model::error_received(
	fruitapp::highscore::callbacks::error_received const &f)
{
	return
		error_received_.connect(
			f);
}

void
fruitapp::gui::ce::post_model::post(
	fruitapp::highscore::name const &_name,
	fruitapp::highscore::score const &_score)
{
	for(
		fruitapp::gui::ce::table::row_index::value_type i =
			static_cast<fruitapp::gui::ce::table::row_index::value_type>(connections_.size()-1);
		i != static_cast<fruitapp::gui::ce::table::row_index::value_type>(-1);
		--i)
		row_removed_(
			i);

	for(
		highscore::provider_sequence::iterator i =
			providers_.begin();
		i != providers_.end();
		++i)
	{
		fruitapp::highscore::provider::connection_base_ptr new_connection(
			(*i)->create_connection());

		fruitapp::gui::ce::table::row new_row;
		new_row.push_back(
			(*i)->identifier());
		new_row.push_back(
			FCPPT_TEXT(""));

		fruitapp::gui::ce::table::row_index::value_type const new_row_index =
			static_cast<fruitapp::gui::ce::table::row_index::value_type>(
				std::distance(
					providers_.begin(),
					i));

		row_added_(
			new_row_index,
			new_row);

		fcppt::signal::auto_connection con1(
			new_connection->message_received(
				fruitapp::highscore::callbacks::message_received{
					std::bind(
						&post_model::message_received_internal,
						this,
						std::cref(
							**i),
						std::placeholders::_1
					)
				}
			)
		);

		fcppt::signal::auto_connection con2(
			new_connection->error_received(
				fruitapp::highscore::callbacks::error_received{
					std::bind(
						&post_model::error_received_internal,
						this,
						std::cref(
							**i),
						std::placeholders::_1
					)
				}
			)
		);

		fcppt::signal::auto_connection con3(
			new_connection->rank_received(
				fruitapp::highscore::callbacks::rank_received{
					std::bind(
						&post_model::rank_received_internal,
						this,
						std::cref(
							**i),
						new_row_index,
						std::placeholders::_1
					)
				}
			)
		);

		connections_.push_back(
			fcppt::make_unique_ptr<connection_wrapper>(
				std::move(
					new_connection),
				std::move(
					con1),
				std::move(
					con2),
				std::move(
					con3)));

		connections_.back()->connection().post_rank(
			_name,
			_score);
	}
}

void
fruitapp::gui::ce::post_model::update()
{
	for(
		auto const &connection
		:
		connections_
	)
		connection->connection().update();
}

fruitapp::gui::ce::table::column_sequence const
fruitapp::gui::ce::post_model::columns() const
{
	return
		fcppt::assign::make_container<fruitapp::gui::ce::table::column_sequence>
			(fruitapp::gui::ce::table::column(
				FCPPT_TEXT("Provider"),
				static_cast<fruitapp::gui::ce::table::column::width_type>(
					0.9)))
			(fruitapp::gui::ce::table::column(
				FCPPT_TEXT("Rank"),
				static_cast<fruitapp::gui::ce::table::column::width_type>(
					0.1)));
}

fcppt::signal::auto_connection
fruitapp::gui::ce::post_model::row_added(
	fruitapp::gui::ce::table::row_added const &f)
{
	return
		row_added_.connect(
			f);
}

fcppt::signal::auto_connection
fruitapp::gui::ce::post_model::row_removed(
	fruitapp::gui::ce::table::row_removed const &f)
{
	return
		row_removed_.connect(
			f);
}

fruitapp::gui::ce::post_model::~post_model()
{
}

void
fruitapp::gui::ce::post_model::message_received_internal(
	fruitapp::highscore::provider::object_base const &_provider,
	fcppt::string const &_message)
{
	message_received_(
		_provider.identifier()+FCPPT_TEXT(": ")+_message);
}

void
fruitapp::gui::ce::post_model::error_received_internal(
	fruitapp::highscore::provider::object_base const &_provider,
	fcppt::string const &_message)
{
	error_received_(
		_provider.identifier()+FCPPT_TEXT(": ")+_message);
}

void
fruitapp::gui::ce::post_model::rank_received_internal(
	fruitapp::highscore::provider::object_base const &_provider,
	fruitapp::gui::ce::table::row_index::value_type const _row_index,
	fruitapp::highscore::rank const &_opt_rank)
{
	row_removed_(
		_row_index);

	fruitapp::gui::ce::table::row new_row;
	new_row.push_back(
		_provider.identifier());
	new_row.push_back(
		fcppt::optional::maybe(
			_opt_rank,
			[]{
				return
					fcppt::string(
						FCPPT_TEXT("-"));
			},
			[](
				unsigned const _rank
			)
			{
				return
					fcppt::insert_to_fcppt_string(
						_rank);
			}
		)
	);

	fruitapp::gui::ce::table::row_index::value_type const new_row_index =
		static_cast<fruitapp::gui::ce::table::row_index::value_type>(
			connections_.size());

	row_added_(
		new_row_index,
		new_row);
}
