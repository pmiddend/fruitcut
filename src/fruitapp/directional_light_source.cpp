#include <fruitapp/directional_light_source.hpp>
#include <sge/renderer/vector3.hpp>
#include <sge/renderer/matrix4.hpp>
#include <fcppt/math/matrix/translation.hpp>
#include <fcppt/math/matrix/arithmetic.hpp>

fruitapp::directional_light_source::directional_light_source()
:
	position_(),
	transformation_()
{
}

sge::renderer::vector3 const &
fruitapp::directional_light_source::position() const
{
	return 
		position_;
}

void	
fruitapp::directional_light_source::position(
	sge::renderer::vector3 const &_position)
{
	position_ = 
		_position;
}

sge::renderer::matrix4 const 
fruitapp::directional_light_source::model_view() const
{
	return 
		transformation_ *
		fcppt::math::matrix::translation(
			-position_);
}

void
fruitapp::directional_light_source::transformation(
	sge::renderer::matrix4 const &_transformation)
{
	transformation_ = 
		_transformation;
}

fruitapp::directional_light_source::~directional_light_source()
{
}
