#ifndef FRUITAPP_FRUIT_RENDERING_BASE_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_FRUIT_RENDERING_BASE_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/fruit/rendering/base_fwd.hpp>
#include <fcppt/config/external_begin.hpp>
#include <memory>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
namespace rendering
{
typedef
std::unique_ptr<fruitapp::fruit::rendering::base>
base_unique_ptr;
}
}
}

#endif
