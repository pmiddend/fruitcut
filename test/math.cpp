#include "../math/line_plane_intersection.hpp"
#include "../math/cut_triangle_at_plane.hpp"
#include "../math/triangle/basic.hpp"
#include "../math/triangle/weak_compare.hpp"
#include <fcppt/io/cout.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/range_compare.hpp>
#include <fcppt/algorithm/shift_compare.hpp>
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

struct triangle3
{
public:
	typedef
	vector3
	vector;

	typedef
	vector3
	value_type;

	typedef
	std::array<vector3,3>
	array_type;

	typedef
	array_type::size_type
	size_type;

	array_type points;

	triangle3(std::initializer_list<vector3> const &_points)
	{
		std::copy(_points.begin(),_points.end(),points.begin());
	}
};

fcppt::io::ostream &operator<<(fcppt::io::ostream &s,triangle3 const &p)
{
	return 
		s 
			<< s.widen('(') 
			<< p.points[0] << s.widen(',')
			<< p.points[1] << s.widen(',')
			<< p.points[2] 
			<< s.widen(')');
}

typedef
fruitcut::math::triangle_plane_intersection<triangle3>
intersection_type;
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
			}
		),
		upper_triangle
		(
			{
				vector3(2,1,0),
				vector3(5.5,1,0),
				vector3(4,5,0)
			}
		),
		lower_triangle
		(
		{
			vector3(2,-1,0),
			vector3(5.5,-1,0),
			vector3(4,-5,0)
		});

	std::function<vector3 const &(triangle3 const &,triangle3::size_type)> 
		get_position = 
			[](triangle3 const &t,triangle3::size_type const i)
			{
				return t.points[i];
			},
		get_data = 
			[](triangle3 const &t,triangle3::size_type const i) -> vector3
			{
				//fcppt::io::cout << "returning data: " << t.points[i] << "\n";
				return t.points[i];
			};

	std::function<vector3 const(vector3 const &,vector3 const &,scalar)> 
		interpolator = 
			[](vector3 const &a, vector3 const &b,scalar c) -> vector3
			{ 
				//fcppt::io::cout << "a: " << a << ", b: " << b << "\n";
				return a * c + (1.0-c) * b;
			};

	std::function<triangle3 const (vector3 const &,vector3 const &,vector3 const &)> 
		create_triangle = 
			[](vector3 const &a,vector3 const &b,vector3 const &c)
			{
				return triangle3({a,b,c});
			};

	{
		fcppt::io::cout << FCPPT_TEXT("Cutting a triangle which shouldn't be cut\n");

		intersection_type const is =
			fruitcut::math::cut_triangle_at_plane(
				upper_triangle,
				p,
				get_position,
				get_data,
				interpolator,
				create_triangle);

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
				is.triangles().front().points.begin(),
				is.triangles().front().points.end(),
				upper_triangle.points.begin(),
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

		intersection_type const is = 
			fruitcut::math::cut_triangle_at_plane(
				lower_triangle,
				p,
				get_position,
				get_data,
				interpolator,
				create_triangle);

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

		intersection_type const is = 
			fruitcut::math::cut_triangle_at_plane(
				middle_triangle,
				p,
				get_position,
				get_data,
				interpolator,
				create_triangle);

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

		fcppt::io::cout 
			<< FCPPT_TEXT("Border points are ") 
			<< is.points()[0]
			<< FCPPT_TEXT(", ")
			<< is.points()[1]
			<< FCPPT_TEXT("\n");

		// We cannot test for the exact triangle since it could be permuted
		triangle3 const expected_triangle(
			{
				vector3(2.05556,0,0),
				vector3(5.27273,0,0),
				vector3(4,3.5,0),
			});

		fcppt::io::cout 
			<< FCPPT_TEXT("Expected the triangle to be: ")
			<< expected_triangle
			<< FCPPT_TEXT("\n");

		fcppt::io::cout 
			<< FCPPT_TEXT("Got a triangle: ") 
			<< is.triangles()[0]
			<< FCPPT_TEXT("\n");

		BOOST_CHECK( 
			fcppt::algorithm::shift_compare(
				is.triangles()[0].points,
				expected_triangle.points,
				std::bind(
					&fcppt::math::range_compare<vector3,vector3,scalar>,
					std::placeholders::_1,
					std::placeholders::_2,
					epsilon)) );
	}

	{
		plane3 new_plane(
			-p.normal(),
			p.lambda());
		
		fcppt::io::cout << FCPPT_TEXT("Cutting the same triangle in the other half\n");

		intersection_type const is = 
			fruitcut::math::cut_triangle_at_plane(
				middle_triangle,
				new_plane,
				get_position,
				get_data,
				interpolator,
				create_triangle);

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
