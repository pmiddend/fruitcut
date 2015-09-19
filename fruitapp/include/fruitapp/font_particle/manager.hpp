#ifndef FRUITAPP_FONT_PARTICLE_MANAGER_HPP_INCLUDED
#define FRUITAPP_FONT_PARTICLE_MANAGER_HPP_INCLUDED

#include <fruitapp/ingame_clock.hpp>
#include <fruitapp/font_particle/lifetime.hpp>
#include <fruitapp/font_particle/object_fwd.hpp>
#include <fruitapp/font_particle/position.hpp>
#include <fruitapp/font_particle/text.hpp>
#include <fruitapp/projection_manager/object_fwd.hpp>
#include <fruitlib/perspective_projection_information_fwd.hpp>
#include <fruitlib/font/identifier.hpp>
#include <fruitlib/font/manager_fwd.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <sge/image/color/any/object_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/unique_ptr.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <vector>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
namespace font_particle
{
class manager
{
FCPPT_NONCOPYABLE(
	manager);
public:
	manager(
		fruitlib::scenic::optional_parent const &,
		fruitapp::projection_manager::object &,
		fruitapp::ingame_clock const &,
		fruitlib::font::manager &);

	void
	add_particle(
		fruitapp::font_particle::text const &,
		fruitlib::font::identifier const &,
		fruitapp::font_particle::position const &,
		fruitapp::font_particle::lifetime const &,
		sge::image::color::any::object const &);

	void
	update();

	~manager();
private:
	typedef
	std::vector<
		fcppt::unique_ptr<
			fruitapp::font_particle::object
		>
	>
	object_sequence;

	fruitlib::scenic::optional_parent const parent_;
	fruitlib::font::manager &font_manager_;
	fruitapp::ingame_clock const &ingame_clock_;
	object_sequence objects_;
	fcppt::signal::auto_connection const projection_change_connection_;

	void
	projection_change(
		fruitlib::perspective_projection_information const &);
};
}
}

#endif
