#include <fruitlib/font/scene_node.hpp>
#include <fruitlib/scenic/events/render.hpp>

#if 0
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
#endif

fruitlib::font::scene_node::scene_node(
	fruitlib::scenic::optional_parent const &_parent,
	fruitlib::font::object_parameters const &_params)
:
	node_base(
		_parent),
	object_(
		_params)
{
}

fruitlib::font::object &
fruitlib::font::scene_node::object()
{
	return
		object_;
}

fruitlib::font::object const &
fruitlib::font::scene_node::object() const
{
	return
		object_;
}

void
fruitlib::font::scene_node::react(
	fruitlib::scenic::events::render const &_render_event)
{
#if 0
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
#endif

	object_.render(
		_render_event.context());
}

