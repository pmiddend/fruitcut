#ifndef FRUITCUT_SANDBOX_SCREEN_VF_FORMAT_HPP_INCLUDED
#define FRUITCUT_SANDBOX_SCREEN_VF_FORMAT_HPP_INCLUDED

#include "position.hpp"
#include <sge/renderer/vf/format.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace sandbox
{
namespace screen_vf
{
typedef 
sge::renderer::vf::format
<
	boost::mpl::vector1
	<
		position
	>
> 
format;
}
}
}

#endif
