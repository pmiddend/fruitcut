#ifndef FRUITCUT_APP_FRUIT_MODEL_VF_PART_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_MODEL_VF_PART_HPP_INCLUDED

#include "position.hpp"
#include "texcoord.hpp"
#include "normal.hpp"
#include <sge/renderer/vf/part.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
namespace model_vf
{
typedef 
sge::renderer::vf::part
<
	boost::mpl::vector3
	<
		position,
		texcoord,
		normal
	>
> 
part;
}
}
}
}

#endif
