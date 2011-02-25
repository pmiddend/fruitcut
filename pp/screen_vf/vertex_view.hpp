#ifndef FRUITCUT_PP_SCREEN_VF_VERTEX_VIEW_HPP_INCLUDED
#define FRUITCUT_PP_SCREEN_VF_VERTEX_VIEW_HPP_INCLUDED

#include "part.hpp"
#include <sge/renderer/vf/view.hpp>

namespace fruitcut
{
namespace pp
{
namespace screen_vf
{
typedef 
sge::renderer::vf::view
<
	part
> 
vertex_view;
}
}
}

#endif
