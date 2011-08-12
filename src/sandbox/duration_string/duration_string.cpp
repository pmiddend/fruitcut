#include <fruitlib/time_format/string_to_duration.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/chrono/seconds.hpp>
#include <iostream>
#include <string>

int main()
{
	fcppt::optional<fcppt::chrono::seconds> const result =
		fruitlib::time_format::string_to_duration<fcppt::chrono::seconds>(
			std::wstring(L"1d"));

	if (!result)
		std::cout << "!result\n";
	else
		std::cout
			<<
				result->count()
			<< "\n";
}

