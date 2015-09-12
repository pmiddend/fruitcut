#ifndef FRUITLIB_PERSPECTIVE_PROJECTION_INFORMATION_TO_MATRIX_HPP_INCLUDED
#define FRUITLIB_PERSPECTIVE_PROJECTION_INFORMATION_TO_MATRIX_HPP_INCLUDED

#include <fruitlib/perspective_projection_information_fwd.hpp>
#include <fruitlib/detail/symbol.hpp>
#include <sge/renderer/matrix4.hpp>
#include <fcppt/preprocessor/const.hpp>
#include <fcppt/preprocessor/warn_unused_result.hpp>


namespace fruitlib
{
FRUITLIB_DETAIL_SYMBOL
sge::renderer::matrix4 const
perspective_projection_information_to_matrix(
	fruitlib::perspective_projection_information const &)
FCPPT_PP_WARN_UNUSED_RESULT
FCPPT_PP_CONST;
}

#endif

