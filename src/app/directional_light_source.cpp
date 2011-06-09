#include "directional_light_source.hpp"
#include <fcppt/math/matrix/matrix.hpp>

fruitcut::app::directional_light_source::directional_light_source()
:
	position_(),
	transformation_()
{
}

sge::renderer::vector3 const &
fruitcut::app::directional_light_source::position() const
{
	return 
		position_;
}

void	
fruitcut::app::directional_light_source::position(
	sge::renderer::vector3 const &_position)
{
	position_ = 
		_position;
}

sge::renderer::matrix4 const 
fruitcut::app::directional_light_source::model_view() const
{
	return 
		transformation_ *
		fcppt::math::matrix::translation(
			-position_);
}

void
fruitcut::app::directional_light_source::transformation(
	sge::renderer::matrix4 const &_transformation)
{
	transformation_ = 
		_transformation;
}

fruitcut::app::directional_light_source::~directional_light_source()
{
}
