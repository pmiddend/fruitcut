#ifndef FRUITAPP_BACKGROUND_VF_VERTEX_VIEW_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_VF_VERTEX_VIEW_HPP_INCLUDED

#include <fruitapp/background/vf/part.hpp>
#include <sge/renderer/vf/view.hpp>


namespace fruitapp
{
namespace background
{
namespace vf
{
typedef
sge::renderer::vf::view
<
	fruitapp::background::vf::part
>
vertex_view;
}
}
}

#endif
