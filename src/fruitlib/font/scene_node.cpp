#include "scene_node.hpp"
#include "drawer/scoped_transformation.hpp"
#include "drawer/scoped_color.hpp"
#include "drawer/object.hpp"
#include "../scenic/events/render.hpp"
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
	fruitcut::fruitlib::font::scale::value_type const s)
{
	typedef
	fruitcut::fruitlib::font::scale::value_type
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

fruitcut::fruitlib::font::scene_node::scene_node(
	fruitlib::scenic::parent const &_parent,
	object_parameters const &_params,
	font::color const &_color,
	font::scale const &_scale)
:
	node_base(
		_parent),
	object_(
		_params),
	color_(
		_color),
	scale_(
		_scale.get())
{
}

fruitcut::fruitlib::font::color const
fruitcut::fruitlib::font::scene_node::color() const
{
	return color_;
}

void
fruitcut::fruitlib::font::scene_node::color(
	fruitcut::fruitlib::font::color const &_color)
{
	color_ = 
		_color;
}

fruitcut::fruitlib::font::scale::value_type
fruitcut::fruitlib::font::scene_node::scale() const
{
	return scale_;
}

void
fruitcut::fruitlib::font::scene_node::scale(
	fruitcut::fruitlib::font::scale::value_type const _scale)
{
	scale_ = 
		_scale;
}

fruitcut::fruitlib::font::object &
fruitcut::fruitlib::font::scene_node::object()
{
	return object_;
}

fruitcut::fruitlib::font::object const &
fruitcut::fruitlib::font::scene_node::object() const
{
	return object_;
}

void
fruitcut::fruitlib::font::scene_node::react(
	fruitlib::scenic::events::render const &)
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

