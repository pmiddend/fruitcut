#ifndef FRUITCUT_MATH_PLANE_BASIC_HPP_INCLUDED
#define FRUITCUT_MATH_PLANE_BASIC_HPP_INCLUDED

#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/io/ostream.hpp>

namespace fruitcut
{
namespace math
{
namespace plane
{
template<typename T,fcppt::math::size_type N>
class basic
{
public:
	typedef typename
	fcppt::math::vector::static_<T,N>::type 
	vector;

	typedef
	T
	scalar;

	explicit
	basic()
	{
	}

	explicit
	basic(
		vector const &_normal,
		scalar const _lambda)
	:
		normal_(
			_normal),
		lambda_(
			_lambda)
	{
	}

	vector const
	normal() const
	{
		return normal_;
	}

	scalar const
	lambda() const
	{
		return lambda_;
	}
private:
	vector normal_;
	scalar lambda_;
};
}
}
}

template<typename T,fcppt::math::size_type N>
fcppt::io::ostream &
operator<<(
	fcppt::io::ostream &s,
	fruitcut::math::plane::basic<T,N> const &p)
{
	return 
		s << s.widen('(') << p.normal() << s.widen(',') 
			<< p.lambda() << s.widen(')');
}

#endif
