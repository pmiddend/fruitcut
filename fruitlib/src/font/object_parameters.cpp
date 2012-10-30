#include <fruitlib/font/object_parameters.hpp>


fruitlib::font::object_parameters::object_parameters(
	sge::renderer::device::ffp &_renderer,
	sge::font::object &_font_object,
	sge::font::string const &_text,
	sge::font::rect const &_bounding_box,
	sge::font::align_h::type const _alignment_h,
	fruitlib::font::align_v::type const _alignment_v,
	sge::font::flags_field const &_flags,
	sge::image::color::any::object const &_color,
	fruitlib::font::scale const &_scale,
	sge::renderer::texture::emulate_srgb::type const _emulate_srgb)
:
	renderer_(
		_renderer),
	font_object_(
		_font_object),
	text_(
		_text),
	bounding_box_(
		_bounding_box),
	alignment_h_(
		_alignment_h),
	alignment_v_(
		_alignment_v),
	flags_(
		_flags),
	color_(
		_color),
	scale_(
		_scale),
	emulate_srgb_(
		_emulate_srgb)
{
}

sge::renderer::device::ffp &
fruitlib::font::object_parameters::renderer() const
{
	return
		renderer_;
}

sge::font::object &
fruitlib::font::object_parameters::font_object() const
{
	return
		font_object_;
}

sge::font::string const &
fruitlib::font::object_parameters::text() const
{
	return
		text_;
}

sge::font::rect const &
fruitlib::font::object_parameters::bounding_box() const
{
	return
		bounding_box_;
}

sge::font::align_h::type
fruitlib::font::object_parameters::alignment_h() const
{
	return
		alignment_h_;
}

fruitlib::font::align_v::type
fruitlib::font::object_parameters::alignment_v() const
{
	return
		alignment_v_;
}

sge::font::flags_field const &
fruitlib::font::object_parameters::flags() const
{
	return
		flags_;
}

sge::image::color::any::object const &
fruitlib::font::object_parameters::color() const
{
	return
		color_;
}

fruitlib::font::scale const &
fruitlib::font::object_parameters::scale() const
{
	return
		scale_;
}

sge::renderer::texture::emulate_srgb::type
fruitlib::font::object_parameters::emulate_srgb() const
{
	return
		emulate_srgb_;
}
