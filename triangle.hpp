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
	std::array<vec3,3>
	vertex_array;

	typedef
	std::array<vec2,3>
	texcoord_array;

	vertex_array vertices;
	texcoord_array texcoords;

	triangle(
		std::initializer_list<vec3> const &_vertices,
		std::initializer_list<vec2> const &_texcoords)
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
