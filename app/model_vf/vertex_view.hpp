#ifndef FRUITCUT_APP_MODEL_VF_VERTEX_VIEW_HPP_INCLUDED
#define FRUITCUT_APP_MODEL_VF_VERTEX_VIEW_HPP_INCLUDED

#include "part.hpp"
#include <sge/renderer/vf/view.hpp>

namespace fruitcut
{
namespace app
{
namespace model_vf
{
typedef
sge::renderer::vf::view<part>
vertex_view;
}
}
}

#endif
