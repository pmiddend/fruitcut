#ifndef FRUITCUT_LINE_DRAWER_VF_FORMAT_HPP_INCLUDED
#define FRUITCUT_LINE_DRAWER_VF_FORMAT_HPP_INCLUDED

#include "part.hpp"
#include <sge/renderer/vf/format.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace line_drawer
{
namespace vf
{
typedef 
sge::renderer::vf::format
<
	boost::mpl::vector1<part>
> 
format;
}
}
}

#endif
