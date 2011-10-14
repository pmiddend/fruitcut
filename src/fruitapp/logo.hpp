#ifndef FRUITAPP_LOGO_HPP_INCLUDED
#define FRUITAPP_LOGO_HPP_INCLUDED

#include <fruitlib/scenic/node.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <fruitlib/scenic/events/render.hpp>
#include <fruitlib/scenic/events/viewport_change.hpp>
#include <sge/sprite/with_texture.hpp>
#include <sge/sprite/with_repetition.hpp>
#include <sge/sprite/with_visibility.hpp>
#include <sge/sprite/with_dim.hpp>
#include <sge/sprite/with_rotation.hpp>
#include <sge/sprite/with_color.hpp>
#include <sge/sprite/intrusive/tag.hpp>
#include <sge/sprite/intrusive/system_impl.hpp>
#include <sge/sprite/type_choices.hpp>
#include <sge/sprite/choices.hpp>
#include <sge/sprite/system.hpp>
#include <sge/sprite/object.hpp>
#include <sge/sprite/parameters_fwd.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/image/color/rgba8_format.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

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
		sge::image2d::multi_loader &,
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
	boost::mpl::vector7
	<
		sge::sprite::with_texture,
		sge::sprite::with_repetition,
		sge::sprite::with_visibility,
		sge::sprite::with_dim,
		sge::sprite::with_rotation,
		sge::sprite::with_color,
		sge::sprite::intrusive::tag
	>
	sprite_elements;

	typedef
	sge::sprite::type_choices
	<
		int,
		float,
		sge::image::color::rgba8_format
	>
	sprite_type_choices;

	typedef
	sge::sprite::choices
	<
		sprite_type_choices,
		sprite_elements
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

	sge::renderer::device &renderer_;
	sprite_system sprite_system_;
	sprite_object sprite_object_;
};
}

#endif
