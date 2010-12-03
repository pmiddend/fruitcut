#ifndef FRUITCUT_TRIANGLE_HPP_INCLUDED
#define FRUITCUT_TRIANGLE_HPP_INCLUDED

#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vector2.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/tr1/array.hpp>

namespace fruitcut
{
class triangle
{
public:
	typedef
	sge::renderer::vector3
	vector;

	typedef
	sge::renderer::vector2
	data_type;

	typedef
	std::tr1::array<vector,3>
	vertex_array;

	typedef
	std::tr1::array<data_type,3>
	texcoord_array;

	typedef
	vertex_array::size_type
	size_type;

	vertex_array vertices;
	texcoord_array texcoords;

	explicit
	triangle(
		vertex_array const &_vertices,
		texcoord_array const &_texcoords)
	:
		vertices(
			_vertices),
		texcoords(
			_texcoords)
	{
	}
};
}

#endif
