#include <fruitlib/font/cache.hpp>
#include <fruitlib/font/manager.hpp>
#include <fruitlib/font/object_parameters.hpp>


fruitlib::font::object_parameters::object_parameters(
	fruitlib::font::manager &_manager,
	fruitlib::font::identifier const &_identifier,
	sge::font::string const &_text,
	sge::font::rect const &_bounding_box,
	sge::font::align_h const _alignment_h,
	fruitlib::font::align_v::type const _alignment_v,
	sge::font::flags_field const &_flags,
	sge::image::color::any::object const &_color,
	fruitlib::font::scale const &_scale)
:
	manager_(
		_manager),
	font_object_(
		_manager.cache().get(
			_identifier)),
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
		_scale)
{
}

fruitlib::font::manager &
fruitlib::font::object_parameters::manager() const
{
	return
		manager_;
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

sge::font::align_h
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
