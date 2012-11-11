#ifndef FRUITAPP_CURSOR_INSTANCE_HPP_INCLUDED
#define FRUITAPP_CURSOR_INSTANCE_HPP_INCLUDED

#include <fruitapp/cursor/manager_fwd.hpp>
#include <fruitapp/cursor/sampler_fwd.hpp>
#include <fruitapp/cursor/sound_fwd.hpp>
#include <sge/camera/base_fwd.hpp>
#include <fruitapp/cursor/sword_trail_fwd.hpp>
#include <fruitapp/fruit/object_fwd.hpp>
#include <fruitapp/fruit/hull/intersection_pair.hpp>
#include <fruitapp/ingame_clock.hpp>
#include <fruitlib/scenic/optional_parent.hpp>
#include <sge/input/cursor/object_fwd.hpp>
#include <sge/line_drawer/line_sequence.hpp>
#include <sge/renderer/target/base_fwd.hpp>
#include <fcppt/noncopyable.hpp>
#include <fcppt/scoped_ptr.hpp>


namespace fruitapp
{
namespace cursor
{
class instance
{
FCPPT_NONCOPYABLE(
	instance);
public:
	instance(
		fruitapp::cursor::manager &,
		sge::input::cursor::object &,
		fruitlib::scenic::optional_parent const &);

	void
	draw_trail(
		sge::line_drawer::line_sequence &) const;

	fruitapp::fruit::hull::intersection_pair const
	calculate_fruit_intersection(
		fruitapp::fruit::object const &) const;

	sge::input::cursor::object const &
	cursor_object() const;

	fruitapp::ingame_clock::duration const
	total_expiry_duration() const;

	~instance();
private:
	sge::input::cursor::object const &cursor_object_;
	sge::renderer::target::base const &target_;
	sge::camera::base const &camera_;
	fcppt::scoped_ptr<fruitapp::cursor::sound> const sound_;
	fcppt::scoped_ptr<fruitapp::cursor::sampler> const sampler_;
	fcppt::scoped_ptr<fruitapp::cursor::sword_trail> const sword_trail_;
};
}
}

#endif
