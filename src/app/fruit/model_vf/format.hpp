#ifndef FRUITCUT_APP_FRUIT_MODEL_VF_FORMAT_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_MODEL_VF_FORMAT_HPP_INCLUDED

#include "part.hpp"
#include <sge/renderer/vf/format.hpp>
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
sge::renderer::vf::format
<
	boost::mpl::vector1<part>
> 
format;
}
}
}
}

#endif
