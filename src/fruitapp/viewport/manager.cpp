#include <fruitapp/viewport/manager.hpp>
#include <sge/renderer/active_target.hpp>
#include <sge/renderer/target_base.hpp>
#include <sge/viewport/manager.hpp>
#include <fcppt/move.hpp>
#include <fcppt/tr1/functional.hpp>


fruitapp::viewport::manager::manager(
	sge::viewport::manager &_viewport_manager,
	sge::renderer::device &_renderer)
:
	renderer_(
		_renderer),
	change_signal_(),
	viewport_connection_(
		_viewport_manager.manage_callback(
			std::tr1::bind(
				&manager::internal_change_callback,
				this))),
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

	if(_trigger_early.get() && current_viewport_)
		_change_callback(
			*current_viewport_);

	return
		fcppt::move(
			result);
}

fruitapp::viewport::manager::~manager()
{
}

void
fruitapp::viewport::manager::internal_change_callback()
{
	current_viewport_ =
		viewport::optional(
			sge::renderer::active_target(
				renderer_).viewport());

	change_signal_(
		*current_viewport_);
}
