#include "object_parameters.hpp"

fruitlib::font::object_parameters::object_parameters(
	sge::font::metrics_ptr const _metrics,
	font::drawer::object &_drawer,
	sge::font::text::string const &_text,
	sge::font::rect const &_bounding_box,
	sge::font::text::align_h::type const _alignment_h,
	sge::font::text::align_v::type const _alignment_v,
	sge::font::text::flags_field const &_flags)
:
	metrics_(
		_metrics),
	drawer_(
		_drawer),
	text_(
		_text),
	bounding_box_(
		_bounding_box),
	alignment_h_(
		_alignment_h),
	alignment_v_(
		_alignment_v),
	flags_(
		_flags)
{
}

sge::font::metrics_ptr const
fruitlib::font::object_parameters::metrics() const
{
	return metrics_;
}

fruitlib::font::drawer::object &
fruitlib::font::object_parameters::drawer() const
{
	return drawer_;
}

sge::font::text::string const &
fruitlib::font::object_parameters::text() const
{
	return text_;
}

sge::font::rect const &
fruitlib::font::object_parameters::bounding_box() const
{
	return bounding_box_;
}

sge::font::text::align_h::type 
fruitlib::font::object_parameters::alignment_h() const
{
	return alignment_h_;
}

sge::font::text::align_v::type 
fruitlib::font::object_parameters::alignment_v() const
{
	return alignment_v_;
}

sge::font::text::flags_field const &
fruitlib::font::object_parameters::flags() const
{
	return flags_;
}
