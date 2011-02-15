#include "base.hpp"
#include "base_parameters.hpp"
#include "../drawer.hpp"
#include <sge/font/text/draw.hpp>
// Because text::draw returns part. I don't use it, but I have to include this crap *sigh*
#include <sge/font/text/part.hpp>
// Because font::flags is a bitfield *sigh*
#include <fcppt/container/bitfield/basic_impl.hpp>

fruitcut::font::particle::base::base(
	base_parameters const &params)
:
	metrics_(
		params.metrics()),
	drawer_(
		params.drawer()),
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
}

void
fruitcut::font::particle::base::render()
{
	sge::font::text::draw(
		metrics_,
		drawer_,
		text_,
		bounding_box_,
		alignment_h_,
		alignment_v_,
		flags_);
}

fruitcut::font::particle::base::~base()
{
}
