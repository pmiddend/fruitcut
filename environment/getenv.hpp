#ifndef FRUITCUT_ENVIRONMENT_GETENV_HPP_INCLUDED
#define FRUITCUT_ENVIRONMENT_GETENV_HPP_INCLUDED

#include "optional_string.hpp"
#include <fcppt/string.hpp>

namespace fruitcut
{
namespace environment
{
optional_string const
getenv(
	fcppt::string const &);
}
}

#endif
