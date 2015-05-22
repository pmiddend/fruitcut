#include <fruitlib/font/align_h.hpp>
#include <fruitlib/font/manager.hpp>
#include <fruitlib/font/object.hpp>
#include <fruitlib/font/object_parameters.hpp>
#include <sge/font/text_parameters.hpp>
#include <sge/font/align_h/center.hpp>
#include <sge/font/align_h/left.hpp>
#include <sge/font/align_h/max_width.hpp>
#include <sge/font/align_h/right.hpp>
#include <sge/font/align_h/variant.hpp>
#include <sge/font/draw/static_text.hpp>
#include <sge/image/color/any/object.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/assert/unreachable.hpp>
#include <fcppt/math/dim/comparison.hpp>


fruitlib::font::object::object(
	fruitlib::font::object_parameters const &p)
:
	manager_(
		p.manager()),
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
	this->regenerate_text_object();
}

void
fruitlib::font::object::render(
	sge::renderer::context::ffp &_context)
{
	if(text_object_)
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

	this->regenerate_text_object();
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

	if(_bounding_box.size() == bounding_box_.size())
	{
		if(!this->rebuild_text_object())
			if(text_object_)
				text_object_->pos(
					bounding_box_.pos());
	}
	else
		this->regenerate_text_object();
}

fruitlib::font::align_h
fruitlib::font::object::alignment_h() const
{
	return
		alignment_h_;
}

void
fruitlib::font::object::alignment_h(
	fruitlib::font::align_h const _alignment_h)
{
	alignment_h_ =
		_alignment_h;

	this->regenerate_text_object();
}

fruitlib::font::align_v
fruitlib::font::object::alignment_v() const
{
	return
		alignment_v_;
}

void
fruitlib::font::object::alignment_v(
	fruitlib::font::align_v const _alignment_v)
{
	alignment_v_ =
		_alignment_v;

	this->regenerate_text_object();
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

	this->regenerate_text_object();
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

	this->rebuild_text_object();

	if(text_object_)
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

bool
fruitlib::font::object::rebuild_text_object()
{
	if(!bounding_box_.content() || text_.empty())
	{
		if(text_object_)
		{
			text_object_.reset();
			return true;
		}
		text_object_.reset();
		return false;
	}

	if(text_object_)
		return false;

	this->regenerate_text_object();
	return true;
}

void
fruitlib::font::object::regenerate_text_object()
{
	if(!bounding_box_.content() || text_.empty())
		return;

	sge::font::align_h::max_width const max_width(
		bounding_box_.w()
	);

	auto const convert_align_h(
		[
			max_width
		](
			fruitlib::font::align_h const _align
		)
		-> sge::font::align_h::variant
		{
			switch(
				_align
			)
			{
			case fruitlib::font::align_h::left:
				return
					sge::font::align_h::left(
						max_width
					);
			case fruitlib::font::align_h::center:
				return
					sge::font::align_h::center(
						max_width
					);
			case fruitlib::font::align_h::right:
				return
					sge::font::align_h::right(
						max_width
					);
			}

			FCPPT_ASSERT_UNREACHABLE;
		}
	);

	text_object_ =
		fcppt::make_unique_ptr<sge::font::draw::static_text>(
			manager_.renderer(),
			font_object_,
			text_,
			sge::font::text_parameters(
				convert_align_h(
					alignment_h_
				)
			)
			.flags(
				flags_
			),
			bounding_box_.pos(),
			color_,
			manager_.emulate_srgb());

	switch(alignment_v_)
	{
	case fruitlib::font::align_v::top:
		break;
	case fruitlib::font::align_v::middle:
		text_object_->pos(
			sge::font::vector(
				text_object_->pos().x(),
				bounding_box_.pos().y() +
				bounding_box_.size().h()/2 -
				text_object_->rect().size().h()/2));
		break;
	case fruitlib::font::align_v::bottom:
		text_object_->pos(
			sge::font::vector(
				text_object_->pos().x(),
				bounding_box_.size().h() -
				text_object_->rect().size().h()));
		break;
	}
}
