#ifndef FRUITAPP_LOAD_USER_CONFIG_HPP_INCLUDED
#define FRUITAPP_LOAD_USER_CONFIG_HPP_INCLUDED

#include <sge/charconv/system_fwd.hpp>
#include <sge/parse/json/object.hpp>

namespace fruitapp
{
sge::parse::json::object const
load_user_config(
	sge::charconv::system &);
}

#endif
