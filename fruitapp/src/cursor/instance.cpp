#include <fruitapp/cursor/instance.hpp>
#include <fruitapp/cursor/manager.hpp>
#include <fruitapp/cursor/sampler.hpp>
#include <fruitapp/cursor/sound.hpp>
#include <fruitapp/cursor/sword_trail.hpp>
#include <fruitapp/fruit/hull/intersection_pair.hpp>
#include <fruitapp/fruit/hull/projected.hpp>
#include <fruitapp/fruit/hull/trail_intersection.hpp>
#include <fruitlib/time_format/find_and_convert_duration.hpp>
#include <sge/camera/base.hpp>
#include <sge/camera/coordinate_system/object.hpp>
#include <sge/camera/matrix_conversion/world_projection.hpp>
#include <sge/image/color/predef.hpp>
#include <sge/line_drawer/line.hpp>
#include <sge/parse/json/find_and_convert_member.hpp>
#include <sge/parse/json/path.hpp>
#include <fcppt/cref.hpp>
#include <fcppt/make_unique_ptr.hpp>
#include <fcppt/optional_impl.hpp>
#include <fcppt/ref.hpp>
#include <fcppt/text.hpp>
#include <fcppt/math/vector/object_impl.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/next_prior.hpp>
#include <fcppt/config/external_end.hpp>


fruitapp::cursor::instance::instance(
	fruitapp::cursor::manager &_manager,
	sge::input::cursor::object &_cursor_object,
	fruitlib::scenic::optional_parent const &_parent)
:
	cursor_object_(
		_cursor_object),
	target_(
		_manager.target_),
	camera_(
		_manager.camera_),
	sound_(
		fcppt::make_unique_ptr<fruitapp::cursor::sound>(
			fcppt::cref(
				_parent),
			fcppt::ref(
				_cursor_object),
			fcppt::cref(
				_manager.ingame_clock_),
			fcppt::ref(
				_manager.viewport_manager_),
			fcppt::ref(
				_manager.sound_controller_))),
	sampler_(
		fcppt::make_unique_ptr<fruitapp::cursor::sampler>(
			fcppt::cref(
				_parent),
			fcppt::ref(
				_cursor_object),
			fcppt::cref(
				_manager.ingame_clock_),
			fruitlib::time_format::find_and_convert_duration<fruitapp::ingame_clock::duration>(
				_manager.configuration_,
				sge::parse::json::path(
					FCPPT_TEXT("trail-update-rate"))),
			fruitapp::cursor::sampler::sample_count(
				sge::parse::json::find_and_convert_member<fruitapp::cursor::sampler::sample_count::value_type>(
					_manager.configuration_,
					sge::parse::json::path(
						FCPPT_TEXT("trail-samples")))),
			fcppt::cref(
				_manager.target_))),
	sword_trail_(
		fcppt::make_unique_ptr<fruitapp::cursor::sword_trail>(
			fcppt::cref(
				_parent),
			fcppt::ref(
				_cursor_object),
			fcppt::cref(
				_manager.ingame_clock_),
			fcppt::ref(
				_manager.renderer_),
			fcppt::cref(
				_manager.target_),
			fcppt::ref(
				_manager.texture_manager_),
			fcppt::cref(
				_manager.configuration_)))
{
}

void
fruitapp::cursor::instance::draw_trail(
	sge::line_drawer::line_sequence &_lines) const
{
	if(sampler_->positions().empty())
		return;

	for(
		fruitapp::cursor::sampler::position_buffer::const_iterator i(
			sampler_->positions().begin());
		i != boost::prior(sampler_->positions().end());
		++i)
		_lines.push_back(
			sge::line_drawer::line(
				sge::renderer::vector3(
					static_cast<sge::renderer::scalar>(
						i->x()),
					static_cast<sge::renderer::scalar>(
						i->y()),
					static_cast<sge::renderer::scalar>(
						0)),
				sge::renderer::vector3(
					static_cast<sge::renderer::scalar>(
						boost::next(i)->x()),
					static_cast<sge::renderer::scalar>(
						boost::next(i)->y()),
					static_cast<sge::renderer::scalar>(
						0)),
				sge::image::color::predef::red(),
				sge::image::color::predef::red()));
}

fruitapp::fruit::hull::optional_intersection_pair const
fruitapp::cursor::instance::calculate_fruit_intersection(
	fruitapp::fruit::object const &_fruit) const
{
	return
		fruitapp::fruit::hull::trail_intersection(
			fruitapp::fruit::hull::projected(
				_fruit,
				target_,
				sge::camera::matrix_conversion::world_projection(
					camera_.coordinate_system(),
					camera_.projection_matrix())),
			sampler_->positions());
}

sge::input::cursor::object const &
fruitapp::cursor::instance::cursor_object() const
{
	return
		cursor_object_;
}

fruitapp::ingame_clock::duration const
fruitapp::cursor::instance::total_expiry_duration() const
{
	return
		sampler_->total_expiry_duration();
}

fruitapp::cursor::instance::~instance()
{
}
