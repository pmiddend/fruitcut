#ifndef FRUITLIB_MATH_TRIANGLE_BASIC_HPP_INCLUDED
#define FRUITLIB_MATH_TRIANGLE_BASIC_HPP_INCLUDED

#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/io/ostream.hpp>
#include <fcppt/container/array.hpp>
#include <algorithm>

namespace fruitlib
{
namespace math
{
namespace triangle
{
template
<
	typename T,
	fcppt::math::size_type N,
	typename Data
>
class basic
{
public:
	typedef typename
	fcppt::math::vector::static_<T,N>::type
	vector;

	typedef
	fcppt::container::array<vector,3>
	point_array;

	typedef
	fcppt::container::array<Data,3>
	data_array;

	basic(
		point_array const &_points,
		data_array const &_data)
	{
		std::copy(
			_points.begin(),
			_points.end(),
			points_.begin());
		std::copy(
			_data.begin(),
			_data.end(),
			data_.begin());
	}

	// There's no non-const getter because that would break the
	// invariant points_.size() == 3
	point_array const &
	points() const
	{
		return points_;
	}

	data_array const &
	data() const
	{
		return data_;
	}
private:
	point_array points_;
	data_array data_;
};
}
}
}

template<typename T,fcppt::math::size_type N,typename Data>
fcppt::io::ostream &
operator<<(
	fcppt::io::ostream &s,
	fruitlib::math::triangle::basic<T,N,Data> const &p)
{
	return
		s
			<< s.widen('(')
			<< p.points()[0]
				<< s.widen(' ')  << s.widen('[') << p.data()[0] << s.widen(']')
				<< s.widen(',')
			<< p.points()[1]
				<< s.widen(' ')  << s.widen('[') << p.data()[1] << s.widen(']')
				<< s.widen(',')
			<< p.points()[2]
				<< s.widen(' ')  << s.widen('[') << p.data()[2] << s.widen(']')
			<< s.widen(')');
}

#endif
