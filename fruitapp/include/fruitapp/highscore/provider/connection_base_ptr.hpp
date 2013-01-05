#ifndef FRUITAPP_HIGHSCORE_PROVIDER_CONNECTION_BASE_PTR_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_PROVIDER_CONNECTION_BASE_PTR_HPP_INCLUDED

#include <fruitapp/highscore/provider/connection_base_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace highscore
{
namespace provider
{
typedef
std::unique_ptr<provider::connection_base>
connection_base_ptr;
}
}
}

#endif
