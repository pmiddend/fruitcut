#ifndef FRUITCUT_SCENIC_NODES_INTRUSIVE_WITH_CALLBACKS_HPP_INCLUDED
#define FRUITCUT_SCENIC_NODES_INTRUSIVE_WITH_CALLBACKS_HPP_INCLUDED

#include "intrusive.hpp"
#include <fcppt/noncopyable.hpp>
#include <fcppt/function/object.hpp>

namespace fruitcut
{
namespace scenic
{
namespace nodes
{
class intrusive_with_callbacks
:
	public intrusive
{
FCPPT_NONCOPYABLE(
	intrusive_with_callbacks);
public:
	typedef
	fcppt::function::object<void ()>
	update_callback;

	typedef
	fcppt::function::object<void ()>
	render_callback;

	explicit
	intrusive_with_callbacks(
		update_callback const &,
		render_callback const &);

	~intrusive_with_callbacks();
private:
	update_callback update_;
	render_callback render_;

	void
	update();

	void
	render();
};
}
}
}

#endif
