#include <fruitlib/math/line_plane_intersection.hpp>
#include <fruitlib/math/cut_triangle_at_plane.hpp>
#include <fruitlib/math/line/basic.hpp>
#include <fruitlib/math/line/distance_to_point.hpp>
#include <fruitlib/math/triangle/vector_type.hpp>
#include <fruitlib/math/triangle/vertex_access.hpp>
#include <fruitlib/math/triangle/scalar_type.hpp>
#include <fruitlib/math/triangle/create_from_interpolation.hpp>
#include <fruitlib/math/triangle/size_type.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/text.hpp>
#include <fcppt/optional.hpp>
#include <fcppt/math/range_compare.hpp>
#include <fcppt/math/size_type.hpp>
#include <fcppt/math/vector/orthogonalize.hpp>
#include <fcppt/math/vector/normalize.hpp>
#include <fcppt/math/vector/dot.hpp>
#include <fcppt/math/vector/static.hpp>
#include <fcppt/math/vector/output.hpp>
#include <fcppt/algorithm/shift_compare.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/container/array.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/io/ostream.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/range/numeric.hpp>
#include <boost/spirit/home/phoenix/function/function.hpp>
#include <boost/spirit/home/phoenix/core/argument.hpp>
#include <boost/spirit/home/phoenix/bind.hpp>
#include <algorithm>

namespace
{
typedef
double
scalar;

fcppt::math::size_type const N = 3;

typedef
fcppt::math::vector::static_<scalar,2>::type
vector2;

typedef
fruitlib::math::line::basic<scalar,N>
line3;

typedef
fruitlib::math::plane::basic<scalar,N>
plane3;

typedef
line3::vector
vector3;

struct triangle3
{
public:
	typedef
	vector3
	vector;

	typedef
	fcppt::container::array<vector,3>
	array_type;

	typedef
	array_type::size_type
	size_type;

	array_type points;

	explicit
	triangle3()
	:
		points()
	{
	}

	explicit
	triangle3(
		vector const &a,
		vector const &b,
		vector const &c)
	{
		points[0] = a;
		points[1] = b;
		points[2] = c;
	}
};
}

namespace fruitlib
{
namespace math
{
namespace triangle
{
template<>
struct vector_type<triangle3>
{
	typedef
	::triangle3::vector
	type;
};

template<>
struct scalar_type<triangle3>
{
	typedef
	::triangle3::vector::value_type
	type;
};

template<>
struct vertex_access<triangle3>
{
	static triangle3::vector const &
	get(
		triangle3 const &t,
		math::triangle::size_type const n)
	{
		return t.points[n];
	}
};

template<>
struct create_from_interpolation<triangle3>
{
	static triangle3 const
	create(
		triangle3 const &input,
		math::triangle::interpolation_pair<triangle3> const &a,
		math::triangle::interpolation_pair<triangle3> const &b,
		math::triangle::interpolation_pair<triangle3> const &c)
	{
		triangle3 result;
		result.points[0] = a.value() * input.points[a.index1()] + (1-a.value()) * input.points[a.index2()];
		result.points[1] = b.value() * input.points[b.index1()] + (1-b.value()) * input.points[b.index2()];
		result.points[2] = c.value() * input.points[c.index1()] + (1-c.value()) * input.points[c.index2()];
		return result;
	}
};
}
}
}

namespace
{
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
fruitlib::math::triangle_plane_intersection<triangle3>
intersection_type;
}

