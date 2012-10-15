#ifndef FRUITAPP_FRUIT_MODEL_VF_FORMAT_HPP_INCLUDED
#define FRUITAPP_FRUIT_MODEL_VF_FORMAT_HPP_INCLUDED

#include <fruitapp/fruit/model_vf/part.hpp>
#include <sge/renderer/vf/format.hpp>
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
sge::renderer::vf::format
<
	boost::mpl::vector1<fruitapp::fruit::model_vf::part>
>
format;
}
}
}

#endif
