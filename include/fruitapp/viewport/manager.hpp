#ifndef FRUITAPP_VIEWPORT_MANAGER_HPP_INCLUDED
#define FRUITAPP_VIEWPORT_MANAGER_HPP_INCLUDED

#include <fruitapp/viewport/change_callback.hpp>
#include <fruitapp/viewport/change_callback_fn.hpp>
#include <fruitapp/viewport/optional.hpp>
#include <fruitapp/viewport/trigger_early.hpp>
#include <sge/viewport/manager_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/optional_impl.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/object.hpp>
#include <fcppt/signal/scoped_connection.hpp>


namespace fruitapp
{
namespace viewport
{
class manager
{
FCPPT_NONCOPYABLE(
	manager);
public:
	explicit
	manager(
		sge::viewport::manager &);

	fruitapp::viewport::optional const &
	current_viewport() const;

	fcppt::signal::auto_connection
	change_callback(
		fruitapp::viewport::change_callback const &,
		fruitapp::viewport::trigger_early const &);

	~manager();
private:
	fcppt::signal::object<fruitapp::viewport::change_callback_fn> change_signal_;
	fcppt::signal::scoped_connection viewport_connection_;
	fruitapp::viewport::optional current_viewport_;

	void
	internal_change_callback(
		sge::renderer::target::viewport const &);
};
}
}

#endif

