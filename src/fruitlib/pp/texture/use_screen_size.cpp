#include "use_screen_size.hpp"
#include <fcppt/math/dim/basic_impl.hpp>
#include <limits>

sge::renderer::dim2 const
fruitlib::pp::texture::use_screen_size()
{
	return 
		sge::renderer::dim2(
			std::numeric_limits<sge::renderer::dim2::value_type>::max(),
			std::numeric_limits<sge::renderer::dim2::value_type>::max());
}
