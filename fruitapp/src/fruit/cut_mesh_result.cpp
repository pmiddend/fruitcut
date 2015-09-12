#include <fruitapp/fruit/cut_mesh_result.hpp>
#include <fruitapp/fruit/mesh.hpp>
#include <sge/renderer/vector3.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/math/box/null.hpp>
#include <fcppt/math/vector/null.hpp>
#include <fcppt/config/external_begin.hpp>
#include <utility>
#include <fcppt/config/external_end.hpp>


fruitapp::fruit::cut_mesh_result::cut_mesh_result()
:
	mesh_(
		fcppt::make_unique_ptr<fruit::mesh>(
			fruit::mesh::triangle_sequence())),
	cross_section_(
		fcppt::make_unique_ptr<fruit::mesh>(
			fruit::mesh::triangle_sequence())),
	bounding_box_(
		fcppt::math::box::null<
			fruitapp::fruit::box3
		>()
	),
	area_(
		0.f),
	barycenter_(
		fcppt::math::vector::null<
			sge::renderer::vector3
		>()
	)
{
}

fruitapp::fruit::mesh const &
fruitapp::fruit::cut_mesh_result::mesh() const
{
	return
		*mesh_;
}

fruitapp::fruit::mesh &
fruitapp::fruit::cut_mesh_result::mesh()
{
	return
		*mesh_;
}

fruitapp::fruit::mesh_unique_ptr
fruitapp::fruit::cut_mesh_result::release_mesh()
{
	return
		std::move(
			mesh_);
}

fruitapp::fruit::mesh &
fruitapp::fruit::cut_mesh_result::cross_section()
{
	return *cross_section_;
}

fruitapp::fruit::mesh const &
fruitapp::fruit::cut_mesh_result::cross_section() const
{
	return *cross_section_;
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
