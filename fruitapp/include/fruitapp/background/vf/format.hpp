#ifndef FRUITAPP_BACKGROUND_VF_FORMAT_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_VF_FORMAT_HPP_INCLUDED

#include <fruitapp/background/vf/part.hpp>
#include <sge/renderer/vf/format.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace background
{
namespace vf
{
typedef
sge::renderer::vf::format
<
	boost::mpl::vector1
	<
		fruitapp::background::vf::part
	>
>
format;
}
}
}

#endif
