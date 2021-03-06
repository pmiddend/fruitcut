#ifndef FRUITAPP_FRUIT_PROTOTYPE_HPP_INCLUDED
#define FRUITAPP_FRUIT_PROTOTYPE_HPP_INCLUDED

#include <fruitapp/fruit/box3.hpp>
#include <fruitapp/fruit/mesh_unique_ptr.hpp>
#include <fruitapp/fruit/splatter_color.hpp>
#include <fruitapp/fruit/tag_set.hpp>
#include <fruitapp/fruit/material/object.hpp>
#include <fruitlib/physics/rigid_body/mass.hpp>
#include <sge/image/color/any/object.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/preprocessor/const.hpp>
#include <fcppt/preprocessor/pure.hpp>
#include <fcppt/variant/object.hpp>


namespace fruitapp
{
namespace fruit
{
class prototype
{
FCPPT_NONCOPYABLE(
	prototype);
public:
	prototype(
		fruitapp::fruit::mesh_unique_ptr,
		sge::renderer::texture::planar_shared_ptr,
		fruitapp::fruit::material::object const &,
		fruitapp::fruit::splatter_color const &,
		fruitapp::fruit::tag_set const &);

	fruitapp::fruit::mesh const &
	mesh() const
	FCPPT_PP_CONST;

	fruitapp::fruit::box3 const &
	bounding_box() const
	FCPPT_PP_CONST;

	fruitlib::physics::rigid_body::mass const
	mass() const
	FCPPT_PP_PURE;

	sge::renderer::texture::planar_shared_ptr const
	texture() const
	FCPPT_PP_PURE;

	sge::image::color::any::object const &
	splatter_color() const
	FCPPT_PP_CONST;

	fruitapp::fruit::material::object const &
	material() const
	FCPPT_PP_CONST;

	fruitapp::fruit::tag_set const &
	tags() const
	FCPPT_PP_CONST;

	~prototype();
private:
	fruitapp::fruit::mesh_unique_ptr const mesh_;
	// Storing the bounding box is not neccessary, but I don't want to
	// recalculate the bounding box each time in the spawner, so ...
	fruitapp::fruit::box3 bounding_box_;
	fruitlib::physics::rigid_body::mass mass_;
	sge::renderer::texture::planar_shared_ptr texture_;
	sge::image::color::any::object splatter_color_;
	fruitapp::fruit::material::object material_;
	fruitapp::fruit::tag_set tags_;
};
}
}

#endif
