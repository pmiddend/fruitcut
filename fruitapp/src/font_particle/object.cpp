#include <fruitapp/font_particle/object.hpp>
#include <fruitlib/font/align_h.hpp>
#include <fruitlib/font/object.hpp>
#include <fruitlib/font/object_parameters.hpp>
#include <sge/font/flags_field.hpp>
#include <sge/font/object.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/text.hpp>
#include <sge/font/text_parameters.hpp>
#include <sge/font/unit.hpp>
#include <sge/font/align_h/left.hpp>
#include <sge/font/align_h/variant.hpp>
#include <sge/image/color/any/object.hpp>
#include <sge/renderer/scalar.hpp>
#include <sge/renderer/vector2.hpp>
#include <sge/renderer/vector2.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/math/dim/arithmetic.hpp>
#include <fcppt/math/vector/arithmetic.hpp>


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
				sge::font::align_h::variant(
					sge::font::align_h::left()
				)
			)
		)->rect()
	);

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
			fruitlib::font::align_h::right,
			fruitlib::font::align_v::top,
			sge::font::flags_field::null(),
			_color,
			fruitlib::font::scale(
				1.f))),
	lifetimer_(
		fruitapp::ingame_timer::parameters(
			_clock,
			_lifetime)),
	position_(
		_position),
	velocity_(
		sge::renderer::vector2(0.f,3.f))
{
	node_.object().bounding_box(
		calculate_bounding_box(
			position_.get(),
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
	position_ =
		fruitapp::font_particle::position(
			position_.get() + velocity_.get());
	velocity_ +=
		fruitapp::font_particle::velocity(
			sge::renderer::vector2(0.f, -0.5f));

	node_.object().bounding_box(
		calculate_bounding_box(
			position_.get(),
			node_.object()));
}

fruitapp::font_particle::object::~object()
{
}
