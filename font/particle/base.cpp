#include "base.hpp"
#include "base_parameters.hpp"
#include "../system.hpp"
#include "../drawer.hpp"
#include <sge/font/text/draw.hpp>
// Because text::draw returns part. I don't use it, but I have to include this crap *sigh*
#include <sge/font/text/part.hpp>
// Because font::flags is a bitfield *sigh*
#include <fcppt/container/bitfield/basic_impl.hpp>
#include <iostream>

fruitcut::font::particle::base::base(
	base_parameters const &params)
:
	system_(
		params.system()),
	metrics_(
		params.metrics()),
	text_(
		params.text()),
	bounding_box_(
		params.bounding_box()),
	alignment_h_(
		params.alignment_h()),
	alignment_v_(
		params.alignment_v()),
	flags_(
		params.flags())
{
	system_.insert(
		*this);
}

void
fruitcut::font::particle::base::render(
	sge::font::metrics_ptr const _metrics,
	drawer &_drawer)
{
	sge::font::text::draw(
		_metrics,
		_drawer,
		text_,
		bounding_box_,
		alignment_h_,
		alignment_v_,
		flags_);
}

fruitcut::font::identifier const &
fruitcut::font::particle::base::metrics() const
{
	return metrics_;
}

void
fruitcut::font::particle::base::text(
	sge::font::text::string const &_text)
{
	text_ = 
		_text;
}

void
fruitcut::font::particle::base::bounding_box(
	sge::font::rect const &_bounding_box)
{
	bounding_box_ = 
		_bounding_box;
}

fruitcut::font::particle::base::~base()
{
}
