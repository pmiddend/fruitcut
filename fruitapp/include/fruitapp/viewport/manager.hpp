#ifndef FRUITAPP_VIEWPORT_MANAGER_HPP_INCLUDED
#define FRUITAPP_VIEWPORT_MANAGER_HPP_INCLUDED

#include <fruitapp/viewport/change_callback.hpp>
#include <fruitapp/viewport/change_callback_fn.hpp>
#include <fruitapp/viewport/optional.hpp>
#include <fruitapp/viewport/trigger_early.hpp>
#include <sge/renderer/target/base_fwd.hpp>
#include <sge/viewport/manager_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/optional/object_impl.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>


namespace fruitapp
{
namespace viewport
{
class manager
{
FCPPT_NONCOPYABLE(
	manager);
public:
	manager(
		sge::viewport::manager &,
		sge::renderer::target::base const &);

	fruitapp::viewport::optional const &
	current_viewport() const;

	fcppt::signal::auto_connection
	change_callback(
		fruitapp::viewport::change_callback const &,
		fruitapp::viewport::trigger_early const &);

	sge::renderer::target::base const &
	target() const;

	~manager();
private:
	sge::renderer::target::base const &target_;
	fcppt::signal::object<fruitapp::viewport::change_callback_fn> change_signal_;
	fcppt::signal::auto_connection viewport_connection_;
	fruitapp::viewport::optional current_viewport_;

	void
	internal_change_callback(
		sge::renderer::target::viewport const &);
};
}
}

#endif

