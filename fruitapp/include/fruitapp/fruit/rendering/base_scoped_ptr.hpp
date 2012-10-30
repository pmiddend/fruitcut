#ifndef FRUITAPP_FRUIT_RENDERING_BASE_SCOPED_PTR_HPP_INCLUDED
#define FRUITAPP_FRUIT_RENDERING_BASE_SCOPED_PTR_HPP_INCLUDED

#include <fruitapp/fruit/rendering/base_fwd.hpp>
#include <fcppt/scoped_ptr.hpp>

namespace fruitapp
{
namespace fruit
{
namespace rendering
{
typedef
fcppt::scoped_ptr<fruitapp::fruit::rendering::base>
base_scoped_ptr;
}
}
}

#endif
