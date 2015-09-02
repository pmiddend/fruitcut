#ifndef FRUITLIB_DEF_CTOR_HPP_INCLUDED
#define FRUITLIB_DEF_CTOR_HPP_INCLUDED

#include <fcppt/no_init.hpp>
#include <fcppt/preprocessor/disable_gcc_warning.hpp>
#include <fcppt/preprocessor/pop_warning.hpp>
#include <fcppt/preprocessor/push_warning.hpp>


namespace fruitlib
{

FCPPT_PP_PUSH_WARNING
FCPPT_PP_DISABLE_GCC_WARNING(-Weffc++)

template<
	typename T
>
struct def_ctor
:
	T
{
	typedef
	T
	base_type;

	def_ctor()
	:
		T(
			fcppt::no_init{}
		)
	{
	}

	explicit
	def_ctor(
		T const &_value
	)
	:
		T(
			_value
		)
	{
	}

	using T::T;

	T &
	base()
	{
		return
			*this;
	}

	T const &
	base() const
	{
		return
			*this;
	}
};

FCPPT_PP_POP_WARNING

}

#endif
