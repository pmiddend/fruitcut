#include "getenv.hpp"
#include <fcppt/config.hpp>
#ifdef FCPPT_WINDOWS_PLATFORM
#include <awl/backends/windows/windows.hpp>
#include <fcppt/container/raw_vector_impl.hpp>
#include <fcppt/char_type.hpp>
#include <fcppt/text.hpp>
#else
#include <fcppt/from_std_string.hpp>
#include <cstdlib>
#endif

fruitcut::environment::optional_string const
fruitcut::environment::getenv(
	fcppt::string const &_name)
{
#ifdef FCPPT_WINDOWS_PLATFORM
	fcppt::container::raw_vector<fcppt::char_type> buffer(
		32767);

	return
		::GetEnvironmentVariable(
			_name.c_str(),
			buffer.data(),
			buffer.size()
		) == 0
		?
			optional_string()
		:
			optional_string(
				buffer.data());
#else
	char const *const ret(
		::std::getenv(
			fcppt::from_std_string(
				_name).c_str())); 
	return 
		ret
		?
			optional_string(
				fcppt::from_std_string(
					ret))
		:
			optional_string();
#endif
}
