#ifndef FRUITSERVER_PROGRAM_OPTIONS_DETAIL_OPTION_HOLDER_BASE_PTR_HPP_INCLUDED
#define FRUITSERVER_PROGRAM_OPTIONS_DETAIL_OPTION_HOLDER_BASE_PTR_HPP_INCLUDED

#include <tr1/memory>
#include <fruitserver/program_options/detail/option_holder_base_fwd.hpp>


namespace fruitserver
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

#endif
