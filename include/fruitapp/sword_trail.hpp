#ifndef FRUITAPP_SWORD_TRAIL_HPP_INCLUDED
#define FRUITAPP_SWORD_TRAIL_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/ingame_timer.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/update.hpp>
#include <sge/image/color/rgba8_format.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/input/cursor/object_fwd.hpp>
#include <sge/parse/json/object.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/renderer/target_base_fwd.hpp>
#include <sge/renderer/texture/planar_shared_ptr.hpp>
#include <sge/sprite/object_decl.hpp>
#include <sge/sprite/parameters_fwd.hpp>
#include <sge/sprite/buffers/single_decl.hpp>
#include <sge/sprite/buffers/with_declaration_decl.hpp>
#include <sge/sprite/config/choices.hpp>
#include <sge/sprite/config/custom_center.hpp>
#include <sge/sprite/config/float_type.hpp>
#include <sge/sprite/config/normal_size.hpp>
#include <sge/sprite/config/texture_coordinates.hpp>
#include <sge/sprite/config/texture_level_count.hpp>
#include <sge/sprite/config/type_choices.hpp>
#include <sge/sprite/config/unit_type.hpp>
#include <sge/sprite/config/with_rotation.hpp>
#include <sge/sprite/config/with_texture.hpp>
#include <sge/texture/part_shared_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/shared_ptr.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
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
		sge::image2d::system &,
		sge::input::cursor::object &,
		fruitapp::ingame_clock const &,
		sge::parse::json::object const &);

	~sword_trail();

	void
	react(
		fruitlib::scenic::events::update const &);

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	// Arbitrary choice
	typedef
	float
	float_type;

	typedef
	sge::sprite::config::choices
	<
		sge::sprite::config::type_choices
		<
			sge::sprite::config::unit_type
			<
				float_type
			>,
			sge::sprite::config::float_type
			<
				float_type
			>
		>,
		sge::sprite::config::normal_size,
		boost::mpl::vector2
		<
			sge::sprite::config::with_texture
			<
				sge::sprite::config::texture_level_count
				<
					1u
				>,
				sge::sprite::config::texture_coordinates::automatic,
				sge::sprite::config::texture_ownership::shared
			>,
			sge::sprite::config::with_rotation
			<
				sge::sprite::config::custom_center
				<
					false
				>
			>
		>
	>
	sprite_choices;

	typedef
	sge::sprite::buffers::with_declaration
	<
		sge::sprite::buffers::single
		<
			sprite_choices
		>
	>
	sprite_buffers;

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
		fcppt::shared_ptr<fruitapp::ingame_timer>
	>
	timer_buffer;

	sge::input::cursor::object &cursor_;
	sge::renderer::target_base &target_;
	fruitapp::ingame_clock const &clock_;
	fruitapp::ingame_clock::duration const element_lifetime_;
	sprite_object::unit const max_width_;
	sge::renderer::texture::planar_shared_ptr texture_;
	sge::texture::part_shared_ptr texture_part_;
	sprite_buffers sprite_buffers_;
	position_buffer positions_;
	sprite_buffer sprites_;
	timer_buffer timers_;
	fruitapp::ingame_timer update_timer_;

};
}

#endif
