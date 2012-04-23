#include <fruitapp/fruit/mesh.hpp>

fruitapp::fruit::mesh::mesh(
	triangle_sequence const &_triangles)
:
	triangles_(
		_triangles)
{
}

fruitapp::fruit::mesh::triangle_sequence const &
fruitapp::fruit::mesh::triangles() const
{
	return
		triangles_;
}

fruitapp::fruit::mesh::triangle_sequence &
fruitapp::fruit::mesh::triangles()
{
	return triangles_;
}

fruitapp::fruit::mesh::~mesh()
{
}
