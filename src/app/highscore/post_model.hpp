#ifndef FRUITCUT_APP_HIGHSCORE_POST_MODEL_HPP_INCLUDED
#define FRUITCUT_APP_HIGHSCORE_POST_MODEL_HPP_INCLUDED

#include "callbacks/message_received.hpp"
#include "callbacks/message_received_fn.hpp"
#include "callbacks/error_received.hpp"
#include "callbacks/error_received_fn.hpp"
#include "callbacks/rank_received.hpp"
#include "name.hpp"
#include "rank.hpp"
#include "provider_sequence.hpp"
#include "provider/connection_base_fwd.hpp"
#include "provider/connection_base_ptr.hpp"
#include "provider/object_base_fwd.hpp"
#include "score.hpp"
#include "../gui/table/model.hpp"
#include "../gui/table/row_added.hpp"
#include "../gui/table/row_added_fn.hpp"
#include "../gui/table/row_removed.hpp"
#include "../gui/table/row_removed_fn.hpp"
#include "../gui/table/column_sequence.hpp"
#include "../gui/table/row_index.hpp"
#include <boost/ptr_container/ptr_vector.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/shared_connection.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/string.hpp>

namespace fruitcut
{
namespace app
{
namespace highscore
{
class post_model
:
	public gui::table::model
{
FCPPT_NONCOPYABLE(
	post_model);
public:
	explicit
	post_model(
		highscore::provider_sequence &);

	fcppt::signal::auto_connection
	message_received(
		callbacks::message_received const &);

	fcppt::signal::auto_connection
	error_received(
		callbacks::error_received const &);

	void
	post(
		highscore::name const &,
		highscore::score const &);

	void
	update();

	gui::table::column_sequence const
	columns() const;

	fcppt::signal::auto_connection
	row_added(
		gui::table::row_added const &);

	fcppt::signal::auto_connection
	row_removed(
		gui::table::row_removed const &);

	~post_model();
private:
	class connection_wrapper
	{
	FCPPT_NONCOPYABLE(
		connection_wrapper);
	public:
		explicit
		connection_wrapper(
			provider::connection_base_ptr,
			fcppt::signal::shared_connection,
			fcppt::signal::shared_connection,
			fcppt::signal::shared_connection);

		provider::connection_base &
		connection();

		~connection_wrapper();
	private:
		provider::connection_base_ptr	connection_;
		fcppt::signal::shared_connection const message_received_;
		fcppt::signal::shared_connection const error_received_;
		fcppt::signal::shared_connection const rank_received_;
	};

	typedef
	boost::ptr_vector<connection_wrapper>
	connection_sequence;

	highscore::provider_sequence &providers_;
	fcppt::signal::object<callbacks::message_received_fn> message_received_;
	fcppt::signal::object<callbacks::error_received_fn> error_received_;
	fcppt::signal::object<gui::table::row_added_fn> row_added_;
	fcppt::signal::object<gui::table::row_removed_fn> row_removed_;
	connection_sequence connections_;

	void
	message_received_internal(
		provider::object_base const &,
		fcppt::string const &);

	void
	error_received_internal(
		provider::object_base const &,
		fcppt::string const &);

	void
	rank_received_internal(
		provider::object_base const &,
		gui::table::row_index::value_type,
		highscore::rank const &);
};
}
}
}

#endif