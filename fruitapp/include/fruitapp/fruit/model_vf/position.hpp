#ifndef FRUITAPP_FRUIT_MODEL_VF_POSITION_HPP_INCLUDED
#define FRUITAPP_FRUIT_MODEL_VF_POSITION_HPP_INCLUDED

#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vf/pos.hpp>

namespace fruitapp
{
namespace fruit
{
namespace model_vf
{
typedef
sge::renderer::vf::pos
<
	sge::renderer::scalar,
	3
>
position;
}
}
}

#endif
