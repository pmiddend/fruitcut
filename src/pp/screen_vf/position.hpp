#ifndef FRUITCUT_PP_SCREEN_VF_POSITION_HPP_INCLUDED
#define FRUITCUT_PP_SCREEN_VF_POSITION_HPP_INCLUDED

#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/vector.hpp>
#include <sge/renderer/vf/make_unspecified_tag.hpp>
#include <sge/renderer/scalar.hpp>

namespace fruitcut
{
namespace pp
{
namespace screen_vf
{
namespace tags
{
SGE_RENDERER_VF_MAKE_UNSPECIFIED_TAG(position)
}

typedef 
sge::renderer::vf::unspecified
<
	sge::renderer::vf::vector
	<
		sge::renderer::scalar,
		2
	>,
	tags::position
> 
position;
}
}
}

#endif