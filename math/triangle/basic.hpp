#ifndef FRUITCUT_MATH_TRIANGLE_BASIC_HPP_INCLUDED
#define FRUITCUT_MATH_TRIANGLE_BASIC_HPP_INCLUDED

#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/assert.hpp>
#include <fcppt/io/ostream.hpp>
#include <initializer_list>
#include <array>
#include <algorithm>

namespace fruitcut
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
	std::array<vector,3>
	point_array;

	typedef
	std::array<Data,3>
	data_array;

	basic(
		std::initializer_list<vector> const &_points,
		std::initializer_list<Data> const &_data)
	{
		FCPPT_ASSERT(
			_points.size() == 3 &&
			_data.size() == 3);
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
	fruitcut::math::triangle::basic<T,N,Data> const &p)
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
