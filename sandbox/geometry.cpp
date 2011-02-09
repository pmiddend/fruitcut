#include <iostream>
#include "../geometry_traits/vector.hpp"
#include "../geometry_traits/box.hpp"
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/multi/multi.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/box/output.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <vector>

int main()
{
	typedef
	fcppt::math::vector::static_<float,3>::type
	vector3;

	typedef
	fcppt::math::vector::static_<float,2>::type
	vector2;

	typedef 
	boost::geometry::model::multi_point<vector3> 
	point_cloud;

	point_cloud points;

	points.push_back(
		vector3(
			-1,-1,-1));

	points.push_back(
		vector3(
			1,1,1));

	points.push_back(
		vector3(
			0,0,0));

	typedef
	fcppt::math::box::basic<float,3>
	box3;

	box3 env = 
		boost::geometry::make_envelope<box3>(
			points);

	std::cout << env << std::endl;
}
