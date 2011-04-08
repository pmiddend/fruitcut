#include "scene_node.hpp"
#include "drawer/scoped_transformation.hpp"
#include "drawer/scoped_color.hpp"
#include "drawer/object.hpp"
#include "../scene/scale.hpp"
#include <sge/font/rect.hpp>
#include <sge/font/pos.hpp>
#include <fcppt/math/vector/vector.hpp>
#include <fcppt/math/dim/dim.hpp>
#include <fcppt/math/box/box.hpp>
#include <fcppt/tr1/functional.hpp>

namespace
{
sge::font::rect const
scale_transformation(
	sge::font::rect const &total_rect,
	sge::font::rect const &character_rect,
	fruitcut::scene::scale const s)
{
	typedef
	fruitcut::scene::scale
	real;

	sge::font::pos const c = 
		total_rect.pos() + total_rect.size()/2;
	return 
		sge::font::rect(
			sge::font::pos(
				static_cast<sge::font::pos::value_type>(
					static_cast<real>(
						c.x()) - 
					static_cast<real>(
						c.x()) * s + 
					static_cast<real>(
						character_rect.pos().x()) * s),
				static_cast<sge::font::pos::value_type>(
					static_cast<real>(
						c.y()) - 
					static_cast<real>(
						c.y()) * s + 
					static_cast<real>(
						character_rect.pos().y()) * s)),
			sge::font::dim(
				static_cast<sge::font::pos::value_type>(
					static_cast<real>(
						character_rect.size().w()) * s),
				static_cast<sge::font::pos::value_type>(
					static_cast<real>(
						character_rect.size().h()) * s)));
}
}

fruitcut::font::scene_node::scene_node(
	object_parameters const &_params)
:
	object_(
		_params)
{
}

void
fruitcut::font::scene_node::render()
{
	drawer::scoped_color scoped_color(
		object_.drawer(),
		color_);

	drawer::scoped_transformation scoped_transformation(
		object_.drawer(),
		std::tr1::bind(
			&scale_transformation,
			std::tr1::placeholders::_1,
			std::tr1::placeholders::_2,
			scale_));

	object_.render();
}

void
fruitcut::font::scene_node::update()
{
}

fruitcut::scene::color const
fruitcut::font::scene_node::color() const
{
	return color_;
}

void
fruitcut::font::scene_node::color(
	fruitcut::scene::color const &_color)
{
	color_ = 
		_color;
}
