#ifndef FRUITCUT_APP_MODEL_VF_PART_HPP_INCLUDED
#define FRUITCUT_APP_MODEL_VF_PART_HPP_INCLUDED

#include "position.hpp"
#include "texcoord.hpp"
#include <sge/renderer/vf/part.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
{
namespace model_vf
{
typedef 
sge::renderer::vf::part
<
	boost::mpl::vector2
	<
		position,
		texcoord
	>
> 
part;
}
}
}

#endif
