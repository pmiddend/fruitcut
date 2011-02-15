#ifndef FRUITCUT_FONT_BASE_HPP_INCLUDED
#define FRUITCUT_FONT_BASE_HPP_INCLUDED

#include "base_parameters_fwd.hpp"
#include "../drawer_fwd.hpp"
#include <sge/font/metrics_ptr.hpp>
#include <sge/font/text/string.hpp>
#include <sge/font/metrics_ptr.hpp>
#include <sge/font/text/string.hpp>
#include <sge/font/rect.hpp>
#include <sge/font/text/align_h.hpp>
#include <sge/font/text/align_v.hpp>
#include <sge/font/text/flags.hpp>
#include <fcppt/math/box/basic_impl.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/intrusive/list_hook.hpp>
#include <boost/intrusive/link_mode.hpp>

namespace fruitcut
{
namespace font
{
namespace particle
{
/**
	This is currently a "heavyweight" base class. There's no particular
	reason for it, it just seems to fit until now.

	Also, it derives from intrusive so you can have it both ways: Either
	you insert the font particle to the particle system via a unique_ptr
	and forget the particle afterwards, or you add it with to an
	intrusive list and get to keep ownership.

	As a side note to later: Fonts are somewhat similar to sprites: They
	both can be intrusive or not, they have attributes you can set, they
	have a "sprite_system" (which is the font drawer) which manages
	stuff for them. Currently, the "base" class manages one of two
	things: The font sprite rendering and modification
	setters/getters. The other thing is behaviour: _How_ it moves, for
	example. This could be designed differently. We could define a
	"sprite" class for fonts, which you can use just like
	sge::sprite. Then base would lose that responsibility and just
	manage the fonts behaviour.
 */
class base
:
	public 
		boost::intrusive::list_base_hook
		<
			boost::intrusive::link_mode<boost::intrusive::auto_unlink>
		>
{
FCPPT_NONCOPYABLE(
	base);
public:
	explicit
	base(
		base_parameters const &);

	virtual void
	update() = 0;

	virtual void
	render();

	virtual bool
	dead() = 0;

	virtual 
	~base();
protected:
	sge::font::metrics_ptr metrics_;
	font::drawer &drawer_;
	sge::font::text::string text_;
	sge::font::rect bounding_box_;
	sge::font::text::align_h::type alignment_h_;
	sge::font::text::align_v::type alignment_v_;
	sge::font::text::flags::type flags_;
};
}
}
}

#endif
