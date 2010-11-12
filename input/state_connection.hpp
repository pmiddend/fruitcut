#ifndef FRUITCUT_INPUT_STATE_CONNECTION_HPP_INCLUDED
#define FRUITCUT_INPUT_STATE_CONNECTION_HPP_INCLUDED

#include "state_fwd.hpp"
#include "optional_key_callback.hpp"
#include "optional_mouse_axis_callback.hpp"
#include "optional_mouse_button_callback.hpp"
#include <sge/systems/instance_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <memory>

namespace fruitcut
{
namespace input
{
class state_connection_impl
{
public:
	explicit
	state_connection_impl(
		state &,
		optional_key_callback const &,
		optional_mouse_axis_callback const &,
		optional_mouse_button_callback const &);
private:
	fcppt::signal::scoped_connection c1,c2,c3;
};

typedef
std::shared_ptr<state_connection_impl>
state_connection;
}
}

#endif
