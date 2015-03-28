#ifndef FRUITLIB_PP_TEXTURE_USE_SCREEN_SIZE_HPP_INCLUDED
#define FRUITLIB_PP_TEXTURE_USE_SCREEN_SIZE_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <sge/renderer/dim2.hpp>
#include <fcppt/math/dim/object_impl.hpp>

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
FRUITLIB_DETAIL_SYMBOL
sge::renderer::dim2 const
use_screen_size();
}
}
}

#endif
