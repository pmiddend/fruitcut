#ifndef FRUITLIB_FONT_OBJECT_PARAMETERS_HPP_INCLUDED
#define FRUITLIB_FONT_OBJECT_PARAMETERS_HPP_INCLUDED

#include <fruitlib/font/align_v.hpp>
#include <fruitlib/font/identifier.hpp>
#include <fruitlib/font/object_parameters_fwd.hpp>
#include <fruitlib/font/scale.hpp>
#include <sge/font/align_h.hpp>
#include <sge/font/flags_field.hpp>
#include <sge/font/object_fwd.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/string.hpp>
#include <sge/image/color/any/object.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <fcppt/nonassignable.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>
#include <fcppt/math/box/object_impl.hpp>


namespace fruitlib
{
namespace font
{
class object_parameters
{
FCPPT_NONASSIGNABLE(
	object_parameters);
public:
	object_parameters(
		sge::renderer::device::ffp &,
		sge::font::object &,
		sge::font::string const &,
		sge::font::rect const &,
		sge::font::align_h::type,
		fruitlib::font::align_v::type,
		sge::font::flags_field const &,
		sge::image::color::any::object const &,
		fruitlib::font::scale const &);

	sge::renderer::device::ffp &
	renderer() const;

	sge::font::object &
	font_object() const;

	sge::font::string const &
	text() const;

	sge::font::rect const &
	bounding_box() const;

	sge::font::align_h::type
	alignment_h() const;

	fruitlib::font::align_v::type
	alignment_v() const;

	sge::font::flags_field const &
	flags() const;

	sge::image::color::any::object const &
	color() const;

	fruitlib::font::scale const &
	scale() const;
private:
	sge::renderer::device::ffp &renderer_;
	sge::font::object &font_object_;
	sge::font::string text_;
	sge::font::rect bounding_box_;
	sge::font::align_h::type alignment_h_;
	fruitlib::font::align_v::type alignment_v_;
	sge::font::flags_field flags_;
	sge::image::color::any::object color_;
	fruitlib::font::scale scale_;
};
}
}

#endif
