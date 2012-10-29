#ifndef FRUITAPP_BACKGROUND_VF_POSITION_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_VF_POSITION_HPP_INCLUDED

#include <sge/renderer/vf/pos.hpp>
#include <sge/renderer/scalar.hpp>

namespace fruitapp
{
namespace background
{
namespace vf
{
typedef
sge::renderer::vf::pos
<
	sge::renderer::scalar,
	2
>
position;
}
}
}

#endif
