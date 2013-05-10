#ifndef FRUITAPP_FRUIT_TRIANGLE_TRAITS_HPP_INCLUDED
#define FRUITAPP_FRUIT_TRIANGLE_TRAITS_HPP_INCLUDED

#include <fruitapp/fruit/triangle.hpp>
#include <fruitlib/math/triangle/create_from_interpolation.hpp>
#include <fruitlib/math/triangle/interpolation_pair.hpp>
#include <fruitlib/math/triangle/scalar_type.hpp>
#include <fruitlib/math/triangle/size_type.hpp>
#include <fruitlib/math/triangle/vector_type.hpp>
#include <fruitlib/math/triangle/vertex_access.hpp>
#include <fcppt/math/vector/arithmetic.hpp>

namespace fruitlib
{
namespace math
{
namespace triangle
{
template<>
struct vector_type<fruitapp::fruit::triangle>
{
	typedef
	fruitapp::fruit::triangle::vector
	type;
};

template<>
struct scalar_type<fruitapp::fruit::triangle>
{
	typedef
	fruitapp::fruit::triangle::vector::value_type
	type;
};

template<>
struct vertex_access<fruitapp::fruit::triangle>
{
	static fruitapp::fruit::triangle::vector const &
	get(
		fruitapp::fruit::triangle const &t,
		fruitlib::math::triangle::size_type const n)
	{
		return t.vertices[n];
	}
};

template<>
struct create_from_interpolation<fruitapp::fruit::triangle>
{
	static fruitapp::fruit::triangle const
	create(
		fruitapp::fruit::triangle const &input,
		fruitlib::math::triangle::interpolation_pair<fruitapp::fruit::triangle> const &a,
		fruitlib::math::triangle::interpolation_pair<fruitapp::fruit::triangle> const &b,
		fruitlib::math::triangle::interpolation_pair<fruitapp::fruit::triangle> const &c)
	{
		return
			fruitapp::fruit::triangle(
				fruitapp::fruit::triangle::vertex_array{{
					a.value() * input.vertices[a.index1()] + (1-a.value()) * input.vertices[a.index2()],
					b.value() * input.vertices[b.index1()] + (1-b.value()) * input.vertices[b.index2()],
					c.value() * input.vertices[c.index1()] + (1-c.value()) * input.vertices[c.index2()]
				}},
				fruitapp::fruit::triangle::texcoord_array{{
					a.value() * input.texcoords[a.index1()] + (1-a.value()) * input.texcoords[a.index2()],
					b.value() * input.texcoords[b.index1()] + (1-b.value()) * input.texcoords[b.index2()],
					c.value() * input.texcoords[c.index1()] + (1-c.value()) * input.texcoords[c.index2()]
				}},
				fruitapp::fruit::triangle::normal_array{{
					a.value() * input.normals[a.index1()] + (1-a.value()) * input.normals[a.index2()],
					b.value() * input.normals[b.index1()] + (1-b.value()) * input.normals[b.index2()],
					c.value() * input.normals[c.index1()] + (1-c.value()) * input.normals[c.index2()]
				}}
			);
	}
};
}
}
}

#endif
