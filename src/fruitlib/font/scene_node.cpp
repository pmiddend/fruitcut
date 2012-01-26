#include <fruitlib/font/scene_node.hpp>
#include <fruitlib/font/drawer/object.hpp>
#include <fruitlib/font/drawer/scoped_color.hpp>
#include <fruitlib/font/drawer/scoped_transformation.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/font/pos.hpp>
#include <sge/font/rect.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/tr1/functional.hpp>


namespace
{
sge::font::rect const
scale_transformation(
	sge::font::rect const &total_rect,
	sge::font::rect const &character_rect,
	fruitlib::font::scale::value_type const s)
{
	typedef
	fruitlib::font::scale::value_type
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

fruitlib::font::scene_node::scene_node(
	fruitlib::scenic::optional_parent const &_parent,
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

fruitlib::font::color const
fruitlib::font::scene_node::color() const
{
	return color_;
}

void
fruitlib::font::scene_node::color(
	fruitlib::font::color const &_color)
{
	color_ =
		_color;
}

fruitlib::font::scale::value_type
fruitlib::font::scene_node::scale() const
{
	return scale_;
}

void
fruitlib::font::scene_node::scale(
	fruitlib::font::scale::value_type const _scale)
{
	scale_ =
		_scale;
}

fruitlib::font::object &
fruitlib::font::scene_node::object()
{
	return object_;
}

fruitlib::font::object const &
fruitlib::font::scene_node::object() const
{
	return object_;
}

void
fruitlib::font::scene_node::react(
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

