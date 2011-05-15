#include "cut_mesh_result.hpp"

fruitcut::app::fruit::cut_mesh_result::cut_mesh_result()
:
	mesh_(),
	bounding_box_(),
	area_(),
	barycenter_()
{
}

fruitcut::app::fruit::mesh const &
fruitcut::app::fruit::cut_mesh_result::mesh() const
{
	return mesh_;
}

fruitcut::app::fruit::mesh &
fruitcut::app::fruit::cut_mesh_result::mesh()
{
	return mesh_;
}

fruitcut::app::fruit::box3 const &
fruitcut::app::fruit::cut_mesh_result::bounding_box() const
{
	return bounding_box_;
}

fruitcut::app::fruit::box3 &
fruitcut::app::fruit::cut_mesh_result::bounding_box()
{
	return bounding_box_;
}

fruitcut::app::fruit::area &
fruitcut::app::fruit::cut_mesh_result::area()
{
	return area_;
}

fruitcut::app::fruit::area const &
fruitcut::app::fruit::cut_mesh_result::area() const
{
	return area_;
}

sge::renderer::vector3 &
fruitcut::app::fruit::cut_mesh_result::barycenter()
{
	return barycenter_;
}

sge::renderer::vector3 const &
fruitcut::app::fruit::cut_mesh_result::barycenter() const
{
	return barycenter_;
}

fruitcut::app::fruit::cut_mesh_result::~cut_mesh_result()
{
}
