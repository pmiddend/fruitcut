#ifndef FRUITAPP_CONFIG_VARIABLES_AUDIO_VARIABLE_HPP_INCLUDED
#define FRUITAPP_CONFIG_VARIABLES_AUDIO_VARIABLE_HPP_INCLUDED

#include <sge/audio/scalar.hpp>
#include <sge/parse/json/config/user_config_variable_fwd.hpp>


namespace fruitapp
{
namespace config_variables
{
typedef
sge::parse::json::config::user_config_variable<sge::audio::scalar>
audio_variable;
}
}

#endif
