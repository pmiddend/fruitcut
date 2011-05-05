#include "mesh_to_point_cloud.hpp"
#include "mesh.hpp"
#include "triangle.hpp"
#include <boost/foreach.hpp>

fruitcut::app::fruit::point_cloud const
fruitcut::app::fruit::mesh_to_point_cloud(
	mesh const &m)
{
	point_cloud output;
	output.reserve(
		static_cast<point_cloud::size_type>(
			m.triangles.size() * 3));

	BOOST_FOREACH(
		triangle const &t,
		m.triangles)
		BOOST_FOREACH(
			triangle::vertex_array::const_reference r,
			t.vertices)
			output.push_back(
				r);

	return output;
}
