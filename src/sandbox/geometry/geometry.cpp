#include <fruitlib/geometry_traits/box.hpp>
#include <fruitlib/geometry_traits/vector.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/box/output.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/multi/multi.hpp>
#include <boost/geometry/multi/geometries/multi_point.hpp>
#include <iostream>
#include <vector>
#include <fcppt/config/external_end.hpp>


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
	fcppt::math::box::object<float,3>
	box3;

	box3 env =
		boost::geometry::return_envelope<box3>(
			points);

	std::cout << env << std::endl;
}
