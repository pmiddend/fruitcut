#include <fruitapp/states/gameover/superstate.hpp>
#include <fruitapp/config_variables.hpp>
#include <fcppt/assert/pre.hpp>
#include <fcppt/text.hpp>


fruitapp::states::gameover::superstate::superstate(
	my_context const ctx)
:
	my_base(
		ctx),
	name_(
		FCPPT_TEXT("You shouldn't see this"))
{
}

void
fruitapp::states::gameover::superstate::name(
	fcppt::string const &_name)
{
	FCPPT_ASSERT_PRE(
		!_name.empty());
	context<fruitapp::machine>().config_variables().last_user_name().value(
		_name);
	name_ =
		_name;
}

fcppt::string const &
fruitapp::states::gameover::superstate::name() const
{
	return
		name_;
}

fruitapp::states::gameover::superstate::~superstate()
{
}
