#ifndef FRUITAPP_HIGHSCORE_PROVIDERS_FROM_JSON_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_PROVIDERS_FROM_JSON_HPP_INCLUDED

#include <fruitapp/highscore/provider_sequence.hpp>
#include <sge/parse/json/array_fwd.hpp>
#include <sge/charconv/system_fwd.hpp>


namespace fruitapp
{
namespace highscore
{
void
providers_from_json(
	sge::charconv::system &,
	sge::parse::json::array const &,
	highscore::provider_sequence &);
}
}

#endif
