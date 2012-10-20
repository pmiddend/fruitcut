#ifndef FRUITLIB_PP_SCREEN_VF_PART_HPP_INCLUDED
#define FRUITLIB_PP_SCREEN_VF_PART_HPP_INCLUDED

#include <fruitlib/pp/screen_vf/position.hpp>
#include <sge/renderer/vf/part.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace pp
{
namespace screen_vf
{
typedef
sge::renderer::vf::part
<
	boost::mpl::vector1
	<
		fruitlib::pp::screen_vf::position
	>
>
part;
}
}
}

#endif
