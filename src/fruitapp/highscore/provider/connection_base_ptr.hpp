#ifndef FRUITAPP_HIGHSCORE_PROVIDER_CONNECTION_BASE_PTR_HPP_INCLUDED
#define FRUITAPP_HIGHSCORE_PROVIDER_CONNECTION_BASE_PTR_HPP_INCLUDED

#include <fruitapp/highscore/provider/connection_base_fwd.hpp>
#include <fcppt/unique_ptr.hpp>

namespace fruitapp
{
namespace highscore
{
namespace provider
{
typedef
fcppt::unique_ptr<provider::connection_base>
connection_base_ptr;
}
}
}

#endif
