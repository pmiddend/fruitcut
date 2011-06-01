#include "permanent_sprite.hpp"
#include <sge/sprite/parameters_impl.hpp>
#include <iostream>

fruitcut::fruitlib::particle::objects::permanent_sprite::permanent_sprite(
	sprite::parameters const &params)
:
	sprite_(
		params.elements())
{
}

void
fruitcut::fruitlib::particle::objects::permanent_sprite::update()
{
}

bool
fruitcut::fruitlib::particle::objects::permanent_sprite::dead() const
{
	return false;
}

fruitcut::fruitlib::particle::sprite::object &
fruitcut::fruitlib::particle::objects::permanent_sprite::sprite()
{
	return sprite_;
}

fruitcut::fruitlib::particle::sprite::object const &
fruitcut::fruitlib::particle::objects::permanent_sprite::sprite() const
{
	return sprite_;
}
