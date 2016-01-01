#include <fruitlib/time_format/string_to_duration.hpp>
#include <fcppt/optional/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <chrono>
#include <iostream>
#include <string>
#include <fcppt/config/external_end.hpp>


int main()
{
	fcppt::optional::object<std::chrono::seconds> const result =
		fruitlib::time_format::string_to_duration<std::chrono::seconds>(
			std::wstring(L"1h"));

	if (!result)
		std::cout << "!result\n";
	else
		std::cout
			<<
				result->count()
			<< "\n";
}

