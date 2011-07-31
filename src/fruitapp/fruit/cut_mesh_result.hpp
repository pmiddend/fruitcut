#ifndef FRUITAPP_FRUIT_CUT_MESH_RESULT_HPP_INCLUDED
#define FRUITAPP_FRUIT_CUT_MESH_RESULT_HPP_INCLUDED

#include "mesh.hpp"
#include "box3.hpp"
#include "area.hpp"
#include <sge/renderer/vector3.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/noncopyable.hpp>

namespace fruitapp
{
namespace fruit
{
class cut_mesh_result
{
FCPPT_NONCOPYABLE(
	cut_mesh_result);
public:
	explicit 
	cut_mesh_result();

	fruit::mesh const &
	mesh() const;

	fruit::mesh &
	mesh();

	fruit::mesh const &
	cross_section() const;

	fruit::mesh &
	cross_section();

	fruit::box3 const &
	bounding_box() const;

	fruit::box3 &
	bounding_box();

	fruit::area &
	area();

	fruit::area const &
	area() const;

	sge::renderer::vector3 &
	barycenter();

	sge::renderer::vector3 const &
	barycenter() const;

	~cut_mesh_result();
private:
	fruit::mesh mesh_;
	fruit::mesh cross_section_;
	fruit::box3 bounding_box_;
	fruit::area area_;
	sge::renderer::vector3 barycenter_;
};
}
}

#endif
