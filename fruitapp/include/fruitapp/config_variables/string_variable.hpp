#ifndef FRUITAPP_CONFIG_VARIABLES_STRING_VARIABLE_HPP_INCLUDED
#define FRUITAPP_CONFIG_VARIABLES_STRING_VARIABLE_HPP_INCLUDED

#include <sge/parse/json/config/user_config_variable_fwd.hpp>
#include <fcppt/string.hpp>

namespace fruitapp
{
namespace config_variables
{
typedef
sge::parse::json::config::user_config_variable<fcppt::string>
string_variable;
}
}

#endif
