#include <fruitlib/font/object.hpp>
#include <fruitlib/font/object_parameters.hpp>
#include <sge/font/text_parameters.hpp>
#include <sge/font/draw/static_text.hpp>
#include <sge/image/colors.hpp>
#include <sge/image/color/any/object.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>


fruitlib::font::object::object(
	fruitlib::font::object_parameters const &p)
:
	renderer_(
		p.renderer()),
	font_object_(
		p.font_object()),
	text_(
		p.text()),
	bounding_box_(
		p.bounding_box()),
	alignment_h_(
		p.alignment_h()),
	alignment_v_(
		p.alignment_v()),
	flags_(
		p.flags()),
	color_(
		p.color()),
	scale_(
		p.scale()),
	text_object_()
{
	this->rebuild_text_object();
}

void
fruitlib::font::object::render(
	sge::renderer::context::ffp &_context)
{
	text_object_->draw(
		_context);
}

sge::font::object &
fruitlib::font::object::font_object() const
{
	return
	font_object_;
}

sge::font::string const &
fruitlib::font::object::text() const
{
	return
		text_;
}

void
fruitlib::font::object::text(
	sge::font::string const &_text)
{
	text_ =
		_text;

	text_object_->string(
		text_);
}

sge::font::rect const &
fruitlib::font::object::bounding_box() const
{
	return
		bounding_box_;
}

void
fruitlib::font::object::bounding_box(
	sge::font::rect const &_bounding_box)
{
	bounding_box_ =
		_bounding_box;

	this->rebuild_text_object();
}

sge::font::align_h::type
fruitlib::font::object::alignment_h() const
{
	return
		alignment_h_;
}

void
fruitlib::font::object::alignment_h(
	sge::font::align_h::type const _alignment_h)
{
	alignment_h_ =
		_alignment_h;

	this->rebuild_text_object();
}

fruitlib::font::align_v::type
fruitlib::font::object::alignment_v() const
{
	return
		alignment_v_;
}

void
fruitlib::font::object::alignment_v(
	fruitlib::font::align_v::type const _alignment_v)
{
	alignment_v_ =
		_alignment_v;

	this->rebuild_text_object();
}

sge::font::flags_field const &
fruitlib::font::object::flags() const
{
	return flags_;
}

void
fruitlib::font::object::flags(
	sge::font::flags_field const &_flags)
{
	flags_ =
		_flags;

	this->rebuild_text_object();
}

sge::image::color::any::object const &
fruitlib::font::object::color() const
{
	return
		color_;
}

void
fruitlib::font::object::color(
	sge::image::color::any::object const &_color)
{
	color_ =
		_color;

	text_object_->color(
		_color);
}

fruitlib::font::scale const &
fruitlib::font::object::scale() const
{
	return
		scale_;
}

void
fruitlib::font::object::scale(
	fruitlib::font::scale const &_scale)
{
	scale_ =
		_scale;
}

fruitlib::font::object::~object()
{
}

void
fruitlib::font::object::rebuild_text_object()
{
	text_object_.take(
		fcppt::make_unique_ptr<sge::font::draw::static_text>(
			fcppt::ref(
				renderer_),
			fcppt::ref(
				font_object_),
			text_,
			sge::font::text_parameters(
				alignment_h_)
				.flags(
					flags_)
				.max_width(
					bounding_box_.w()),
			bounding_box_.pos(),
			color_));

	text_object_->pos(
		sge::font::vector(
			text_object_->pos().x(),
			bounding_box_.pos().y() +
			bounding_box_.size().w()/2 -
			text_object_->rect().size().h()/2));
}
