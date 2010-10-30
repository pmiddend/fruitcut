#include "../math/line_plane_intersection.hpp"
#include "../math/cut_triangle_at_plane.hpp"
#include "../math/triangle/basic.hpp"
#include "../math/triangle/weak_compare.hpp"
#include <fcppt/io/cout.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/range_compare.hpp>
#include <boost/test/unit_test.hpp>
#include <algorithm>

namespace
{
typedef
double
scalar;

scalar const epsilon = 0.00001;

fcppt::math::size_type const N = 3;

typedef
fruitcut::math::line::basic<scalar,N>
line3;

typedef
fruitcut::math::plane::basic<scalar,N>
plane3;

typedef
line3::vector
vector3;

typedef
scalar
data_type;

typedef
fruitcut::math::triangle::basic<scalar,N,data_type>
triangle3;
}

BOOST_AUTO_TEST_CASE(line_plane)
{
	vector3 const 
		plane_direction = 
			fcppt::math::vector::normalize(
				vector3(1,1,0)),
		line_direction = 
			plane_direction;

	fcppt::io::cout << FCPPT_TEXT("Checking intersection of a plane and a line\n");

	fcppt::optional<vector3> const intersection = 
		fruitcut::math::line_plane_intersection<scalar,N>(
			line3(
				vector3(-4.5,1.5,0.0),
				line_direction),
			plane3(
				plane_direction,
				0));

	vector3 const expected_intersection = vector3(-3,3,0);

	fcppt::io::cout 
		<< FCPPT_TEXT("Expecting intersection at ") 
		<< expected_intersection 
		<< FCPPT_TEXT("\n");

	if (!intersection)
		fcppt::io::cout 
			<< FCPPT_TEXT("Got no intersection!\n");
	else
		fcppt::io::cout 
			<< FCPPT_TEXT("Got an intersection at ") 
			<< (*intersection) 
			<< FCPPT_TEXT("\n");

	BOOST_CHECK(
		intersection 
			&& fcppt::math::range_compare(*intersection,expected_intersection,epsilon));
}

BOOST_AUTO_TEST_CASE(line_plane_no_intersection)
{
	vector3 const 
		plane_direction = 
			fcppt::math::vector::normalize(
				vector3(1,1,0)),
		line_direction = 
			fcppt::math::vector::normalize(
				vector3(-1,1,0));

	fcppt::io::cout << FCPPT_TEXT("Checking intersection of a plane and a line which are parallel\n");

	fcppt::optional<vector3> const intersection = 
		fruitcut::math::line_plane_intersection<scalar,N>(
			line3(
				vector3(-4.5,1.5,0.0),
				line_direction),
			plane3(
				plane_direction,
				0));

	if (!intersection)
		fcppt::io::cout 
			<< FCPPT_TEXT("Got no intersection!\n");
	else
		fcppt::io::cout 
			<< FCPPT_TEXT("Got an intersection at ") 
			<< (*intersection) 
			<< FCPPT_TEXT("\n");

	BOOST_CHECK(
		!intersection);
}

