#include "cut_mesh.hpp"
#include "math/cut_triangle_at_plane.hpp"
#include "triangle.hpp"
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/scalar.hpp>
#include <fcppt/algorithm/std/copy.hpp>
#include <fcppt/algorithm/std/accumulate.hpp>
#include <boost/foreach.hpp>
#include <algorithm>
#include <iterator>

fruitcut::mesh const
fruitcut::cut_mesh(
	mesh const &m,
	plane const &p)
{
	mesh result;

	BOOST_FOREACH(
		mesh::triangle_sequence::const_reference t,
		m.triangles)
		fcppt::algorithm::std::copy(
			fruitcut::math::cut_triangle_at_plane(
				t,
				p,
				[](triangle const &t,triangle::size_type const i)
				{
					return t.vertices[i];
				},
				[](triangle const &t,triangle::size_type const i)
				{
					return t.texcoords[i];
				},
				[](sge::renderer::vector2 const &a, sge::renderer::vector2 const &b,sge::renderer::scalar c)
				{ 
					return a * c + (static_cast<sge::renderer::scalar>(1)-c) * b;
				},
				[](
					sge::renderer::vector3 const &p0,
					sge::renderer::vector2 const &t0,
					sge::renderer::vector3 const &p1,
					sge::renderer::vector2 const &t1,
					sge::renderer::vector3 const &p2,
					sge::renderer::vector2 const &t2)
				{
					return 
						triangle(
							{p0,p1,p2},
							{t0,t1,t2});
				}).triangles(),
			std::back_inserter(
				result.triangles));

	return result;
}
