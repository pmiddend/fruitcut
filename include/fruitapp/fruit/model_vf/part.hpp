#ifndef FRUITAPP_FRUIT_MODEL_VF_PART_HPP_INCLUDED
#define FRUITAPP_FRUIT_MODEL_VF_PART_HPP_INCLUDED

#include <fruitapp/fruit/model_vf/normal.hpp>
#include <fruitapp/fruit/model_vf/position.hpp>
#include <fruitapp/fruit/model_vf/texcoord.hpp>
#include <sge/renderer/vf/part.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace fruit
{
namespace model_vf
{
typedef
sge::renderer::vf::part
<
	boost::mpl::vector3
	<
		position,
		texcoord,
		normal
	>
>
part;
}
}
}

#endif