BOOST_AUTO_TEST_CASE(cut_triangle_test)
{
	vector3 const plane_normal = 
		vector3(0,1,0);

	plane3 const p(
		plane_normal,
		fcppt::math::vector::dot(
			vector3(3.5,0,0),
			plane_normal));

	triangle3 const 
		middle_triangle
		(
			{
				vector3(1.5,-1,0),
				vector3(6.0,-2,0),
				vector3(4.0,3.5,0)
			},
			{ 1,2,3 }
		),
		upper_triangle
		(
			{
				vector3(2,1,0),
				vector3(5.5,1,0),
				vector3(4,5,0)
			},
			{ 1,2,3 }
		),
		lower_triangle
		(
		{
			vector3(2,-1,0),
			vector3(5.5,-1,0),
			vector3(4,-5,0)
		},
		{1,2,3});

	std::function<data_type(data_type,data_type,scalar)> interpolator = 
		[](data_type a, data_type b,scalar c) 
		{ 
			return data_type(a * c + (1.0-c) * b);
		};

	{
		fcppt::io::cout << FCPPT_TEXT("Cutting a triangle which shouldn't be cut\n");

		fruitcut::math::triangle_plane_intersection<scalar,N,data_type> const is = 
			fruitcut::math::cut_triangle_at_plane<scalar,N,data_type>(
				upper_triangle,
				p,
				interpolator);

		fcppt::io::cout << FCPPT_TEXT("Expected 0 border points and 1 triangle\n");

		fcppt::io::cout 
			<< FCPPT_TEXT("Got ") 
			<< is.points().size()
			<< FCPPT_TEXT(" border points and ")
			<< is.triangles().size()
			<< FCPPT_TEXT(" triangles\n");

		BOOST_CHECK(
			is.points().size() == 0 
				&& is.triangles().size() == 1);

		BOOST_CHECK(
			std::inner_product(
				is.triangles().front().points().begin(),
				is.triangles().front().points().end(),
				upper_triangle.points().begin(),
				true,
				std::logical_and<bool>(),
				[&epsilon](vector3 const &a,vector3 const &b) 
				{
					return 
						fcppt::math::range_compare(a,b,epsilon);
				}));
	}

	{
		fcppt::io::cout << FCPPT_TEXT("Cutting a triangle should be cut completely\n");

		fruitcut::math::triangle_plane_intersection<scalar,N,data_type> const is = 
			fruitcut::math::cut_triangle_at_plane<scalar,N>(
				lower_triangle,
				p,
				interpolator);

		fcppt::io::cout << FCPPT_TEXT("Expected 0 border points and 0 triangles\n");

		fcppt::io::cout 
			<< FCPPT_TEXT("Got ") 
			<< is.points().size()
			<< FCPPT_TEXT(" border points and ")
			<< is.triangles().size()
			<< FCPPT_TEXT(" triangles\n");

		BOOST_CHECK(
			is.points().size() == 0 
				&& is.triangles().size() == 0);
	}

	{
		fcppt::io::cout << FCPPT_TEXT("Cutting a triangle should be cut in half\n");

		fruitcut::math::triangle_plane_intersection<scalar,N,data_type> const is = 
			fruitcut::math::cut_triangle_at_plane<scalar,N>(
				middle_triangle,
				p,
				interpolator);

		fcppt::io::cout << FCPPT_TEXT("Expected 2 border points and 1 triangle\n");

		fcppt::io::cout 
			<< FCPPT_TEXT("Got ") 
			<< is.points().size()
			<< FCPPT_TEXT(" border points and ")
			<< is.triangles().size()
			<< FCPPT_TEXT(" triangles\n");

		BOOST_CHECK(
			is.points().size() == 2
				&& is.triangles().size() == 1);

		// We cannot test for the exact triangle since it could be permuted
		triangle3 const expected_triangle(
			{
				vector3(2.05556,0,0),
				vector3(5.27273,0,0),
				vector3(4,3.5,0),
			},{1,2,3});

		fcppt::io::cout 
			<< FCPPT_TEXT("Expected the triangle to be: ")
			<< expected_triangle
			<< FCPPT_TEXT("\n");

		fcppt::io::cout 
			<< FCPPT_TEXT("Got a triangle: ") 
			<< is.triangles()[0]
			<< FCPPT_TEXT("\n");

		BOOST_CHECK(
			(
			fruitcut::math::triangle::weak_compare<scalar,N,data_type>(
				expected_triangle,
				is.triangles()[0],
				epsilon)));
	}

	{
		plane3 new_plane(
			-p.normal(),
			p.lambda());
		
		fcppt::io::cout << FCPPT_TEXT("Cutting the same triangle in the other half\n");

		fruitcut::math::triangle_plane_intersection<scalar,N,data_type> const is = 
			fruitcut::math::cut_triangle_at_plane<scalar,N,data_type>(
				middle_triangle,
				new_plane,
				interpolator);

		fcppt::io::cout << FCPPT_TEXT("Expected 2 border points and 2 triangles\n");

		fcppt::io::cout 
			<< FCPPT_TEXT("Got ") 
			<< is.points().size()
			<< FCPPT_TEXT(" border points and ")
			<< is.triangles().size()
			<< FCPPT_TEXT(" triangles\n");

		BOOST_CHECK(
			is.points().size() == 2
				&& is.triangles().size() == 2);

		fcppt::io::cout 
			<< FCPPT_TEXT("The triangles are: \n")
			<< is.triangles()[0]
			<< FCPPT_TEXT("\n")
			<< is.triangles()[1]
			<< FCPPT_TEXT("\n");

		// Correct triangles are:
		// ((6,-2,0),(5.27273,0,0),(1.5,-1,0))
		// ((5.27273,0,0),(2.05556,0,0),(1.5,-1,0))
	}
}
