#ifndef FRUITAPP_CONFIG_VARIABLES_GLOBAL_CONFIG_REF_HPP_INCLUDED
#define FRUITAPP_CONFIG_VARIABLES_GLOBAL_CONFIG_REF_HPP_INCLUDED

#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/reference_wrapper.hpp>
#include <fcppt/strong_typedef.hpp>

namespace fruitapp
{
namespace config_variables
{
FCPPT_MAKE_STRONG_TYPEDEF(
	fcppt::reference_wrapper<
		sge::parse::json::object const
	>,
	global_config_ref);
}
}

#endif
