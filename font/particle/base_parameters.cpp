#include "base_parameters.hpp"

fruitcut::font::particle::base_parameters::base_parameters(
	identifier const &_metrics,
	sge::font::text::string const &_text,
	sge::font::rect const &_bounding_box,
	sge::font::text::align_h::type const _alignment_h,
	sge::font::text::align_v::type const _alignment_v,
	sge::font::text::flags_field const &_flags)
:
	metrics_(
		_metrics),
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

fruitcut::font::identifier const &
fruitcut::font::particle::base_parameters::metrics() const
{
	return metrics_;
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

sge::font::text::flags_field const &
fruitcut::font::particle::base_parameters::flags() const
{
	return flags_;
}
