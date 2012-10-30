#ifndef FRUITAPP_FRUIT_MATERIAL_OBJECT_HPP_INCLUDED
#define FRUITAPP_FRUIT_MATERIAL_OBJECT_HPP_INCLUDED

#include <fruitapp/fruit/material/diffuse_coefficient.hpp>
#include <fruitapp/fruit/material/diffuse_color.hpp>
#include <fruitapp/fruit/material/specular_coefficient.hpp>
#include <fruitapp/fruit/material/specular_color.hpp>
#include <fruitapp/fruit/material/specular_shininess.hpp>


namespace fruitapp
{
namespace fruit
{
namespace material
{
class object
{
public:
	object(
		fruitapp::fruit::material::diffuse_color const &,
		fruitapp::fruit::material::diffuse_coefficient const &,
		fruitapp::fruit::material::specular_color const &,
		fruitapp::fruit::material::specular_coefficient const &,
		fruitapp::fruit::material::specular_shininess const &);

	fruitapp::fruit::material::diffuse_color::value_type const
	diffuse_color() const;

	fruitapp::fruit::material::diffuse_coefficient::value_type
	diffuse_coefficient() const;

	fruitapp::fruit::material::specular_color::value_type const
	specular_color() const;

	fruitapp::fruit::material::specular_coefficient::value_type
	specular_coefficient() const;

	fruitapp::fruit::material::specular_shininess::value_type
	specular_shininess() const;
private:
	fruitapp::fruit::material::diffuse_color::value_type diffuse_color_;
	fruitapp::fruit::material::diffuse_coefficient::value_type diffuse_coefficient_;
	fruitapp::fruit::material::specular_color::value_type specular_color_;
	fruitapp::fruit::material::specular_coefficient::value_type specular_coefficient_;
	fruitapp::fruit::material::specular_shininess::value_type specular_shininess_;
};
}
}
}

#endif
