#ifndef FRUITLIB_FONT_OBJECT_PARAMETERS_HPP_INCLUDED
#define FRUITLIB_FONT_OBJECT_PARAMETERS_HPP_INCLUDED

#include <fruitlib/detail/symbol.hpp>
#include <fruitlib/font/align_h.hpp>
#include <fruitlib/font/align_v.hpp>
#include <fruitlib/font/identifier.hpp>
#include <fruitlib/font/manager_fwd.hpp>
#include <fruitlib/font/object_parameters_fwd.hpp>
#include <fruitlib/font/scale.hpp>
#include <sge/font/flags_field.hpp>
#include <sge/font/object_fwd.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/string.hpp>
#include <sge/image/color/any/object.hpp>
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
	FRUITLIB_DETAIL_SYMBOL
	object_parameters(
		fruitlib::font::manager &,
		fruitlib::font::identifier const &,
		sge::font::string const &,
		sge::font::rect const &,
		fruitlib::font::align_h,
		fruitlib::font::align_v,
		sge::font::flags_field const &,
		sge::image::color::any::object const &,
		fruitlib::font::scale const &);

	fruitlib::font::manager &
	manager() const;

	sge::font::object &
	font_object() const;

	sge::font::string const &
	text() const;

	sge::font::rect const &
	bounding_box() const;

	fruitlib::font::align_h
	alignment_h() const;

	fruitlib::font::align_v
	alignment_v() const;

	sge::font::flags_field const &
	flags() const;

	sge::image::color::any::object const &
	color() const;

	fruitlib::font::scale const &
	scale() const;
private:
	fruitlib::font::manager &manager_;
	sge::font::object &font_object_;
	sge::font::string text_;
	sge::font::rect bounding_box_;
	fruitlib::font::align_h alignment_h_;
	fruitlib::font::align_v alignment_v_;
	sge::font::flags_field flags_;
	sge::image::color::any::object color_;
	fruitlib::font::scale scale_;
};
}
}

#endif
