#ifndef FRUITCUT_APP_HIGHSCORE_PROVIDER_FILE_CONNECTION_HPP_INCLUDED
#define FRUITCUT_APP_HIGHSCORE_PROVIDER_FILE_CONNECTION_HPP_INCLUDED

#include "../../name.hpp"
#include "../../score.hpp"
#include "../connection_base.hpp"
#include "../../callbacks/message_received_fn.hpp"
#include "../../callbacks/message_received.hpp"
#include "../../callbacks/error_received_fn.hpp"
#include "../../callbacks/error_received.hpp"
#include "../../callbacks/list_received_fn.hpp"
#include "../../callbacks/list_received.hpp"
#include "../../callbacks/rank_received_fn.hpp"
#include "../../callbacks/rank_received.hpp"
#include <fcppt/signal/object.hpp>
#include <fcppt/filesystem/path.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitcut
{
namespace app
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
		fcppt::filesystem::path const &);

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
	fcppt::filesystem::path const path_;
	fcppt::signal::object<callbacks::message_received_fn> message_received_;
	fcppt::signal::object<callbacks::error_received_fn> error_received_;
	fcppt::signal::object<callbacks::list_received_fn> list_received_;
	fcppt::signal::object<callbacks::rank_received_fn> rank_received_;
};
}
}
}
}
}

#endif
