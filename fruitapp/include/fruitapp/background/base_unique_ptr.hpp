#ifndef FRUITAPP_BACKGROUND_BASE_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_BASE_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/background/base_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace background
{
typedef
std::unique_ptr<fruitapp::background::base>
base_unique_ptr;
}
}

#endif
