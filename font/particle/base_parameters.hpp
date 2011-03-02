#ifndef FRUITCUT_FONT_PARTICLE_BASE_PARAMETERS_HPP_INCLUDED
#define FRUITCUT_FONT_PARTICLE_BASE_PARAMETERS_HPP_INCLUDED

#include "../identifier.hpp"
#include "../system_fwd.hpp"
#include <sge/font/text/string.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/text/align_h.hpp>
#include <sge/font/text/align_v.hpp>
// This is just convenience so I don't have to include this fucking
// bitfield impl and the flags header
#include <sge/font/text/flags_field.hpp>
#include <sge/font/text/flags.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/container/bitfield/bitfield.hpp>

namespace fruitcut
{
namespace font
{
namespace particle
{
class base_parameters
{
public:
	explicit
	base_parameters(
		font::system &,
		identifier const &,
		sge::font::text::string const &,
		sge::font::rect const &,
		sge::font::text::align_h::type,
		sge::font::text::align_v::type,
		sge::font::text::flags_field const &);

	font::system &
	system() const;

	identifier const &
	metrics() const;

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
	font::system &system_;
	identifier metrics_;
	sge::font::text::string text_;
	sge::font::rect bounding_box_;
	sge::font::text::align_h::type alignment_h_;
	sge::font::text::align_v::type alignment_v_;
	sge::font::text::flags_field flags_;
};
}
}
}

#endif
