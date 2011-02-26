#ifndef FRUITCUT_APP_FRUIT_MODEL_VF_POSITION_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_MODEL_VF_POSITION_HPP_INCLUDED

#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vf/make_unspecified_tag.hpp>
#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/vector.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
namespace model_vf
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
		3
	>,
	tags::position
> 
position;
}
}
}
}

#endif
