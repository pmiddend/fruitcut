#include "object.hpp"
#include "object_parameters.hpp"
#include "drawer/object.hpp"
#include <sge/font/text/draw.hpp>
#include <sge/font/text/part.hpp>

fruitcut::font::object::object(
	object_parameters const &p)
:
	metrics_(
		p.metrics()),
	drawer_(
		p.drawer()),
	text_(
		p.text()),
	bounding_box_(
		p.bounding_box()),
	alignment_h_(
		p.alignment_h()),
	alignment_v_(
		p.alignment_v()),
	flags_(
		p.flags())
{
}

void
fruitcut::font::object::render()
{
	sge::font::text::draw(
		*metrics_,
		drawer_,
		text_,
		bounding_box_,
		alignment_h_,
		alignment_v_,
		flags_);
}

sge::font::metrics &
fruitcut::font::object::metrics() const
{
	return *metrics_;
}

fruitcut::font::drawer::object &
fruitcut::font::object::drawer() const
{
	return drawer_;
}

sge::font::text::string const &
fruitcut::font::object::text() const
{
	return text_;
}

void
fruitcut::font::object::text(
	sge::font::text::string const &_text)
{
	text_ = 
		_text;
}

sge::font::rect const &
fruitcut::font::object::bounding_box() const
{
	return bounding_box_;
}

void
fruitcut::font::object::bounding_box(
	sge::font::rect const &_bounding_box)
{
	bounding_box_ = 
		_bounding_box;
}

sge::font::text::align_h::type 
fruitcut::font::object::alignment_h() const
{
	return alignment_h_;
}

void
fruitcut::font::object::alignment_h(
	sge::font::text::align_h::type _alignment_h)
{
	alignment_h_ = 
		_alignment_h;
}

sge::font::text::align_v::type 
fruitcut::font::object::alignment_v() const
{
	return alignment_v_;
}

void
fruitcut::font::object::alignment_v(
	sge::font::text::align_v::type const _alignment_v)
{
	alignment_v_ = 
		_alignment_v;
}

sge::font::text::flags_field const &
fruitcut::font::object::flags() const
{
	return flags_;
}

void
fruitcut::font::object::flags(
	sge::font::text::flags_field const &_flags)
{
	flags_ = 
		_flags;
}

fruitcut::font::object::~object()
{
}
