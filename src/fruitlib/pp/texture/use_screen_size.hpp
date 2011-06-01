#ifndef FRUITCUT_FRUITLIB_PP_TEXTURE_USE_SCREEN_SIZE_HPP_INCLUDED
#define FRUITCUT_FRUITLIB_PP_TEXTURE_USE_SCREEN_SIZE_HPP_INCLUDED

#include <sge/renderer/dim2.hpp>
#include <fcppt/math/dim/basic_impl.hpp>

namespace fruitcut
{
namespace fruitlib
{
namespace pp
{
namespace texture
{
// The filters all get a dimension argument specifying their output
// dimension. Since we have to differentiate between screen textures
// (which have a "dynamic" size) and normal textures, we introduce
// this constant here, since managing an optional<dim> would be much
// uglier (it would have to go in the texture descriptor which has an
// operator< which we would have to wrap in order to accomodate the
// optional value)
sge::renderer::dim2 const
use_screen_size();
}
}
}
}

#endif
