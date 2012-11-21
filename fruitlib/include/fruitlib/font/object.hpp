#ifndef FRUITLIB_FONT_OBJECT_HPP_INCLUDED
#define FRUITLIB_FONT_OBJECT_HPP_INCLUDED

#include <fruitlib/font/align_v.hpp>
#include <fruitlib/font/identifier.hpp>
#include <fruitlib/font/manager_fwd.hpp>
#include <fruitlib/font/object_parameters_fwd.hpp>
#include <fruitlib/font/scale.hpp>
#include <sge/font/align_h.hpp>
#include <sge/font/flags_field.hpp>
#include <sge/font/object_fwd.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/string.hpp>
#include <sge/font/draw/static_text_fwd.hpp>
#include <sge/image/color/any/object.hpp>
#include <sge/renderer/context/ffp_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/scoped_ptr.hpp>
#include <fcppt/container/bitfield/object_impl.hpp>
#include <fcppt/math/box/object_impl.hpp>


namespace fruitlib
{
namespace font
{
class object
{
FCPPT_NONCOPYABLE(
	object);
public:
	explicit
	object(
		fruitlib::font::object_parameters const &);

	void
	render(
		sge::renderer::context::ffp &);

	sge::font::object &
	font_object() const;

	sge::font::string const &
	text() const;

	void
	text(
		sge::font::string const &);

	sge::font::rect const &
	bounding_box() const;

	void
	bounding_box(
		sge::font::rect const &);

	sge::font::align_h::type
	alignment_h() const;

	void
	alignment_h(
		sge::font::align_h::type);

	fruitlib::font::align_v::type
	alignment_v() const;

	void
	alignment_v(
		fruitlib::font::align_v::type);

	sge::font::flags_field const &
	flags() const;

	void
	flags(
		sge::font::flags_field const &);

	sge::image::color::any::object const &
	color() const;

	void
	color(
		sge::image::color::any::object const &);

	fruitlib::font::scale const &
	scale() const;

	void
	scale(
		fruitlib::font::scale const &);

	~object();
private:
	fruitlib::font::manager &manager_;
	sge::font::object &font_object_;
	sge::font::string text_;
	sge::font::rect bounding_box_;
	sge::font::align_h::type alignment_h_;
	fruitlib::font::align_v::type alignment_v_;
	sge::font::flags_field flags_;
	sge::image::color::any::object color_;
	fruitlib::font::scale scale_;
	fcppt::scoped_ptr<sge::font::draw::static_text> text_object_;

	bool
	rebuild_text_object();

	void
	regenerate_text_object();
};
}
}

#endif
