#ifndef FRUITLIB_PP_TEXTURE_OPTIONAL_DEPTH_STENCIL_SURFACE_HPP_INCLUDED
#define FRUITLIB_PP_TEXTURE_OPTIONAL_DEPTH_STENCIL_SURFACE_HPP_INCLUDED

#include <sge/renderer/depth_stencil_buffer/surface_unique_ptr.hpp>
#include <fcppt/optional.hpp>


namespace fruitlib
{
namespace pp
{
namespace texture
{

typedef
fcppt::optional<
	sge::renderer::depth_stencil_buffer::surface_unique_ptr
>
optional_depth_stencil_surface;

}
}
}

#endif
