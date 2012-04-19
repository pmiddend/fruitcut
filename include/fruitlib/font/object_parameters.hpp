#ifndef FRUITLIB_FONT_OBJECT_PARAMETERS_HPP_INCLUDED
#define FRUITLIB_FONT_OBJECT_PARAMETERS_HPP_INCLUDED

#include <fruitlib/font/identifier.hpp>
#include <fruitlib/font/object_parameters_fwd.hpp>
#include <fruitlib/font/drawer/object_fwd.hpp>
#include <sge/font/metrics_fwd.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/text/align_h.hpp>
#include <sge/font/text/align_v.hpp>
#include <sge/font/text/flags_field.hpp>
#include <sge/font/text/string.hpp>
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
	explicit
	object_parameters(
		sge::font::metrics &,
		font::drawer::object &,
		sge::font::text::string const &,
		sge::font::rect const &,
		sge::font::text::align_h::type,
		sge::font::text::align_v::type,
		sge::font::text::flags_field const &);

	sge::font::metrics &
	metrics() const;

	font::drawer::object &
	drawer() const;

	sge::font::text::string const &
	text() const;

	sge::font::rect const &
	bounding_box() const;

	sge::font::text::align_h::type
	alignment_h() const;

	sge::font::text::align_v::type
	alignment_v() const;

	sge::font::text::flags_field const &
	flags() const;
private:
	sge::font::metrics &metrics_;
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
