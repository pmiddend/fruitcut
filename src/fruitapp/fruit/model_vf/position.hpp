#ifndef FRUITAPP_FRUIT_MODEL_VF_POSITION_HPP_INCLUDED
#define FRUITAPP_FRUIT_MODEL_VF_POSITION_HPP_INCLUDED

#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vf/make_unspecified_tag.hpp>
#include <sge/renderer/vf/unspecified.hpp>
#include <sge/renderer/vf/vector.hpp>

namespace fruitapp
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

#endif
