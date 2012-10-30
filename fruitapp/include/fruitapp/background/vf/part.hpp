#ifndef FRUITAPP_BACKGROUND_VF_PART_HPP_INCLUDED
#define FRUITAPP_BACKGROUND_VF_PART_HPP_INCLUDED

#include <fruitapp/background/vf/position.hpp>
#include <fruitapp/background/vf/texcoord.hpp>
#include <sge/renderer/vf/part.hpp>
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
sge::renderer::vf::part
<
	boost::mpl::vector2
	<
		fruitapp::background::vf::position,
		fruitapp::background::vf::texcoord
	>
>
part;
}
}
}

#endif
