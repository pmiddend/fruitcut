#include "cut_mesh.hpp"
#include "math/cut_triangle_at_plane.hpp"
#include "triangle.hpp"
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/algorithm/std/copy.hpp>
#include <fcppt/algorithm/std/accumulate.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/math/vector/arithmetic.hpp>
#include <boost/foreach.hpp>
#include <algorithm>
#include <iterator>

namespace
{
fruitcut::triangle const
create_triangle(
	sge::renderer::vector3 const &p0,
	sge::renderer::vector2 const &t0,
	sge::renderer::vector3 const &p1,
	sge::renderer::vector2 const &t1,
	sge::renderer::vector3 const &p2,
	sge::renderer::vector2 const &t2)
{
	fruitcut::triangle::vertex_array const vertices = {{ p0,p1,p2 }};
	fruitcut::triangle::texcoord_array const texcoords = {{ t0,t1,t2 }};
	return 
		fruitcut::triangle(
			vertices,
			texcoords);
}

sge::renderer::vector3 const
get_position(
	fruitcut::triangle const &t,
	fruitcut::triangle::size_type const i)
{
	return t.vertices[i];
}

sge::renderer::vector2 const
get_data(
	fruitcut::triangle const &t,
	fruitcut::triangle::size_type const i)
{
	return t.texcoords[i];
}

sge::renderer::vector2 const
interpolate(
	sge::renderer::vector2 const &a,
	sge::renderer::vector2 const &b,
	sge::renderer::scalar c)
{
	return a * c + (static_cast<sge::renderer::scalar>(1)-c) * b;
}
}

fruitcut::mesh const
fruitcut::cut_mesh(
	mesh const &m,
	plane const &p)
{
	mesh result;

	using namespace boost::phoenix::arg_names;

	BOOST_FOREACH(
		mesh::triangle_sequence::const_reference t,
		m.triangles)
		fcppt::algorithm::std::copy(
			fruitcut::math::cut_triangle_at_plane(
				t,
				p,
				&get_position,
				&get_data,
				&interpolate,
				&create_triangle).triangles(),
			std::back_inserter(
				result.triangles));

	return result;
}
