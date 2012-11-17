#include <fruitapp/font_particle/object.hpp>
#include <sge/font/text.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <sge/font/object.hpp>
#include <fruitlib/font/object.hpp>
#include <fruitlib/font/object_parameters.hpp>
#include <sge/font/text_parameters.hpp>
#include <fcppt/math/box/object_impl.hpp>


namespace
{
sge::font::rect const
calculate_bounding_box(
	sge::renderer::vector2 const &_position,
	fruitlib::font::object &_object)
{
	sge::font::rect const calculated_rect(
		_object.font_object().create_text(
			_object.text(),
			sge::font::text_parameters(
				sge::font::align_h::left))->rect());

	return
		sge::font::rect(
			sge::font::rect::vector(
				static_cast<sge::font::unit>(
					_position.x() -
					static_cast<sge::renderer::scalar>(
						calculated_rect.size().w()/2) +
					static_cast<sge::renderer::scalar>(
						calculated_rect.pos().x())),
				static_cast<sge::font::unit>(
					_position.y() -
					static_cast<sge::renderer::scalar>(
						calculated_rect.size().h()/2) +
					static_cast<sge::renderer::scalar>(
						calculated_rect.pos().y()))),
			2*calculated_rect.size());
}
}

fruitapp::font_particle::object::object(
	fruitlib::scenic::optional_parent const &_parent,
	fruitlib::font::manager &_font_manager,
	fruitlib::font::identifier const &_font_identifier,
	fruitapp::font_particle::text const &_text,
	fruitapp::font_particle::position const &_position,
	fruitapp::ingame_clock const &_clock,
	fruitapp::font_particle::lifetime const &_lifetime,
	sge::image::color::any::object const &_color)
:
	node_(
		_parent,
		fruitlib::font::object_parameters(
			_font_manager,
			_font_identifier,
			_text.get(),
			sge::font::rect::null(),
			sge::font::align_h::right,
			fruitlib::font::align_v::top,
			sge::font::flags_field::null(),
			_color,
			fruitlib::font::scale(
				1.f))),
	lifetimer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			_lifetime))
{
	node_.object().bounding_box(
		calculate_bounding_box(
			_position.get(),
			node_.object()));
}

bool
fruitapp::font_particle::object::dead() const
{
	return
		lifetimer_.expired();
}

void
fruitapp::font_particle::object::update()
{
}

fruitapp::font_particle::object::~object()
{
}
