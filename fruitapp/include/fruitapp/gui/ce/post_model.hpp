#ifndef FRUITAPP_GUI_CE_POST_MODEL_HPP_INCLUDED
#define FRUITAPP_GUI_CE_POST_MODEL_HPP_INCLUDED

#include <fruitapp/gui/ce/table/column_sequence.hpp>
#include <fruitapp/gui/ce/table/model.hpp>
#include <fruitapp/gui/ce/table/row_added.hpp>
#include <fruitapp/gui/ce/table/row_added_fn.hpp>
#include <fruitapp/gui/ce/table/row_index.hpp>
#include <fruitapp/gui/ce/table/row_removed.hpp>
#include <fruitapp/gui/ce/table/row_removed_fn.hpp>
#include <fruitapp/highscore/name.hpp>
#include <fruitapp/highscore/provider_sequence.hpp>
#include <fruitapp/highscore/rank.hpp>
#include <fruitapp/highscore/score.hpp>
#include <fruitapp/highscore/callbacks/error_received.hpp>
#include <fruitapp/highscore/callbacks/error_received_fn.hpp>
#include <fruitapp/highscore/callbacks/message_received.hpp>
#include <fruitapp/highscore/callbacks/message_received_fn.hpp>
#include <fruitapp/highscore/callbacks/rank_received.hpp>
#include <fruitapp/highscore/provider/connection_base_fwd.hpp>
#include <fruitapp/highscore/provider/connection_base_ptr.hpp>
#include <fruitapp/highscore/provider/object_base_fwd.hpp>
#include <fcppt/string.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/shared_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <fcppt/config/external_end.hpp>

namespace fruitapp
{
namespace gui
{
namespace ce
{
class post_model
:
	public fruitapp::gui::ce::table::model
{
FCPPT_NONCOPYABLE(
	post_model);
public:
	explicit
	post_model(
		fruitapp::highscore::provider_sequence &);

	fcppt::signal::auto_connection
	message_received(
		fruitapp::highscore::callbacks::message_received const &);

	fcppt::signal::auto_connection
	error_received(
		fruitapp::highscore::callbacks::error_received const &);

	void
	post(
		fruitapp::highscore::name const &,
		fruitapp::highscore::score const &);

	void
	update();

	fruitapp::gui::ce::table::column_sequence const
	columns() const;

	fcppt::signal::auto_connection
	row_added(
		fruitapp::gui::ce::table::row_added const &);

	fcppt::signal::auto_connection
	row_removed(
		fruitapp::gui::ce::table::row_removed const &);

	~post_model();
private:
	class connection_wrapper
	{
	FCPPT_NONCOPYABLE(
		connection_wrapper);
	public:
		explicit
		connection_wrapper(
			fruitapp::highscore::provider::connection_base_ptr &,
			fcppt::signal::shared_connection,
			fcppt::signal::shared_connection,
			fcppt::signal::shared_connection);

		fruitapp::highscore::provider::connection_base &
		connection();

		~connection_wrapper();
	private:
		fruitapp::highscore::provider::connection_base_ptr connection_;
		fcppt::signal::shared_connection const message_received_;
		fcppt::signal::shared_connection const error_received_;
		fcppt::signal::shared_connection const rank_received_;
	};

	typedef
	boost::ptr_vector<connection_wrapper>
	connection_sequence;

	fruitapp::highscore::provider_sequence &providers_;
	fcppt::signal::object<fruitapp::highscore::callbacks::message_received_fn> message_received_;
	fcppt::signal::object<fruitapp::highscore::callbacks::error_received_fn> error_received_;
	fcppt::signal::object<fruitapp::gui::ce::table::row_added_fn> row_added_;
	fcppt::signal::object<fruitapp::gui::ce::table::row_removed_fn> row_removed_;
	connection_sequence connections_;

	void
	message_received_internal(
		fruitapp::highscore::provider::object_base const &,
		fcppt::string const &);

	void
	error_received_internal(
		fruitapp::highscore::provider::object_base const &,
		fcppt::string const &);

	void
	rank_received_internal(
		fruitapp::highscore::provider::object_base const &,
		fruitapp::gui::ce::table::row_index::value_type,
		fruitapp::highscore::rank const &);
};
}
}
}

#endif
