#ifndef FRUITCUT_APP_FRUIT_TRIANGLE_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_TRIANGLE_HPP_INCLUDED

#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vector2.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/container/array.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
class triangle
{
public:
	typedef
	sge::renderer::vector3
	vector;

	typedef
	sge::renderer::vector2
	texcoord_type;

	typedef
	fcppt::container::array<vector,3>
	vertex_array;

	typedef
	fcppt::container::array<vector,3>
	normal_array;

	typedef
	fcppt::container::array<texcoord_type,3>
	texcoord_array;

	typedef
	vertex_array::size_type
	size_type;

	vertex_array vertices;
	texcoord_array texcoords;
	normal_array normals;

	explicit
	triangle()
	:
		vertices(),
		texcoords(),
		normals()
	{
	}

	explicit
	triangle(
		vertex_array const &_vertices,
		texcoord_array const &_texcoords,
		normal_array const &_normals)
	:
		vertices(
			_vertices),
		texcoords(
			_texcoords),
		normals(
			_normals)
	{
	}
};
}
}
}

#endif
