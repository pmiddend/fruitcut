#include <fruitapp/fruit/mesh.hpp>
#include <fruitapp/fruit/mesh_to_point_cloud.hpp>
#include <fruitapp/fruit/triangle.hpp>


fruitapp::fruit::point_cloud
fruitapp::fruit::mesh_to_point_cloud(
	fruitapp::fruit::mesh const &m
)
{
	point_cloud output;
	output.reserve(
		static_cast<point_cloud::size_type>(
			m.triangles().size() * 3));

	for(
		fruit::mesh::triangle_sequence::const_iterator t =
			m.triangles().begin();
		t != m.triangles().end();
		++t)
	{
		for(
			triangle::vertex_array::const_iterator r =
				t->vertices.begin();
			r != t->vertices.end();
			++r)
			output.push_back(
				point_cloud::value_type(
					*r
				)
			);
	}

	return output;
}
