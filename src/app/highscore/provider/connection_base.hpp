#ifndef FRUITCUT_APP_HIGHSCORE_PROVIDER_CONNECTION_BASE_HPP_INCLUDED
#define FRUITCUT_APP_HIGHSCORE_PROVIDER_CONNECTION_BASE_HPP_INCLUDED

#include "../name.hpp"
#include "../../score.hpp"
#include "../callbacks/message_received.hpp"
#include "../callbacks/error_received.hpp"
#include "../callbacks/list_received.hpp"
#include "../callbacks/rank_received.hpp"
#include <fcppt/signal/auto_connection.hpp>

namespace fruitcut
{
namespace app
{
namespace highscore
{
namespace provider
{
class connection_base
{
public:
	virtual void
	post_rank(
		highscore::name const &,
		app::score const &) = 0;

	virtual void
	retrieve_list() = 0;

	virtual void
	update() = 0;

	virtual fcppt::signal::auto_connection
	message_received(
		callbacks::message_received const &) = 0;

	virtual fcppt::signal::auto_connection
	error_received(
		callbacks::error_received const &) = 0;

	virtual fcppt::signal::auto_connection
	list_received(
		callbacks::list_received const &) = 0;

	virtual fcppt::signal::auto_connection
	rank_received(
		callbacks::rank_received const &) = 0;

	virtual ~connection_base();
protected:
	explicit
	connection_base();
};
}
}
}
}

#endif
