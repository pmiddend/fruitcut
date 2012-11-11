#ifndef FRUITAPP_FRUIT_CUT_MESH_RESULT_HPP_INCLUDED
#define FRUITAPP_FRUIT_CUT_MESH_RESULT_HPP_INCLUDED

#include <fruitapp/fruit/area.hpp>
#include <fruitapp/fruit/box3.hpp>
#include <fruitapp/fruit/mesh_unique_ptr.hpp>
#include <sge/renderer/vector3.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/preprocessor/const.hpp>
#include <fcppt/preprocessor/warn_unused_result.hpp>


namespace fruitapp
{
namespace fruit
{
class cut_mesh_result
{
FCPPT_NONCOPYABLE(
	cut_mesh_result);
public:
	cut_mesh_result();

	fruitapp::fruit::mesh const &
	mesh() const
	FCPPT_PP_CONST;

	fruitapp::fruit::mesh &
	mesh()
	FCPPT_PP_CONST;

	fruitapp::fruit::mesh_unique_ptr
	release_mesh()
	FCPPT_PP_WARN_UNUSED_RESULT;

	fruitapp::fruit::mesh const &
	cross_section() const
	FCPPT_PP_CONST;

	fruitapp::fruit::mesh &
	cross_section()
	FCPPT_PP_CONST;

	fruitapp::fruit::box3 const &
	bounding_box() const
	FCPPT_PP_CONST;

	fruitapp::fruit::box3 &
	bounding_box()
	FCPPT_PP_CONST;

	fruitapp::fruit::area &
	area()
	FCPPT_PP_CONST;

	fruitapp::fruit::area const &
	area() const
	FCPPT_PP_CONST;

	sge::renderer::vector3 &
	barycenter()
	FCPPT_PP_CONST;

	sge::renderer::vector3 const &
	barycenter() const
	FCPPT_PP_CONST;

	~cut_mesh_result();
private:
	fruit::mesh_unique_ptr mesh_;
	fruit::mesh_unique_ptr cross_section_;
	fruit::box3 bounding_box_;
	fruit::area area_;
	sge::renderer::vector3 barycenter_;
};
}
}

#endif
