#ifndef FRUITAPP_LOGO_HPP_INCLUDED
#define FRUITAPP_LOGO_HPP_INCLUDED

#include <fruitapp/viewport/manager_fwd.hpp>
#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/device/ffp_fwd.hpp>
#include <sge/renderer/target/viewport.hpp>
#include <sge/renderer/texture/emulate_srgb.hpp>
#include <sge/sprite/object_decl.hpp>
#include <sge/sprite/parameters_fwd.hpp>
#include <sge/sprite/buffers/single_decl.hpp>
#include <sge/sprite/buffers/with_declaration_decl.hpp>
#include <sge/sprite/config/choices.hpp>
#include <sge/sprite/config/float_type.hpp>
#include <sge/sprite/config/normal_size.hpp>
#include <sge/sprite/config/texture_coordinates.hpp>
#include <sge/sprite/config/texture_level_count.hpp>
#include <sge/sprite/config/type_choices.hpp>
#include <sge/sprite/config/unit_type.hpp>
#include <sge/sprite/config/with_texture.hpp>
#include <sge/sprite/state/all_choices.hpp>
#include <sge/sprite/state/object.hpp>
#include <sge/sprite/state/parameters.hpp>
#include <sge/texture/const_part_scoped_ptr.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
class logo
:
	public fruitlib::scenic::node<logo>
{
FCPPT_NONCOPYABLE(
	logo);
public:
	typedef
	boost::mpl::vector1<fruitlib::scenic::events::render>
	scene_reactions;

	logo(
		fruitlib::scenic::optional_parent const &,
		sge::renderer::device::ffp &,
		sge::image2d::system &,
		sge::parse::json::object const &,
		fruitapp::viewport::manager &,
		sge::renderer::texture::emulate_srgb::type);

	~logo();

	void
	react(
		fruitlib::scenic::events::render const &);
private:
	typedef
	boost::mpl::vector1
	<
		sge::sprite::config::with_texture
		<
			sge::sprite::config::texture_level_count
			<
				1u
			>,
			sge::sprite::config::texture_coordinates::automatic,
			sge::sprite::config::texture_ownership::reference
		>
	>
	sprite_elements;

	typedef
	sge::sprite::config::type_choices
	<
		sge::sprite::config::unit_type
		<
			int
		>,
		sge::sprite::config::float_type
		<
			float
		>
	>
	sprite_type_choices;

	typedef
	sge::sprite::config::choices
	<
		sprite_type_choices,
		sge::sprite::config::normal_size,
		sprite_elements
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
	sge::sprite::state::all_choices
	sprite_state_choices;

	typedef
	sge::sprite::state::object<sprite_state_choices>
	sprite_state_object;

	typedef
	sge::sprite::state::parameters<sprite_state_choices>
	sprite_state_parameters;

	sprite_state_object sprite_states_;
	sprite_buffers sprite_buffers_;
	sge::texture::const_part_scoped_ptr const texture_;
	sprite_object sprite_object_;
	fcppt::signal::scoped_connection viewport_change_connection_;

	void
	viewport_change(
		sge::renderer::target::viewport const &);
};
}

#endif
