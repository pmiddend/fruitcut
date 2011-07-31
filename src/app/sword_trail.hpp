#ifndef FRUITCUT_APP_SWORD_TRAIL_HPP_INCLUDED
#define FRUITCUT_APP_SWORD_TRAIL_HPP_INCLUDED

#include "../fruitlib/scenic/node.hpp"
#include "../fruitlib/scenic/optional_parent.hpp"
#include "../fruitlib/scenic/events/update_fwd.hpp"
#include "../fruitlib/scenic/events/render_fwd.hpp"
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/image/color/rgba8_format.hpp>
#include <sge/input/cursor/object_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/target_base_fwd.hpp>
#include <sge/sprite/choices.hpp>
#include <sge/sprite/external_system_impl.hpp>
#include <sge/sprite/object.hpp>
#include <sge/sprite/parameters.hpp>
#include <sge/sprite/system.hpp>
#include <sge/sprite/type_choices.hpp>
#include <sge/sprite/with_dim.hpp>
#include <sge/sprite/with_rotation.hpp>
#include <sge/sprite/with_texture.hpp>
#include <sge/texture/part_ptr.hpp>
#include <sge/time/timer.hpp>
#include <sge/time/funit.hpp>
#include <sge/time/callback.hpp>
#include <sge/time/duration.hpp>
#include <sge/parse/json/object.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/chrono/duration_impl.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <boost/circular_buffer.hpp>

namespace fruitcut
{
namespace app
{
class sword_trail
:
	public fruitlib::scenic::node<sword_trail>
{
FCPPT_NONCOPYABLE(
	sword_trail);
public:
	typedef
	boost::mpl::vector2<fruitlib::scenic::events::update,fruitlib::scenic::events::render>
	scene_reactions;

	explicit
	sword_trail(
		fruitlib::scenic::optional_parent const &,
		sge::renderer::device &,
		sge::renderer::target_base &,
		sge::image2d::multi_loader &,
		sge::input::cursor::object &,
		sge::time::callback const &,
		sge::parse::json::object const &);

	~sword_trail();

	void
	react(
		fruitlib::scenic::events::update const &);

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	typedef 
	sge::sprite::choices
	<
		sge::sprite::type_choices
		<
			// position type
			sge::time::funit,
			// rotation/etc type
			sge::time::funit,
			sge::image::color::rgba8_format
		>,
		boost::mpl::vector3
		<
			sge::sprite::with_dim,
			sge::sprite::with_texture,
			sge::sprite::with_rotation
		>
	> 
	sprite_choices;

	typedef 
	sge::sprite::system<sprite_choices>::type 
	sprite_system;

	typedef 
	sge::sprite::object<sprite_choices> 
	sprite_object;

	typedef 
	sge::sprite::parameters<sprite_choices> 
	sprite_parameters;

	typedef 
	boost::circular_buffer<sprite_object> 
	sprite_buffer;

	typedef	
	boost::circular_buffer<sprite_object::vector>
	position_buffer;

	typedef	
	boost::circular_buffer
	<
		fcppt::shared_ptr<sge::time::timer> 
	>
	timer_buffer;

	sge::input::cursor::object &cursor_;
	sge::renderer::target_base &target_;
	sge::time::callback const time_callback_;
	sge::time::duration const update_interval_;
	sge::time::duration const element_lifetime_;
	sprite_object::unit const max_width_;
	sge::texture::part_ptr texture_;
	sprite_system sprite_system_;
	position_buffer positions_;
	sprite_buffer sprites_;
	timer_buffer timers_;
	sge::time::timer update_timer_;

};
}
}

#endif
