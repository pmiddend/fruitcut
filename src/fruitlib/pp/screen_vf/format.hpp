#ifndef FRUITLIB_PP_SCREEN_VF_FORMAT_HPP_INCLUDED
#define FRUITLIB_PP_SCREEN_VF_FORMAT_HPP_INCLUDED

#include "part.hpp"
#include <sge/renderer/vf/format.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitlib
{
namespace pp
{
namespace screen_vf
{
typedef 
sge::renderer::vf::format
<
	boost::mpl::vector1
	<
		part
	>
> 
format;
}
}
}

#endif
