#include "cut.hpp"
#include "../media_path.hpp"
#include "../sprite/parameters.hpp"
#include "../sprite/object.hpp"
#include <sge/sprite/external_system_impl.hpp>
#include <sge/sprite/render_one.hpp>
#include <sge/sprite/parameters.hpp>
#include <sge/image/multi_loader.hpp>
#include <sge/renderer/filter/linear.hpp>
#include <sge/renderer/resource_flags_none.hpp>
#include <sge/renderer/device.hpp>
#include <sge/texture/part_raw.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/input/mouse/collector.hpp>
#include <sge/input/mouse/axis.hpp>
#include <sge/input/mouse/axis_position.hpp>
#include <sge/image/file.hpp>
#include <sge/input/mouse/axis_event.hpp>
#include <sge/input/mouse/button_event.hpp>
#include <fcppt/math/vector/basic_impl.hpp>
#include <fcppt/io/cout.hpp>
#include <functional>

fruitcut::states::cut::cut(
	my_context ctx)
:
	my_base(
		ctx),
	ss_(
		context<machine>().systems().renderer()),
	cursor_(
		sprite::parameters()
			.texture_size()
			.texture(
				sge::texture::part_ptr(
					new sge::texture::part_raw(
						context<machine>().systems().renderer()->create_texture(
							context<machine>().systems().image_loader().load(
								media_path() 
									/ FCPPT_TEXT("textures") 
									/ FCPPT_TEXT("cursor.png"))->view(),
							sge::renderer::filter::linear,
							sge::renderer::resource_flags::none))))
			.pos(
				sprite::object::point(100,100))
			.elements()),
	mouse_axis_connection_(
		context<machine>().systems().mouse_collector()->axis_callback(
			std::bind(
				&cut::mouse_axis_callback,
				this,
				std::placeholders::_1))),
	mouse_button_connection_(
		context<machine>().systems().mouse_collector()->button_callback(
			std::bind(
				&cut::mouse_button_callback,
				this,
				std::placeholders::_1)))
{
	context<ingame>().current_input_state(
		input_states::cut);
}

boost::statechart::result
fruitcut::states::cut::react(
	events::render_overlay const &)
{
	sge::sprite::render_one(
		ss_,
		cursor_);
	return forward_event();
}

void
fruitcut::states::cut::mouse_axis_callback(
	sge::input::mouse::axis_event const &e)
{
	// Yeah, no switch for axis_type here. TODO!
	cursor_.pos( 
		cursor_.pos() + 
		sprite::object::point(
			static_cast<sprite::object::point::value_type>(
				e.axis() == sge::input::mouse::axis::x
				?
					e.axis_position()
				:
					0),
			static_cast<sprite::object::point::value_type>(
				e.axis() == sge::input::mouse::axis::y
				?
					e.axis_position()
				:
					0)));
}

void
fruitcut::states::cut::mouse_button_callback(
	sge::input::mouse::button_event const &)
{
}
