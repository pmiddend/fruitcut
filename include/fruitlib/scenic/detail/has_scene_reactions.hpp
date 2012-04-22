#ifndef FRUITLIB_SCENIC_DETAIL_HAS_SCENE_REACTIONS_HPP_INCLUDED
#define FRUITLIB_SCENIC_DETAIL_HAS_SCENE_REACTIONS_HPP_INCLUDED

#include <fcppt/preprocessor/disable_gcc_warning.hpp>
#include <fcppt/preprocessor/pop_warning.hpp>
#include <fcppt/preprocessor/push_warning.hpp>
#include <fcppt/type_traits/generate_has_type.hpp>

namespace fruitlib
{
namespace scenic
{
namespace detail
{
FCPPT_PP_PUSH_WARNING
FCPPT_PP_DISABLE_GCC_WARNING(-Weffc++)

FCPPT_TYPE_TRAITS_GENERATE_HAS_TYPE(
	scene_reactions);

FCPPT_PP_POP_WARNING
}
}
}

#endif
