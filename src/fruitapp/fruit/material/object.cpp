#include <fruitapp/fruit/material/object.hpp>

fruitapp::fruit::material::object::object(
	material::diffuse_color const &_diffuse_color,
	material::diffuse_coefficient const &_diffuse_coefficient,
	material::specular_color const &_specular_color,
	material::specular_coefficient const &_specular_coefficient,
	material::specular_shininess const &_specular_shininess)
:
	diffuse_color_(
		_diffuse_color.get()),
	diffuse_coefficient_(
		_diffuse_coefficient.get()),
	specular_color_(
		_specular_color.get()),
	specular_coefficient_(
		_specular_coefficient.get()),
	specular_shininess_(
		_specular_shininess.get())
{
}

fruitapp::fruit::material::diffuse_color::value_type const
fruitapp::fruit::material::object::diffuse_color() const
{
	return diffuse_color_;
}

fruitapp::fruit::material::diffuse_coefficient::value_type
fruitapp::fruit::material::object::diffuse_coefficient() const
{
	return diffuse_coefficient_;
}

fruitapp::fruit::material::specular_color::value_type const
fruitapp::fruit::material::object::specular_color() const
{
	return specular_color_;
}

fruitapp::fruit::material::specular_coefficient::value_type
fruitapp::fruit::material::object::specular_coefficient() const
{
	return specular_coefficient_;
}

fruitapp::fruit::material::specular_shininess::value_type
fruitapp::fruit::material::object::specular_shininess() const
{
	return specular_shininess_;
}
