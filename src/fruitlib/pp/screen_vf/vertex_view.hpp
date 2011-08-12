#ifndef FRUITLIB_PP_SCREEN_VF_VERTEX_VIEW_HPP_INCLUDED
#define FRUITLIB_PP_SCREEN_VF_VERTEX_VIEW_HPP_INCLUDED

#include <fruitlib/pp/screen_vf/part.hpp>
#include <sge/renderer/vf/view.hpp>

namespace fruitlib
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
