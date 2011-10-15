#ifndef FRUITAPP_HIGHSCORE_PROVIDER_CONNECTION_BASE_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_PROVIDER_CONNECTION_BASE_HPP_INCLUDED

#include <fruitapp/highscore/name.hpp>
#include <fruitapp/highscore/score.hpp>
#include <fruitapp/highscore/callbacks/message_received.hpp>
#include <fruitapp/highscore/callbacks/error_received.hpp>
#include <fruitapp/highscore/callbacks/list_received.hpp>
#include <fruitapp/highscore/callbacks/rank_received.hpp>
#include <fcppt/signal/auto_connection.hpp>

namespace fruitapp
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
		highscore::score const &) = 0;

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

#endif
