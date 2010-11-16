#ifndef FRUITCUT_TRIANGLE_HPP_INCLUDED
#define FRUITCUT_TRIANGLE_HPP_INCLUDED

#include "vec2.hpp"
#include "vec3.hpp"
#include <fcppt/assert.hpp>
#include <array>
#include <algorithm>
#include <initializer_list>

namespace fruitcut
{
class triangle
{
public:
	typedef
	vec3
	vector;

	typedef
	vec2
	data_type;

	typedef
	std::array<vector,3>
	vertex_array;

	typedef
	std::array<data_type,3>
	texcoord_array;

	typedef
	vertex_array::size_type
	size_type;

	vertex_array vertices;
	texcoord_array texcoords;

	triangle(
		std::initializer_list<vector> const &_vertices,
		std::initializer_list<data_type> const &_texcoords)
	{
		FCPPT_ASSERT(
			_vertices.size() == _texcoords.size() && _vertices.size() == 3);

		std::copy(
			_vertices.begin(),_vertices.end(),vertices.begin());
		std::copy(
			_texcoords.begin(),_texcoords.end(),texcoords.begin());
	}
};
}

#endif
