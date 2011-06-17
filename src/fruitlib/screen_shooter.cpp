#include "screen_shooter.hpp"
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/renderer/screenshot.hpp>
#include <sge/config/cache_path.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/filesystem/path_to_string.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/text.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

fruitcut::fruitlib::screen_shooter::screen_shooter(
	fcppt::string const &_app_name,
	sge::input::keyboard::device &_keyboard,
	sge::renderer::device &_renderer,
	sge::image2d::multi_loader &_image_loader)
:
	app_name_(
		_app_name),
	renderer_(
		_renderer),
	image_loader_(
		_image_loader),
	callback_connection_(
		_keyboard.key_callback(
			sge::input::keyboard::action(
				sge::input::keyboard::key_code::f12, 
				std::tr1::bind(
					&screen_shooter::callback,
					this))))
{
}

fruitcut::fruitlib::screen_shooter::~screen_shooter()
{
}

void
fruitcut::fruitlib::screen_shooter::callback()
{
	fcppt::string const time_string = 
		(fcppt::from_std_string(
				boost::posix_time::to_iso_string(
					boost::posix_time::second_clock::universal_time())));

	fcppt::filesystem::path const dest_path = 
		sge::config::cache_path(
			app_name_)/(time_string+FCPPT_TEXT(".png"));

	sge::renderer::screenshot(
		renderer_,
		image_loader_,
		dest_path);
	fcppt::io::cout << FCPPT_TEXT("Took a screenshot and saved it to ") << fcppt::filesystem::path_to_string(dest_path) << FCPPT_TEXT("\n");
}
