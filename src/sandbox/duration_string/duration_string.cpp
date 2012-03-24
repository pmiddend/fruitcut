#include <fruitlib/time_format/string_to_duration.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/chrono/duration.hpp>
#include <iostream>
#include <string>
#include <fcppt/config/external_end.hpp>


int main()
{
	fcppt::optional<boost::chrono::seconds> const result =
		fruitlib::time_format::string_to_duration<boost::chrono::seconds>(
			std::wstring(L"1h"));

	if (!result)
		std::cout << "!result\n";
	else
		std::cout
			<<
				result->count()
			<< "\n";
}

