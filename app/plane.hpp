#ifndef FRUITCUT_APP_PLANE_HPP_INCLUDED
#define FRUITCUT_APP_PLANE_HPP_INCLUDED

#include "../math/plane/basic.hpp"
#include <sge/renderer/scalar.hpp>

namespace fruitcut
{
namespace app
{
typedef
math::plane::basic<sge::renderer::scalar,3>
plane;
}
}

#endif
