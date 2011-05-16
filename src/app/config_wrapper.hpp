#ifndef FRUITCUT_APP_CONFIG_WRAPPER_HPP_INCLUDED
#define FRUITCUT_APP_CONFIG_WRAPPER_HPP_INCLUDED

#include <sge/parse/json/object.hpp>

namespace fruitcut
{
namespace app
{
// This function wraps the ever-repeating "read config file(s), add
// options from the console" part. Note that this function "includes"
// the config.json by default
sge::parse::json::object const
config_wrapper(
	int argc,
	char *argv[]);
}
}

#endif