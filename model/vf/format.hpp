#ifndef FRUITCUT_MODEL_VF_FORMAT_HPP_INCLUDED
#define FRUITCUT_MODEL_VF_FORMAT_HPP_INCLUDED

#include "position.hpp"
#include "texcoord.hpp"
#include <sge/renderer/vf/format.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace model
{
namespace vf
{
typedef 
sge::renderer::vf::format
<
	boost::mpl::vector2
	<
		position,
		texcoord
	>
> 
format;
}
}
}

#endif
