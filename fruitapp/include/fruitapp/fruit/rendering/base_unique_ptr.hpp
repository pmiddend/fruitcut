#ifndef FRUITAPP_FRUIT_RENDERING_BASE_UNIQUE_PTR_HPP_INCLUDED
#define FRUITAPP_FRUIT_RENDERING_BASE_UNIQUE_PTR_HPP_INCLUDED

#include <fruitapp/fruit/rendering/base_fwd.hpp>
#include <fcppt/unique_ptr_impl.hpp>


namespace fruitapp
{
namespace fruit
{
namespace rendering
{
typedef
fcppt::unique_ptr<fruitapp::fruit::rendering::base>
base_unique_ptr;
}
}
}

#endif
