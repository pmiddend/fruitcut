#include <fruitapp/exception.hpp>
#include <fruitapp/name.hpp>
#include <fruitapp/quick_log.hpp>
#include <fruitapp/screen_shooter.hpp>
#include <sge/config/app_name.hpp>
#include <sge/config/cache_path.hpp>
#include <sge/image2d/system_fwd.hpp>
#include <sge/input/key/action_callback.hpp>
#include <sge/input/key/code.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/renderer/screenshot.hpp>
#include <sge/renderer/device/core_fwd.hpp>
#include <fcppt/from_std_string.hpp>
#include <fcppt/string.hpp>
#include <fcppt/text.hpp>
#include <fcppt/either/error_from_optional.hpp>
#include <fcppt/either/to_exception.hpp>
#include <fcppt/filesystem/create_directory.hpp>
#include <fcppt/filesystem/create_directory_error.hpp>
#include <fcppt/filesystem/path_to_string.hpp>
#include <fcppt/io/cout.hpp>
#include <fcppt/signal/auto_connection.hpp>
#include <fcppt/signal/connection.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/system/error_code.hpp>
#include <functional>
#include <fcppt/config/external_end.hpp>


fruitapp::screen_shooter::screen_shooter(
	sge::input::keyboard::device &_keyboard,
	sge::renderer::device::core &_renderer,
	sge::image2d::system &_image_loader,
	fruitapp::quick_log &_log)
:
	renderer_(
		_renderer),
	image_loader_(
		_image_loader),
	callback_connection_(
		_keyboard.key_callback(
			sge::input::keyboard::action(
				sge::input::key::code::f12,
				sge::input::key::action_callback{
					std::bind(
						&screen_shooter::callback,
						this,
						std::ref(
							_log
						)
					)
				}
			)
		)
	)
{
}

fruitapp::screen_shooter::~screen_shooter()
{
}

void
fruitapp::screen_shooter::callback(
	fruitapp::quick_log &_log)
{
	fcppt::string const time_string =
		(fcppt::from_std_string(
				boost::posix_time::to_iso_string(
					boost::posix_time::second_clock::universal_time())));

	boost::filesystem::path const target_dir =
		sge::config::cache_path(
			sge::config::app_name(
				fruitapp::name()))
			/ FCPPT_TEXT("screenshots");

	fcppt::either::to_exception(
		fcppt::either::error_from_optional(
			fcppt::filesystem::create_directory(
				target_dir
			)
		),
		[
			&target_dir
		](
			boost::system::error_code const _error
		)
		{
			return
				fruitapp::exception{
					fcppt::filesystem::create_directory_error(
						target_dir,
						_error
					)
				};
		}
	);

	boost::filesystem::path const dest_path =
		target_dir/(time_string+FCPPT_TEXT(".png"));

	sge::renderer::screenshot(
		renderer_,
		image_loader_,
		dest_path);

	_log.add_message(
		FCPPT_TEXT("Screenshot taken"));
}
