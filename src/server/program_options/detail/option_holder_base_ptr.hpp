#ifndef FRUITCUT_SERVER_PROGRAM_OPTIONS_DETAIL_OPTION_HOLDER_BASE_PTR_HPP_INCLUDED
#define FRUITCUT_SERVER_PROGRAM_OPTIONS_DETAIL_OPTION_HOLDER_BASE_PTR_HPP_INCLUDED

#include "option_holder_base_fwd.hpp"
#include <tr1/memory>

namespace fruitcut
{
namespace server
{
namespace program_options
{
namespace detail
{
typedef
std::tr1::shared_ptr<option_holder_base>
option_holder_base_ptr;
}
}
}
}

#endif
