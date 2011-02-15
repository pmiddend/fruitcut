#include "base_parameters.hpp"

fruitcut::font::particle::base_parameters::base_parameters(
	sge::font::metrics_ptr const _metrics,
	font::drawer &_drawer,
	sge::font::text::string const &_text,
	sge::font::rect const &_bounding_box,
	sge::font::text::align_h::type const _alignment_h,
	sge::font::text::align_v::type const _alignment_v,
	sge::font::text::flags::type const _flags)
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
fruitcut::font::particle::base_parameters::metrics() const
{
	return metrics_;
}

fruitcut::font::drawer &
fruitcut::font::particle::base_parameters::drawer() const
{
	return drawer_;
}

sge::font::text::string const &
fruitcut::font::particle::base_parameters::text() const
{
	return text_;
}

sge::font::rect const &
fruitcut::font::particle::base_parameters::bounding_box() const
{
	return bounding_box_;
}

sge::font::text::align_h::type 
fruitcut::font::particle::base_parameters::alignment_h() const
{
	return alignment_h_;
}

sge::font::text::align_v::type 
fruitcut::font::particle::base_parameters::alignment_v() const
{
	return alignment_v_;
}

sge::font::text::flags::type 
fruitcut::font::particle::base_parameters::flags() const
{
	return flags_;
}
