#ifndef FRUITCUT_APP_FRUIT_PROJECTED_HULL_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_PROJECTED_HULL_HPP_INCLUDED

#include "hull_point_sequence.hpp"
#include "fruit_fwd.hpp"
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/target_base_ptr.hpp>

namespace fruitcut
{
namespace app
{
hull_point_sequence const
fruit_projected_hull(
	fruit const &,
	sge::renderer::target_base_ptr,
	sge::renderer::matrix4 const &);
}
}

#endif
