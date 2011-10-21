#ifndef FRUITAPP_HIGHSCORE_PROVIDERS_FROM_JSON_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_PROVIDERS_FROM_JSON_HPP_INCLUDED

#include <fruitapp/highscore/provider_sequence.hpp>
#include <sge/parse/json/object_fwd.hpp>


namespace fruitapp
{
namespace highscore
{
void
providers_from_json(
	sge::parse::json::object const &config_file,
	highscore::provider_sequence &);
}
}

#endif
