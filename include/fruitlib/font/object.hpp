#ifndef FRUITLIB_FONT_OBJECT_HPP_INCLUDED
#define FRUITLIB_FONT_OBJECT_HPP_INCLUDED

#include <fruitlib/font/identifier.hpp>
#include <fruitlib/font/object_parameters_fwd.hpp>
#include <fruitlib/font/drawer/object_fwd.hpp>
#include <sge/font/metrics_ptr.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/text/align_h.hpp>
#include <sge/font/text/align_v.hpp>
#include <sge/font/text/flags_field.hpp>
#include <sge/font/text/string.hpp>
#include <fcppt/noncopyable.hpp>
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
		font::object_parameters const &);

	void
	render();

	sge::font::metrics &
	metrics() const;

	font::drawer::object &
	drawer() const;

	sge::font::text::string const &
	text() const;

	void
	text(
		sge::font::text::string const &);

	sge::font::rect const &
	bounding_box() const;

	void
	bounding_box(
		sge::font::rect const &);

	sge::font::text::align_h::type
	alignment_h() const;

	void
	alignment_h(
		sge::font::text::align_h::type);

	sge::font::text::align_v::type
	alignment_v() const;

	void
	alignment_v(
		sge::font::text::align_v::type);

	sge::font::text::flags_field const &
	flags() const;

	void
	flags(
		sge::font::text::flags_field const &);

	~object();
private:
	sge::font::metrics_ptr const metrics_;
	font::drawer::object &drawer_;
	sge::font::text::string text_;
	sge::font::rect bounding_box_;
	sge::font::text::align_h::type alignment_h_;
	sge::font::text::align_v::type alignment_v_;
	sge::font::text::flags_field flags_;
};
}
}

#endif
