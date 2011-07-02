#include "screen_shooter.hpp"
#include "name.hpp"
#include "quick_log.hpp"
#include <sge/input/keyboard/device.hpp>
#include <sge/input/keyboard/key_code.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/renderer/screenshot.hpp>
#include <sge/config/cache_path.hpp>
#include <fcppt/tr1/functional.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/filesystem/path_to_string.hpp>
#include <fcppt/filesystem/create_directory_exn.hpp>
#include <fcppt/filesystem/exists.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/text.hpp>
#include <fcppt/ref.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

fruitcut::app::screen_shooter::screen_shooter(
	sge::input::keyboard::device &_keyboard,
	sge::renderer::device &_renderer,
	sge::image2d::multi_loader &_image_loader,
	app::quick_log &_log)
:
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
					this,
					fcppt::ref(
						_log)))))
{
}

fruitcut::app::screen_shooter::~screen_shooter()
{
}

void
fruitcut::app::screen_shooter::callback(
	app::quick_log &_log)
{
	fcppt::string const time_string = 
		(fcppt::from_std_string(
				boost::posix_time::to_iso_string(
					boost::posix_time::second_clock::universal_time())));

	fcppt::filesystem::path const target_dir = 
		sge::config::cache_path(
			app::name())
			/ FCPPT_TEXT("screenshots");

	if(!fcppt::filesystem::exists(target_dir))
		fcppt::filesystem::create_directory_exn(
			target_dir);

	fcppt::filesystem::path const dest_path = 
		target_dir/(time_string+FCPPT_TEXT(".png"));

	sge::renderer::screenshot(
		renderer_,
		image_loader_,
		dest_path);

	_log.add_message( 
		FCPPT_TEXT("Screenshot taken"));
}
