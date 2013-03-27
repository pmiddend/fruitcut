#ifndef FRUITAPP_HIGHSCORE_PROVIDER_FILE_CONNECTION_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_PROVIDER_FILE_CONNECTION_HPP_INCLUDED

#include <fruitapp/highscore/name.hpp>
#include <fruitapp/highscore/score.hpp>
#include <fruitapp/highscore/callbacks/error_received.hpp>
#include <fruitapp/highscore/callbacks/error_received_fn.hpp>
#include <fruitapp/highscore/callbacks/list_received.hpp>
#include <fruitapp/highscore/callbacks/list_received_fn.hpp>
#include <fruitapp/highscore/callbacks/message_received.hpp>
#include <fruitapp/highscore/callbacks/message_received_fn.hpp>
#include <fruitapp/highscore/callbacks/rank_received.hpp>
#include <fruitapp/highscore/callbacks/rank_received_fn.hpp>
#include <fruitapp/highscore/provider/connection_base.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/filesystem/path.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace highscore
{
namespace provider
{
namespace file
{
class connection
:
	public provider::connection_base
{
FCPPT_NONCOPYABLE(
	connection);
public:
	explicit
	connection(
		boost::filesystem::path const &);

	void
	post_rank(
		highscore::name const &,
		highscore::score const &);

	void
	retrieve_list();

	void
	update();

	fcppt::signal::auto_connection
	message_received(
		callbacks::message_received const &);

	fcppt::signal::auto_connection
	error_received(
		callbacks::error_received const &);

	fcppt::signal::auto_connection
	list_received(
		callbacks::list_received const &);

	fcppt::signal::auto_connection
	rank_received(
		callbacks::rank_received const &);

	~connection();
private:
	boost::filesystem::path const path_;
	fcppt::signal::object<callbacks::message_received_fn> message_received_;
	fcppt::signal::object<callbacks::error_received_fn> error_received_;
	fcppt::signal::object<callbacks::list_received_fn> list_received_;
	fcppt::signal::object<callbacks::rank_received_fn> rank_received_;
};
}
}
}
}

#endif
