#ifndef FRUITCUT_APP_FRUIT_HULL_PROJECTED_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_HULL_PROJECTED_HPP_INCLUDED

#include "ring.hpp"
#include "../object_fwd.hpp"
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/target_base_fwd.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
namespace hull
{
ring const
projected(
	object const &,
	sge::renderer::target_base &,
	sge::renderer::matrix4 const &);
}
}
}
}

#endif
