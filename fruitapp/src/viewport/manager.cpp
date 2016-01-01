#include <fruitapp/viewport/manager.hpp>
#include <sge/viewport/manage_callback.hpp>
#include <sge/viewport/manager.hpp>
#include <fcppt/optional/maybe_void.hpp>
#include <fcppt/config/external_begin.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::viewport::manager::manager(
	sge::viewport::manager &_viewport_manager,
	sge::renderer::target::base const &_target)
:
	target_(
		_target),
	change_signal_(),
	viewport_connection_(
		_viewport_manager.manage_callback(
			sge::viewport::manage_callback{
				std::bind(
					&manager::internal_change_callback,
					this,
					std::placeholders::_1
				)
			}
		)
	),
	current_viewport_()
{
}

fruitapp::viewport::optional const &
fruitapp::viewport::manager::current_viewport() const
{
	return
		current_viewport_;
}

fcppt::signal::auto_connection
fruitapp::viewport::manager::change_callback(
	viewport::change_callback const &_change_callback,
	viewport::trigger_early const &_trigger_early)
{
	fcppt::signal::auto_connection result(
		change_signal_.connect(
			_change_callback));

	fcppt::optional::maybe_void(
		current_viewport_,
		[
			&_trigger_early,
			&_change_callback
		](
			sge::renderer::target::viewport const &_viewport
		)
		{
			if(_trigger_early.get())
				_change_callback(
					_viewport);
		}
	);

	return
		result;
}

sge::renderer::target::base const &
fruitapp::viewport::manager::target() const
{
	return
		target_;
}

fruitapp::viewport::manager::~manager()
{
}

void
fruitapp::viewport::manager::internal_change_callback(
	sge::renderer::target::viewport const &_viewport)
{
	current_viewport_ =
		fruitapp::viewport::optional(
			_viewport);

	change_signal_(
		_viewport);
}
