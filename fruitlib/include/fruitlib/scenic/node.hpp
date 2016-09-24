#ifndef FRUITLIB_SCENIC_NODE_HPP_INCLUDED
#define FRUITLIB_SCENIC_NODE_HPP_INCLUDED

#include <fruitlib/scenic/base.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/detail/has_scene_reactions.hpp>
#include <fruitlib/scenic/detail/reaction_chooser.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/text.hpp>
#include <fcppt/type_name.hpp>
#include <fcppt/mpl/for_each.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/utility/enable_if.hpp>
#include <typeinfo>
#include <fcppt/config/external_end.hpp>


namespace fruitlib
{
namespace scenic
{
template<typename Derived>
class node
:
	public scenic::base
{
FCPPT_NONCOPYABLE(
	node);
public:
	typedef
	node<Derived>
	node_base;

	explicit
	node(
		scenic::optional_parent const &_parent)
	:
		scenic::base(
			_parent)
	{
	}

	void
	process(
		events::base const &e
	)
	override
	{
		this->process_impl<Derived>(
			e);
	}
protected:
	template<typename Derived2>
	typename
	boost::enable_if
	<
		detail::has_scene_reactions<Derived2>,
		void
	>::type
	process_impl(
		events::base const &e)
	{
		bool event_unconsumed =
			true;
		fcppt::mpl::for_each<typename Derived::scene_reactions>(
			detail::reaction_chooser<Derived>(
				static_cast<Derived &>(
					*this),
				e,
				event_unconsumed));
		if(event_unconsumed)
			static_cast<Derived &>(*this).unconsumed(
				e);
	}

	template<typename Derived2>
	typename
	boost::disable_if
	<
		detail::has_scene_reactions<Derived2>,
		void
	>::type
	process_impl(
		events::base const &e)
	{
		static_cast<Derived &>(*this).unconsumed(
			e);
	}

	void
	unconsumed(
		events::base const &e)
	{
		this->forward_to_children(
			e);
	}
};
}
}

#endif
