#ifndef FRUITAPP_VIEWPORT_MANAGER_HPP_INCLUDED
#define FRUITAPP_VIEWPORT_MANAGER_HPP_INCLUDED

#include <fruitapp/viewport/change_callback.hpp>
#include <fruitapp/viewport/change_callback_fn.hpp>
#include <fruitapp/viewport/optional.hpp>
#include <fruitapp/viewport/trigger_early.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/viewport/manager_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/optional_impl.hpp>
#include <fcppt/math/box/object_impl.hpp>
#include <fcppt/preprocessor/const.hpp>
#include <fcppt/preprocessor/warn_unused_result.hpp>
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
		sge::viewport::manager &,
		sge::renderer::device &);

	fruitapp::viewport::optional const &
	current_viewport() const
	FCPPT_PP_CONST;

	fcppt::signal::auto_connection
	change_callback(
		viewport::change_callback const &,
		viewport::trigger_early const &)
	FCPPT_PP_WARN_UNUSED_RESULT;

	~manager();
private:
	sge::renderer::device &renderer_;
	fcppt::signal::object<viewport::change_callback_fn> change_signal_;
	fcppt::signal::scoped_connection viewport_connection_;
	viewport::optional current_viewport_;

	void
	internal_change_callback();
};
}
}

#endif

