#ifndef FRUITCUT_APP_FRUIT_PLANE_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_PLANE_HPP_INCLUDED

#include "../../fruitlib/math/plane/basic.hpp"
#include <sge/renderer/scalar.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
typedef
fruitlib::math::plane::basic<sge::renderer::scalar,3>
plane;
}
}
}

#endif
