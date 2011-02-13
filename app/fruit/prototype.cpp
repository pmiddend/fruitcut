#include "prototype.hpp"

fruitcut::app::fruit::prototype::prototype(
	fruitcut::app::mesh const &_mesh,
	sge::renderer::texture_ptr const _texture)
:
	mesh_(
		_mesh),
	texture_(
		_texture)
{
}

fruitcut::app::mesh const &
fruitcut::app::fruit::prototype::mesh() const
{
	return mesh_;
}

sge::renderer::texture_ptr const 
fruitcut::app::fruit::prototype::texture() const
{
	return texture_;
}
