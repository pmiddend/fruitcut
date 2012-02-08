#ifndef FRUITAPP_LOGO_HPP_INCLUDED
#define FRUITAPP_LOGO_HPP_INCLUDED

#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/viewport_change.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
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
#include <fcppt/noncopyable.hpp>
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
	boost::mpl::vector2<fruitlib::scenic::events::render,fruitlib::scenic::events::viewport_change>
	scene_reactions;

	explicit
	logo(
		fruitlib::scenic::optional_parent const &,
		sge::renderer::device &,
		sge::image2d::system &,
		sge::parse::json::object const &);

	~logo();

	void
	react(
		fruitlib::scenic::events::render const &);

	void
	react(
		fruitlib::scenic::events::viewport_change const &);
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
			sge::sprite::config::texture_coordinates::automatic
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

	sge::renderer::device &renderer_;
	sprite_buffers sprite_buffers_;
	sprite_object sprite_object_;
};
}

#endif
