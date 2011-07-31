#include "output_tm.hpp"
#include <iterator>
#include <locale>
#include <ostream>
#include <ctime>

std::ostream &
fruitserver::output_tm(
	std::ostream &_stream,
	std::tm const &_tm)
{
	std::ostream::sentry cerberos(
		_stream);

	if(
		!cerberos
	)
		return _stream;

	std::use_facet
	<
		std::time_put
		<
			char,
			std::ostreambuf_iterator
			<
				char,
				std::ostream::traits_type
			>
		>
	>(
		_stream.getloc()
	).put(
		_stream,
		_stream,
		_stream.fill(),
		&_tm,
		'c');

	return 
		_stream;
}
