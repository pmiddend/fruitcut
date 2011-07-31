#ifndef FRUITCUT_APP_FRUIT_TRIANGLE_TRAITS_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_TRIANGLE_TRAITS_HPP_INCLUDED

#include "triangle.hpp"
#include "../../fruitlib/math/triangle/vector_type.hpp"
#include "../../fruitlib/math/triangle/interpolation_pair.hpp"
#include "../../fruitlib/math/triangle/vertex_access.hpp"
#include "../../fruitlib/math/triangle/scalar_type.hpp"
#include "../../fruitlib/math/triangle/create_from_interpolation.hpp"
#include "../../fruitlib/math/triangle/size_type.hpp"

namespace fruitlib
{
namespace math
{
namespace triangle
{
template<>
struct vector_type<fruitcut::app::fruit::triangle>
{
	typedef
	fruitcut::app::fruit::triangle::vector 
	type;
};

template<>
struct scalar_type<fruitcut::app::fruit::triangle>
{
	typedef
	fruitcut::app::fruit::triangle::vector::value_type
	type;
};

template<>
struct vertex_access<fruitcut::app::fruit::triangle>
{
	static fruitcut::app::fruit::triangle::vector const &
	get(
		fruitcut::app::fruit::triangle const &t,
		fruitlib::math::triangle::size_type const n)
	{
		return t.vertices[n];
	}
};

template<>
struct create_from_interpolation<fruitcut::app::fruit::triangle>
{
	static fruitcut::app::fruit::triangle const
	create(
		fruitcut::app::fruit::triangle const &input,
		fruitlib::math::triangle::interpolation_pair<fruitcut::app::fruit::triangle> const &a,
		fruitlib::math::triangle::interpolation_pair<fruitcut::app::fruit::triangle> const &b,
		fruitlib::math::triangle::interpolation_pair<fruitcut::app::fruit::triangle> const &c)
	{
		fruitcut::app::fruit::triangle result;
		result.vertices[0] = a.value() * input.vertices[a.index1()] + (1-a.value()) * input.vertices[a.index2()];
		result.vertices[1] = b.value() * input.vertices[b.index1()] + (1-b.value()) * input.vertices[b.index2()];
		result.vertices[2] = c.value() * input.vertices[c.index1()] + (1-c.value()) * input.vertices[c.index2()];
		result.texcoords[0] = a.value() * input.texcoords[a.index1()] + (1-a.value()) * input.texcoords[a.index2()];
		result.texcoords[1] = b.value() * input.texcoords[b.index1()] + (1-b.value()) * input.texcoords[b.index2()];
		result.texcoords[2] = c.value() * input.texcoords[c.index1()] + (1-c.value()) * input.texcoords[c.index2()];
		result.normals[0] = a.value() * input.normals[a.index1()] + (1-a.value()) * input.normals[a.index2()];
		result.normals[1] = b.value() * input.normals[b.index1()] + (1-b.value()) * input.normals[b.index2()];
		result.normals[2] = c.value() * input.normals[c.index1()] + (1-c.value()) * input.normals[c.index2()];
		return result;
	}
};
}
}
}

#endif
