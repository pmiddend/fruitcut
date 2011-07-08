#ifndef FRUITCUT_APP_HIGHSCORE_PROVIDERS_FROM_JSON_HPP_INCLUDED
#define FRUITCUT_APP_HIGHSCORE_PROVIDERS_FROM_JSON_HPP_INCLUDED

#include <sge/parse/json/object_fwd.hpp>
#include "provider_sequence.hpp"

namespace fruitcut
{
namespace app
{
namespace highscore
{
void
providers_from_json(
	sge::parse::json::object const &config_file,
	highscore::provider_sequence &);
}
}
}

#endif
