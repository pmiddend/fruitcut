#include <fruitapp/fruit/cut_mesh_result.hpp>
#include <sge/renderer/vector3.hpp>


fruitapp::fruit::cut_mesh_result::cut_mesh_result()
:
	mesh_(),
	cross_section_(),
	bounding_box_(),
	area_(),
	barycenter_()
{
}

fruitapp::fruit::mesh const &
fruitapp::fruit::cut_mesh_result::mesh() const
{
	return mesh_;
}

fruitapp::fruit::mesh &
fruitapp::fruit::cut_mesh_result::mesh()
{
	return mesh_;
}

fruitapp::fruit::mesh &
fruitapp::fruit::cut_mesh_result::cross_section()
{
	return cross_section_;
}

fruitapp::fruit::mesh const &
fruitapp::fruit::cut_mesh_result::cross_section() const
{
	return cross_section_;
}

fruitapp::fruit::box3 const &
fruitapp::fruit::cut_mesh_result::bounding_box() const
{
	return bounding_box_;
}

fruitapp::fruit::box3 &
fruitapp::fruit::cut_mesh_result::bounding_box()
{
	return bounding_box_;
}

fruitapp::fruit::area &
fruitapp::fruit::cut_mesh_result::area()
{
	return area_;
}

fruitapp::fruit::area const &
fruitapp::fruit::cut_mesh_result::area() const
{
	return area_;
}

sge::renderer::vector3 &
fruitapp::fruit::cut_mesh_result::barycenter()
{
	return barycenter_;
}

sge::renderer::vector3 const &
fruitapp::fruit::cut_mesh_result::barycenter() const
{
	return barycenter_;
}

fruitapp::fruit::cut_mesh_result::~cut_mesh_result()
{
}
