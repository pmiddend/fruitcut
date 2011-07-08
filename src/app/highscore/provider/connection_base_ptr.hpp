#ifndef FRUITCUT_APP_HIGHSCORE_PROVIDER_CONNECTION_BASE_PTR_HPP_INCLUDED
#define FRUITCUT_APP_HIGHSCORE_PROVIDER_CONNECTION_BASE_PTR_HPP_INCLUDED

#include "connection_base_fwd.hpp"
#include <fcppt/unique_ptr.hpp>

namespace fruitcut
{
namespace app
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
}

#endif
