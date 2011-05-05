#ifndef FRUITCUT_FONT_OBJECT_PARAMETERS_HPP_INCLUDED
#define FRUITCUT_FONT_OBJECT_PARAMETERS_HPP_INCLUDED

#include "object_parameters_fwd.hpp"
#include "drawer/object_fwd.hpp"
#include "identifier.hpp"
#include <sge/font/metrics_ptr.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/text/string.hpp>
#include <sge/font/text/align_h.hpp>
#include <sge/font/text/align_v.hpp>
#include <sge/font/text/flags_field.hpp>
#include <fcppt/container/bitfield/bitfield.hpp>
#include <fcppt/math/box/box.hpp>

namespace fruitcut
{
namespace font
{
class object_parameters
{
public:
	explicit
	object_parameters(
		sge::font::metrics_ptr,
		fruitcut::font::drawer::object &,
		sge::font::text::string const &,
		sge::font::rect const &,
		sge::font::text::align_h::type,
		sge::font::text::align_v::type,
		sge::font::text::flags_field const &);

	sge::font::metrics_ptr const
	metrics() const;

	fruitcut::font::drawer::object &
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
	sge::font::metrics_ptr metrics_;
	fruitcut::font::drawer::object &drawer_;
	sge::font::text::string text_;
	sge::font::rect bounding_box_;
	sge::font::text::align_h::type alignment_h_;
	sge::font::text::align_v::type alignment_v_;
	sge::font::text::flags_field flags_;
};
}
}

#endif
