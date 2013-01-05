#ifndef FRUITAPP_POSTPROCESSING_SYSTEM_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_POSTPROCESSING_SYSTEM_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/postprocessing/system_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace postprocessing
{
typedef
std::unique_ptr<fruitapp::postprocessing::system>
system_unique_ptr;
}
}

#endif
