#ifndef FRUITCUT_APP_FRUIT_MATERIAL_OBJECT_HPP_INCLUDED
#define FRUITCUT_APP_FRUIT_MATERIAL_OBJECT_HPP_INCLUDED

#include "diffuse_color.hpp"
#include "diffuse_coefficient.hpp"
#include "specular_color.hpp"
#include "specular_coefficient.hpp"
#include "specular_shininess.hpp"

namespace fruitcut
{
namespace app
{
namespace fruit
{
namespace material
{
class object
{
public:
	explicit
	object(
		material::diffuse_color const &,
		material::diffuse_coefficient const &,
		material::specular_color const &,
		material::specular_coefficient const &,
		material::specular_shininess const &);

	material::diffuse_color::value_type const
	diffuse_color() const;

	material::diffuse_coefficient::value_type
	diffuse_coefficient() const;

	material::specular_color::value_type const
	specular_color() const;

	material::specular_coefficient::value_type
	specular_coefficient() const;

	material::specular_shininess::value_type
	specular_shininess() const;
private:
	material::diffuse_color::value_type diffuse_color_;
	material::diffuse_coefficient::value_type diffuse_coefficient_;
	material::specular_color::value_type specular_color_;
	material::specular_coefficient::value_type specular_coefficient_; 
	material::specular_shininess::value_type specular_shininess_; 
};
}
}
}
}

#endif