#ifndef FRUITCUT_APP_HIGHSCORE_PROVIDER_NET_CONNECTION_HPP_INCLUDED
#define FRUITCUT_APP_HIGHSCORE_PROVIDER_NET_CONNECTION_HPP_INCLUDED

#include "port.hpp"
#include "host.hpp"
#include "../connection_base.hpp"
#include "../../callbacks/message_received_fn.hpp"
#include "../../callbacks/message_received.hpp"
#include "../../callbacks/error_received_fn.hpp"
#include "../../callbacks/error_received.hpp"
#include "../../callbacks/list_received_fn.hpp"
#include "../../callbacks/list_received.hpp"
#include "../../callbacks/rank_received_fn.hpp"
#include "../../callbacks/rank_received.hpp"
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/function/object.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/utf8/string.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/asio.hpp>
#include <string>

namespace fruitcut
{
namespace app
{
namespace highscore
{
namespace provider
{
namespace net
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
		net::host::value_type const &,
		net::port::value_type const &);

	void
	post_rank(
		highscore::name const &,
		app::score const &);

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
	net::host::value_type host_;
	net::port::value_type port_;
	fcppt::signal::object<callbacks::message_received_fn> message_received_;
	fcppt::signal::object<callbacks::error_received_fn> error_received_;
	fcppt::signal::object<callbacks::list_received_fn> list_received_;
	fcppt::signal::object<callbacks::rank_received_fn> rank_received_;
	boost::asio::io_service io_service_;
	boost::asio::ip::tcp::resolver resolver_;
	boost::asio::ip::tcp::socket socket_;
	fcppt::utf8::string request_;
	boost::asio::streambuf size_response_;
	fcppt::utf8::string content_;

	typedef
	fcppt::function::object<void(sge::parse::json::object const &)>
	json_handler;

	void 
	handle_resolve(
		boost::system::error_code const &,
		boost::asio::ip::tcp::resolver::iterator,
		json_handler const &);

	void 
	handle_connect(
		boost::system::error_code const &,
		boost::asio::ip::tcp::resolver::iterator,
		json_handler const &);

	void
	handle_write_request(
		boost::system::error_code const &,
		json_handler const &);

	void
	handle_read_size(
		boost::system::error_code const &,
		json_handler const &);

	void
	handle_read_content(
		boost::system::error_code const &,
		json_handler const &);

	void
	handle_list_received(
		sge::parse::json::object const &);

	void
	handle_rank_received(
		sge::parse::json::object const &);
};
}
}
}
}
}

#endif
