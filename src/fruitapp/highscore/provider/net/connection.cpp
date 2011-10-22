#include <fruitapp/current_commit.hpp>
#include <fruitapp/highscore/json_to_entry_set.hpp>
#include <fruitapp/highscore/provider/net/connection.hpp>
#include <sge/parse/json/array.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/find_member.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/parse/json/parse_range.hpp>
#include <sge/parse/json/path.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/insert_to_fcppt_string.hpp>
#include <fcppt/insert_to_std_string.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/string.hpp>
#include <fcppt/to_std_string.hpp>
#include <fcppt/io/cerr.hpp>
#include <fcppt/io/istringstream.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/utf8/from_fcppt_string.hpp>
#include <fcppt/utf8/to_fcppt_string.hpp>
#include <fcppt/config/external_begin.hpp>
#include <iomanip>
#include <string>
#include <fcppt/config/external_end.hpp>


namespace
{
fcppt::optional<std::string::size_type>
parse_content_size(
	fcppt::string const &s)
{
	fcppt::io::istringstream ss(
		s);
	std::string::size_type result;
	if (!(ss >> std::hex >> std::setfill(FCPPT_TEXT('0')) >> std::setw(8) >> result))
		return fcppt::optional<std::string::size_type>();
	return result;
}
}

fruitapp::highscore::provider::net::connection::connection(
	net::host::value_type const &_host,
	net::port::value_type const &_port)
:
	host_(
		_host),
	port_(
		_port),
	message_received_(),
	error_received_(),
	list_received_(),
	rank_received_(),
	io_service_(),
	resolver_(
		io_service_),
	socket_(
		io_service_),
	request_(),
	content_()
{
}

void
fruitapp::highscore::provider::net::connection::post_rank(
	highscore::name const &name,
	highscore::score const &score)
{
	socket_.close();

	request_ =
		fcppt::utf8::from_fcppt_string(
			FCPPT_TEXT("P ")+
			fruitapp::current_commit()+
			FCPPT_TEXT(" ")+
			name.get()+
			FCPPT_TEXT(" ")+
			fcppt::insert_to_fcppt_string(
				score.get())+
			FCPPT_TEXT("\n"));

	message_received_(
		FCPPT_TEXT("Initiating connection to ")+
		host_+
		FCPPT_TEXT(", port ")+
		fcppt::insert_to_fcppt_string(
			port_)+
		FCPPT_TEXT("..."));

	boost::asio::ip::tcp::resolver::query query_(
		fcppt::to_std_string(
			host_),
		fcppt::insert_to_std_string(
			port_));

	resolver_.async_resolve(
		query_,
		std::tr1::bind(
			&connection::handle_resolve,
			this,
			// error
			std::tr1::placeholders::_1,
			// iterator
			std::tr1::placeholders::_2,
			// Why is this neccessary? :(
			json_handler(
				std::tr1::bind(
					&connection::handle_rank_received,
					this,
					std::tr1::placeholders::_1))));
}

void
fruitapp::highscore::provider::net::connection::retrieve_list()
{
	socket_.close();

	request_ =
		fcppt::utf8::from_fcppt_string(
			FCPPT_TEXT("G ")+
			fruitapp::current_commit()+
			FCPPT_TEXT("\n"));

	message_received_(
		FCPPT_TEXT("Initiating connection to ")+
		host_+
		FCPPT_TEXT(", port ")+
		fcppt::insert_to_fcppt_string(
			port_)+
		FCPPT_TEXT("..."));

	boost::asio::ip::tcp::resolver::query query_(
		fcppt::to_std_string(
			host_),
		fcppt::insert_to_std_string(
			port_));

	resolver_.async_resolve(
		query_,
		std::tr1::bind(
			&connection::handle_resolve,
			this,
			// error
			std::tr1::placeholders::_1,
			// iterator
			std::tr1::placeholders::_2,
			// Why is this neccessary? :(
			json_handler(
				std::tr1::bind(
					&connection::handle_list_received,
					this,
					std::tr1::placeholders::_1))));
}

void
fruitapp::highscore::provider::net::connection::update()
{
	io_service_.run_one();
}

fcppt::signal::auto_connection
fruitapp::highscore::provider::net::connection::message_received(
	callbacks::message_received const &f)
{
	return
		message_received_.connect(
			f);
}

fcppt::signal::auto_connection
fruitapp::highscore::provider::net::connection::error_received(
	callbacks::error_received const &f)
{
	return
		error_received_.connect(
			f);
}

fcppt::signal::auto_connection
fruitapp::highscore::provider::net::connection::list_received(
	callbacks::list_received const &f)
{
	return
		list_received_.connect(
			f);
}

fcppt::signal::auto_connection
fruitapp::highscore::provider::net::connection::rank_received(
	callbacks::rank_received const &f)
{
	return
		rank_received_.connect(
			f);
}

fruitapp::highscore::provider::net::connection::~connection()
{
}

