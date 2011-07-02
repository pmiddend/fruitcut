#ifndef FRUITCUT_APP_FRUIT_PROTOTYPE_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_PROTOTYPE_HPP_INCLUDED

#include "mesh.hpp"
#include "box3.hpp"
#include "tag_set.hpp"
#include "material/object.hpp"
#include "../../fruitlib/physics/rigid_body/mass.hpp"
#include <sge/renderer/texture/planar_ptr.hpp>
#include <sge/image/color/any/object.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/variant/object.hpp>

namespace fruitcut
{
namespace app
{
namespace fruit
{
class prototype
{
public:
	explicit
	prototype(
		fruitcut::app::fruit::mesh const &,
		sge::renderer::texture::planar_ptr,
		material::object const &,
		fruit::tag_set const &);

	fruitcut::app::fruit::mesh const &
	mesh() const;

	box3 const &
	bounding_box() const;

	fruitlib::physics::rigid_body::mass::value_type
	mass() const;

	sge::renderer::texture::planar_ptr const 
	texture() const;

	sge::image::color::any::object const &
	splatter_color() const;

	material::object const &
	material() const;

	fruit::tag_set const &
	tags() const;
private:
	fruitcut::app::fruit::mesh mesh_;
	// Storing the bounding box is not neccessary, but I don't want to
	// recalculate the bounding box each time in the spawner, so ...
	box3 bounding_box_;
	fruitlib::physics::rigid_body::mass::value_type mass_;
	sge::renderer::texture::planar_ptr texture_;
	sge::image::color::any::object splatter_color_;
	material::object material_;
	fruit::tag_set tags_;
};
}
}
}

#endif
