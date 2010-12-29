#include "permanent_sprite.hpp"
#include <sge/sprite/parameters_impl.hpp>
#include <iostream>

fruitcut::particle::objects::permanent_sprite::permanent_sprite(
	sprite::parameters const &params)
:
	sprite_(
		params.elements())
{
}

void
fruitcut::particle::objects::permanent_sprite::update()
{
	std::cout << "update\n";
}

bool
fruitcut::particle::objects::permanent_sprite::dead() const
{
	return false;
}

fruitcut::particle::sprite::object &
fruitcut::particle::objects::permanent_sprite::sprite()
{
	return sprite_;
}

fruitcut::particle::sprite::object const &
fruitcut::particle::objects::permanent_sprite::sprite() const
{
	return sprite_;
}