BOOST_AUTO_TEST_CASE(line_plane)
{
	scalar const epsilon = 0.00001;

	vector3 const
		plane_direction =
			fcppt::math::vector::normalize(
				vector3(1,1,0)),
		line_direction =
			plane_direction;

	fcppt::io::cout() << FCPPT_TEXT("Checking intersection of a plane and a line\n");

	fcppt::optional<vector3> const intersection =
		fruitlib::math::line_plane_intersection<scalar,N>(
			line3(
				vector3(-4.5,1.5,0.0),
				line_direction),
			plane3(
				plane_direction,
				0));

	vector3 const expected_intersection = vector3(-3,3,0);

	fcppt::io::cout()
		<< FCPPT_TEXT("Expecting intersection at ")
		<< expected_intersection
		<< FCPPT_TEXT("\n");

	if (!intersection)
		fcppt::io::cout()
			<< FCPPT_TEXT("Got no intersection!\n");
	else
		fcppt::io::cout()
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

	fcppt::io::cout() << FCPPT_TEXT("Checking intersection of a plane and a line which are parallel\n");

	fcppt::optional<vector3> const intersection =
		fruitlib::math::line_plane_intersection<scalar,N>(
			line3(
				vector3(-4.5,1.5,0.0),
				line_direction),
			plane3(
				plane_direction,
				0));

	if (!intersection)
		fcppt::io::cout()
			<< FCPPT_TEXT("Got no intersection!\n");
	else
		fcppt::io::cout()
			<< FCPPT_TEXT("Got an intersection at ")
			<< (*intersection)
			<< FCPPT_TEXT("\n");

	BOOST_CHECK(
		!intersection);
}

