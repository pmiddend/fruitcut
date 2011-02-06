#include <iostream>
#include "../geometry_traits/vector.hpp"
#include "../geometry_traits/box.hpp"
#include <boost/geometry/geometries/box.hpp>
#include <boost/geometry/algorithms/length.hpp>
#include <boost/geometry/algorithms/envelope.hpp>
#include <boost/geometry/strategies/strategies.hpp>
#include <boost/geometry/geometries/adapted/std_as_linestring.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/box/output.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <vector>

#if 0
namespace boost 
{ 
namespace geometry 
{ 
namespace traits 
{

template<typename T,fcppt::math::size_type N>
struct tag<fcppt::math::box::basic<T,N> >
{
	typedef box_tag type;
};

template<typename T,fcppt::math::size_type N>
struct point_type< fcppt::math::box::basic<T,N> >
{
    typedef typename fcppt::math::box::basic<T,N>::vector type;
};

template<typename T,fcppt::math::size_type N, std::size_t Dimension>
struct indexed_access<fcppt::math::box::basic<T,N>, boost::geometry::min_corner, Dimension>
{
	static inline T 
	get(
		fcppt::math::box::basic<T,N> const &b)
	{
		return b.pos()[Dimension];
	}

	static inline void 
	set(
		fcppt::math::box::basic<T,N> &b, 
		T const& value)
	{
		typename fcppt::math::box::basic<T,N>::vector p = 
			b.pos();
		typename fcppt::math::box::basic<T,N>::dim d = 
			b.dimension();
		d[Dimension] += 
			p[Dimension] - value;
		p[Dimension] = 
			value;
		b.pos(
			p);
		b.dimension(
			d);
	}
};

template<typename T,fcppt::math::size_type N, std::size_t Dimension>
struct indexed_access<fcppt::math::box::basic<T,N>, boost::geometry::max_corner, Dimension>
{
	static inline T 
	get(
		fcppt::math::box::basic<T,N> const &b)
	{
		return (b.pos() + b.dimension())[Dimension];
	}

	static inline void 
	set(
		fcppt::math::box::basic<T,N> &b, 
		T const& value)
	{
		typename fcppt::math::box::basic<T,N>::dim d = 
			b.dimension();
		d[Dimension] = 
			value - (b.pos()[Dimension] + b.dimension()[Dimension]);
		b.dimension(
			d);
	}
};

}
}
}

namespace boost 
{ 
namespace geometry 
{ 
namespace traits 
{
template<typename T,typename N,typename S>
struct tag<fcppt::math::vector::basic<T,N,S> >
{
	typedef point_tag type;
};

template<typename T,typename N,typename S> 
struct coordinate_type<fcppt::math::vector::basic<T,N,S> >
{ 
	typedef T type; 
};

template<typename T,typename N,typename S> 
struct coordinate_system<fcppt::math::vector::basic<T,N,S> >
{ 
	typedef boost::geometry::cs::cartesian type; 
};

template<typename T,typename N,typename S> 
struct dimension<fcppt::math::vector::basic<T,N,S> > 
	: N
{};

template<typename T,typename N,typename S,std::size_t Dim>
struct access<fcppt::math::vector::basic<T,N,S>,Dim>
{
	static T
	get(
		fcppt::math::vector::basic<T,N,S> const &p)
	{
			return p[Dim];
	}

	static void 
	set(
		fcppt::math::vector::basic<T,N,S> &p, 
		T const& value)
	{
			p[Dim] = value;
	}
};

}
}
}
#endif



int main()
{
	typedef
	fcppt::math::vector::static_<float,3>::type
	vector3;

	typedef
	fcppt::math::vector::static_<float,2>::type
	vector2;

	typedef 
	std::vector<vector2> 
	ln;

	ln myline;
	myline.push_back(
		vector2(
			-1,-1));
	myline.push_back(
		vector2(
			0.5f,-0.5f));
	myline.push_back(
		vector2(
			-0.5f,3.0f));

	typedef
	fcppt::math::box::basic<float,2>
	box;

	box env = 
		boost::geometry::make_envelope<box>(
			myline);

	std::cout << env << std::endl;
}