void
fruitapp::highscore::provider::net::connection::handle_resolve(
	boost::system::error_code const &error,
	boost::asio::ip::tcp::resolver::iterator it,
	json_handler const &continue_here)
{
	if(error)
	{
		error_received_(
			fcppt::from_std_string(
				error.message()));
		return;
	}

	boost::asio::ip::tcp::endpoint const endpoint =
		*it;

	message_received_(
		FCPPT_TEXT("Resolved host, trying to connect via endpoint \"")+
		fcppt::insert_to_fcppt_string(
			endpoint)+
		FCPPT_TEXT("\"..."));

	socket_.async_connect(
		endpoint,
		std::tr1::bind(
			&connection::handle_connect,
			this,
			std::tr1::placeholders::_1,
			++it,
			continue_here));
}

void
fruitapp::highscore::provider::net::connection::handle_connect(
	boost::system::error_code const &error,
	boost::asio::ip::tcp::resolver::iterator it,
	json_handler const &continue_here)
{
	if (!error)
	{
		message_received_(
			FCPPT_TEXT("Connected successfully, sending request..."));

		boost::asio::async_write(
			socket_,
			boost::asio::buffer(
				&request_[0],
				request_.size()),
			std::tr1::bind(
				&connection::handle_write_request,
				this,
				std::tr1::placeholders::_1,
				continue_here));
	}
	else if (it != boost::asio::ip::tcp::resolver::iterator())
	{
		message_received_(
			FCPPT_TEXT("Attempt to connect was unsuccessful, trying the next endpoint..."));

		// The connection failed. Try the next endpoint in the list.
		socket_.close();
		boost::asio::ip::tcp::endpoint const endpoint =
			*it;
		socket_.async_connect(
			endpoint,
			std::tr1::bind(
				&connection::handle_connect,
				this,
				std::tr1::placeholders::_1,
				++it,
				continue_here));
	}
	else
	{
		error_received_(
			fcppt::from_std_string(
				error.message()));
	}
}

void
fruitapp::highscore::provider::net::connection::handle_write_request(
	boost::system::error_code const &error,
	json_handler const &continue_here)
{
	if(error)
	{
		error_received_(
			fcppt::from_std_string(
				error.message()));
		return;
	}

	message_received_(
		FCPPT_TEXT("Sending the request worked, now reading the number of bytes in the answer..."));

	content_.resize(
		8);

	boost::asio::async_read(
		socket_,
		boost::asio::buffer(
			&content_[0],
			content_.size()),
		std::tr1::bind(
			&connection::handle_read_size,
			this,
			std::tr1::placeholders::_1,
			continue_here));
}

void
fruitapp::highscore::provider::net::connection::handle_read_size(
	boost::system::error_code const &error,
	json_handler const &continue_here)
{
	if(error)
	{
		error_received_(
			fcppt::from_std_string(
				error.message()));
		return;
	}

	fcppt::optional<std::string::size_type> const content_size =
		parse_content_size(
			fcppt::utf8::to_fcppt_string(
				content_));

	if(!content_size)
	{
		error_received_(
			FCPPT_TEXT("Invalid content size: \"")+
			fcppt::utf8::to_fcppt_string(
				content_)+
			FCPPT_TEXT("\""));
		return;
	}

	message_received_(
		FCPPT_TEXT("The server sent a projected size of ")+
		fcppt::insert_to_fcppt_string(
			*content_size)+
		FCPPT_TEXT(" bytes, requesting those bytes..."));

	content_.resize(
		*content_size);

	boost::asio::async_read(
		socket_,
		boost::asio::buffer(
			&content_[0],
			content_.size()),
		std::tr1::bind(
			&connection::handle_read_content,
			this,
			std::tr1::placeholders::_1,
			continue_here));
}

void
fruitapp::highscore::provider::net::connection::handle_read_content(
	boost::system::error_code const &error,
	json_handler const &continue_here)
{
	if(error)
	{
		error_received_(
			fcppt::from_std_string(
				error.message()));
		return;
	}

	message_received_(
		FCPPT_TEXT("Received ")+
		fcppt::insert_to_fcppt_string(
			content_.size())+
		FCPPT_TEXT(" bytes, processing..."));

	fcppt::string const content_converted =
		fcppt::utf8::to_fcppt_string(
			content_);

	fcppt::string::const_iterator current =
		content_converted.begin();

	sge::parse::json::object result;
	if(!sge::parse::json::parse_range(current,content_converted.end(),result))
	{
		fcppt::io::cerr()
			<< FCPPT_TEXT("The application crashed, the content was ")
			<< content_converted
			<< FCPPT_TEXT("\n");

		error_received_(
			FCPPT_TEXT("Couldn't parse content..."));

		return;
	}

	fcppt::string const * const json_error =
		sge::parse::json::find_member<fcppt::string>(
			result.members,
			FCPPT_TEXT("error"));

	if(json_error)
	{
		error_received_(
			FCPPT_TEXT("The server signaled an error: ")+
			(*json_error));
		return;
	}

	continue_here(
		result);
}

void
fruitapp::highscore::provider::net::connection::handle_list_received(
	sge::parse::json::object const &result)
{
	list_received_(
		highscore::json_to_entry_set(
			result));
}

void
fruitapp::highscore::provider::net::connection::handle_rank_received(
	sge::parse::json::object const &o)
{
	rank_received_(
		highscore::rank(
			sge::parse::json::find_and_convert_member<highscore::rank::value_type>(
				o,
				sge::parse::json::path(
					FCPPT_TEXT("rank")))));
}
