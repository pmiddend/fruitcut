#ifndef FRUITCUT_APP_LOGO_HPP_INCLUDED
#define FRUITCUT_APP_LOGO_HPP_INCLUDED

#include "../fruitlib/scenic/nodes/intrusive.hpp"
#include "../fruitlib/animation.hpp"
#include <sge/sprite/sprite.hpp>
#include <sge/renderer/device_fwd.hpp>
#include <sge/viewport/manager_fwd.hpp>
#include <sge/image/color/rgba8_format.hpp>
#include <sge/image2d/multi_loader_fwd.hpp>
#include <sge/parse/json/object_fwd.hpp>
#include <fcppt/signal/scoped_connection.hpp>
#include <fcppt/noncopyable.hpp>
#include <boost/mpl/vector/vector10.hpp>

namespace fruitcut
{
namespace app
{
class logo
:
	public fruitlib::scenic::nodes::intrusive
{
FCPPT_NONCOPYABLE(
	logo);
public:
	explicit
	logo(
		sge::renderer::device &,
		sge::viewport::manager &,
		sge::image2d::multi_loader &,
		sge::parse::json::object const &);
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

	typedef
	fruitlib::animation<sprite_object::color_type>
	color_animation;

	sge::renderer::device &renderer_;
	sprite_system sprite_system_;
	sprite_object sprite_object_;
	fcppt::signal::scoped_connection viewport_change_connection_;
	color_animation color_animation_;

	void
	update();

	void
	render();

	void
	viewport_change();
};
}
}

#endif
