#ifndef FRUITCUT_SANDBOX_PP_FILTER_PROPERTY_HPP_INCLUDED
#define FRUITCUT_SANDBOX_PP_FILTER_PROPERTY_HPP_INCLUDED

#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/vector3.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/variant/object.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace sandbox
{
namespace pp
{
namespace filter
{
typedef
fcppt::variant::object
<
	boost::mpl::vector3
	<
		sge::renderer::scalar,
		sge::renderer::vector2,
		sge::renderer::vector3
	>
>
property;
}
}
}
}

#endif
