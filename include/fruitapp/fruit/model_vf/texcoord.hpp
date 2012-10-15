#ifndef FRUITAPP_FRUIT_MODEL_VF_TEXCOORD_HPP_INCLUDED
#define FRUITAPP_FRUIT_MODEL_VF_TEXCOORD_HPP_INCLUDED

#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vf/texpos.hpp>
#include <sge/renderer/vf/index.hpp>

namespace fruitapp
{
namespace fruit
{
namespace model_vf
{
typedef
sge::renderer::vf::texpos
<
	sge::renderer::scalar,
	2,
	sge::renderer::vf::index<0u>
>
texcoord;
}
}
}

#endif