BOOST_AUTO_TEST_CASE(cut_triangle_test)
{
	scalar const epsilon =
		static_cast<scalar>(0.0001);

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
			vector3(1.5,-1,0),
			vector3(6.0,-2,0),
			vector3(4.0,3.5,0)
		),
		upper_triangle
		(
			vector3(2,1,0),
			vector3(5.5,1,0),
			vector3(4,5,0)
		),
		lower_triangle
		(
			vector3(2,-1,0),
			vector3(5.5,-1,0),
			vector3(4,-5,0));

	{
		fcppt::io::cout() << FCPPT_TEXT("Cutting a triangle which shouldn't be cut\n");

		intersection_type const is =
			fruitlib::math::cut_triangle_at_plane(
				upper_triangle,
				p);

		fcppt::io::cout() << FCPPT_TEXT("Expected 0 border points and 1 triangle\n");

		fcppt::io::cout()
			<< FCPPT_TEXT("Got ")
			<< is.points().size()
			<< FCPPT_TEXT(" border points and ")
			<< is.triangles().size()
			<< FCPPT_TEXT(" triangles\n");

		BOOST_CHECK(
			is.points().size() == 0
				&& is.triangles().size() == 1);

		BOOST_CHECK(
			boost::inner_product(
				is.triangles().front().points,
				upper_triangle.points,
				true,
				std::logical_and<bool>(),
				boost::phoenix::bind(
					&fcppt::math::range_compare
					<
						vector3,
						vector3,
						scalar
					>,
					boost::phoenix::arg_names::arg1,
					boost::phoenix::arg_names::arg2,
					epsilon)));
	}

	{
		fcppt::io::cout() << FCPPT_TEXT("Cutting a triangle should be cut completely\n");

		intersection_type const is =
			fruitlib::math::cut_triangle_at_plane(
				lower_triangle,
				p);

		fcppt::io::cout() << FCPPT_TEXT("Expected 0 border points and 0 triangles\n");

		fcppt::io::cout()
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
		fcppt::io::cout() << FCPPT_TEXT("Cutting a triangle should be cut in half\n");

		intersection_type const is =
			fruitlib::math::cut_triangle_at_plane(
				middle_triangle,
				p);

		fcppt::io::cout() << FCPPT_TEXT("Expected 2 border points and 1 triangle\n");

		fcppt::io::cout()
			<< FCPPT_TEXT("Got ")
			<< is.points().size()
			<< FCPPT_TEXT(" border points and ")
			<< is.triangles().size()
			<< FCPPT_TEXT(" triangles\n");

		BOOST_CHECK(
			is.points().size() == 2
				&& is.triangles().size() == 1);

		fcppt::io::cout()
			<< FCPPT_TEXT("Border points are ")
			<< is.points()[0]
			<< FCPPT_TEXT(", ")
			<< is.points()[1]
			<< FCPPT_TEXT("\n");

		// We cannot test for the exact triangle since it could be permuted
		triangle3 const expected_triangle(
				vector3(2.05556,0,0),
				vector3(5.27273,0,0),
				vector3(4,3.5,0));

		fcppt::io::cout()
			<< FCPPT_TEXT("Expected the triangle to be: ")
			<< expected_triangle
			<< FCPPT_TEXT("\n");

		fcppt::io::cout()
			<< FCPPT_TEXT("Got a triangle: ")
			<< is.triangles()[0]
			<< FCPPT_TEXT("\n");

		BOOST_CHECK(
			fcppt::algorithm::shift_compare(
				is.triangles()[0].points,
				expected_triangle.points,
				std::tr1::bind(
					&fcppt::math::range_compare<vector3,vector3,scalar>,
					std::tr1::placeholders::_1,
					std::tr1::placeholders::_2,
					epsilon)) );
	}

	{
		plane3 new_plane(
			-p.normal(),
			p.lambda());

		fcppt::io::cout() << FCPPT_TEXT("Cutting the same triangle in the other half\n");

		intersection_type const is =
			fruitlib::math::cut_triangle_at_plane(
				middle_triangle,
				new_plane);

		fcppt::io::cout() << FCPPT_TEXT("Expected 2 border points and 2 triangles\n");

		fcppt::io::cout()
			<< FCPPT_TEXT("Got ")
			<< is.points().size()
			<< FCPPT_TEXT(" border points and ")
			<< is.triangles().size()
			<< FCPPT_TEXT(" triangles\n");

		BOOST_CHECK(
			is.points().size() == 2
				&& is.triangles().size() == 2);

		fcppt::io::cout()
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

BOOST_AUTO_TEST_CASE(orthonorm)
{
	scalar const epsilon =
		static_cast<scalar>(0.0001);

	typedef
	std::vector<vector2>
	container;

	fcppt::io::cout() << FCPPT_TEXT("Orthonormalizing two 2D vectors\n");

	container const
		wiki_example =
			fcppt::assign::make_container<container>
				(vector2(3,1))
				(vector2(2,2)),
		expected_result =
			fcppt::assign::make_container<container>
				(vector2(3,1))
				(vector2(-0.4,1.2));

	container result =
		wiki_example;

	fcppt::math::vector::orthogonalize(
		result.begin(),
		result.end());

	fcppt::io::cout()
		<< FCPPT_TEXT("Expecting ")
		<< expected_result[0]
		<< FCPPT_TEXT(" and ")
		<< expected_result[1]
		<< FCPPT_TEXT("\n");

	fcppt::io::cout()
		<< FCPPT_TEXT("Got ")
		<< result[0]
		<< FCPPT_TEXT(" and ")
		<< result[1]
		<< FCPPT_TEXT("\n");

	BOOST_CHECK((
		fcppt::math::range_compare(
			result[0],
			expected_result[0],
			epsilon) &&
		fcppt::math::range_compare(
			result[1],
			expected_result[1],
			epsilon)));
}

BOOST_AUTO_TEST_CASE(d_to_point)
{
	typedef
	fruitlib::math::line::basic<scalar,2>
	line_type;

	typedef
	line_type::vector
	vector_type;

	line_type const l(
		vector_type(
			1.5,1),
		vector_type(
			1,0.5));

	scalar const epsilon = static_cast<scalar>(0.001);

	fcppt::io::cout()
		<< FCPPT_TEXT("Checking if a point is on a line (it should be)...\n");

	BOOST_CHECK((
		fruitlib::math::line::distance_to_point<scalar,2>(
			vector_type(
				4.5,2.5),
			l) < epsilon));

	fcppt::io::cout()
		<< FCPPT_TEXT("Now checking if a point which is not on the line has the right distance...\n");

	fcppt::io::cout()
		<< FCPPT_TEXT("Distance should be 1.11803, is ")
		<<
			fruitlib::math::line::distance_to_point<scalar,2>(
				vector_type(
					3,3),
				l)
		<< FCPPT_TEXT("\n");

	BOOST_CHECK((
		fruitlib::math::line::distance_to_point<scalar,2>(
			vector_type(
				3,3),
			l) - static_cast<scalar>(1.11803) < epsilon));

	fcppt::io::cout()
		<< FCPPT_TEXT("Another test: this time, the distance should be 1.78885 and is ")
		<<
			fruitlib::math::line::distance_to_point<scalar,2>(
				vector_type(
					0.5,-1.5),
				l)
		<< FCPPT_TEXT("...\n");

	BOOST_CHECK((
		fruitlib::math::line::distance_to_point<scalar,2>(
			vector_type(
				0.5,-1.5),
			l) - static_cast<scalar>(1.78885) < epsilon));
}
