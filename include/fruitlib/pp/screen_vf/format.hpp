#ifndef FRUITLIB_PP_SCREEN_VF_FORMAT_HPP_INCLUDED
#define FRUITLIB_PP_SCREEN_VF_FORMAT_HPP_INCLUDED

#include <fruitlib/pp/screen_vf/part.hpp>
#include <sge/renderer/vf/format.hpp>
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
sge::renderer::vf::format
<
	boost::mpl::vector1
	<
		part
	>
>
format;
}
}
}

#endif
