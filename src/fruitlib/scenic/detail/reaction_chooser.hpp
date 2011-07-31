#ifndef FRUITLIB_SCENIC_DETAIL_REACTION_CHOOSER_HPP_INCLUDED
#define FRUITLIB_SCENIC_DETAIL_REACTION_CHOOSER_HPP_INCLUDED

#include <fruitlib/scenic/events/base_fwd.hpp>
#include <fcppt/nonassignable.hpp>
#include <fcppt/mpl/for_each.hpp>
#include <typeinfo>

namespace fruitlib
{
namespace scenic
{
namespace detail
{
template<typename Derived>
struct reaction_chooser
{
FCPPT_NONASSIGNABLE(
	reaction_chooser);
public:
	explicit
	reaction_chooser(
		Derived &_derived,
		events::base const &_event,
		bool &_unconsumed)
	:
		derived_(
			_derived),
		event_(
			_event),
		unconsumed_(
			_unconsumed)
	{
	}

	template<typename DerivedEvent>
	void
	operator()() const
	{
		if(typeid(event_) != typeid(DerivedEvent))
			return;

		derived_.react(
			static_cast<DerivedEvent const &>(
				event_));
		unconsumed_ = 
			false;
	}
private:
	Derived &derived_;
	events::base const &event_;
	bool &unconsumed_;
};
}
}
}

#endif
