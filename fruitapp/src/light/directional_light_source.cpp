#include <fruitapp/light/directional_light_source.hpp>
#include <sge/renderer/matrix4.hpp>
#include <sge/renderer/vector3.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/vector/arithmetic.hpp>


fruitapp::light::directional_light_source::directional_light_source()
:
	position_(
		sge::renderer::vector3::null()
	),
	transformation_(
		sge::renderer::matrix4::identity()
	)
{
}

sge::renderer::vector3 const &
fruitapp::light::directional_light_source::position() const
{
	return
		position_;
}

void
fruitapp::light::directional_light_source::position(
	sge::renderer::vector3 const &_position)
{
	position_ =
		_position;
}

sge::renderer::matrix4 const
fruitapp::light::directional_light_source::model_view() const
{
	return
		transformation_ *
		fcppt::math::matrix::translation(
			-position_);
}

void
fruitapp::light::directional_light_source::transformation(
	sge::renderer::matrix4 const &_transformation)
{
	transformation_ =
		_transformation;
}

sge::renderer::matrix4 const
fruitapp::light::directional_light_source::transformation() const
{
	return
		transformation_;
}

fruitapp::light::directional_light_source::~directional_light_source()
{
}
