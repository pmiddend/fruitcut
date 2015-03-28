#ifndef FRUITLIB_SCENIC_BASE_HPP_INCLUDED
#define FRUITLIB_SCENIC_BASE_HPP_INCLUDED

#include <fruitlib/scenic/depth.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/base_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fruitlib/detail/symbol.hpp>
#include <fcppt/preprocessor/disable_gcc_warning.hpp>
#include <fcppt/preprocessor/pop_warning.hpp>
#include <fcppt/preprocessor/push_warning.hpp>
#include <fcppt/symbol/class.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/intrusive/list.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace scenic
{
FCPPT_PP_PUSH_WARNING
FCPPT_PP_DISABLE_GCC_WARNING(-Weffc++)
FCPPT_PP_DISABLE_GCC_WARNING(-Wnon-virtual-dtor)

class FCPPT_SYMBOL_CLASS base
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
	typedef
	boost::intrusive::list
	<
		base,
		boost::intrusive::constant_time_size<false>
	>
	child_sequence;

	virtual void
	process(
		events::base const &) = 0;

	child_sequence const &
	children() const;

	child_sequence &
	children();

	void
	forward_to_children(
		events::base const &);

	scenic::depth::value_type
	depth() const;

	FRUITLIB_DETAIL_SYMBOL
	virtual ~base();
protected:
	explicit
	base(
		scenic::optional_parent const &);
private:
	child_sequence children_;
	scenic::depth::value_type const depth_;

	void
	add_child(
		scenic::base &);
};

FCPPT_PP_POP_WARNING

}
}

#endif
