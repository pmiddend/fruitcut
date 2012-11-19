#include <fruitapp/font_particle/manager.hpp>
#include <fruitapp/font_particle/object.hpp>
#include <fruitapp/projection_manager/object.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/container/ptr/push_back_unique_ptr.hpp>
#include <fcppt/tr1/functional.hpp>


fruitapp::font_particle::manager::manager(
	fruitlib::scenic::optional_parent const &_parent,
	fruitapp::projection_manager::object &_projection_manager,
	fruitapp::ingame_clock const &_ingame_clock,
	fruitlib::font::manager &_font_manager)
:
	parent_(
		_parent),
	font_manager_(
		_font_manager),
	ingame_clock_(
		_ingame_clock),
	objects_(),
	projection_change_connection_(
		_projection_manager.projection_change_callback(
			std::tr1::bind(
				&fruitapp::font_particle::manager::projection_change,
				this,
				std::tr1::placeholders::_1),
			fruitapp::projection_manager::trigger_early(
				false)))
{
}

void
fruitapp::font_particle::manager::add_particle(
	fruitapp::font_particle::text const &_text,
	fruitlib::font::identifier const &_identifier,
	fruitapp::font_particle::position const &_position,
	fruitapp::font_particle::lifetime const &_lifetime,
	sge::image::color::any::object const &_color)
{
	fcppt::container::ptr::push_back_unique_ptr(
		objects_,
		fcppt::make_unique_ptr<fruitapp::font_particle::object>(
			fcppt::cref(
				parent_),
			fcppt::ref(
				font_manager_),
			_identifier,
			_text,
			_position,
			fcppt::cref(
				ingame_clock_),
			_lifetime,
			_color));
}

void
fruitapp::font_particle::manager::update()
{
	for(
		object_sequence::iterator it =
			objects_.begin();
		it != objects_.end();)
	{
		it->update();
		if(it->dead())
			it =
				objects_.erase(
					it);
		else
			++it;
	}
}

fruitapp::font_particle::manager::~manager()
{
}

void
fruitapp::font_particle::manager::projection_change(
	fruitlib::perspective_projection_information const &)
{
	objects_.clear();
}
