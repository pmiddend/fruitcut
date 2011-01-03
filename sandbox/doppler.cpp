/*
spacegameengine is a portable easy to use game engine written in C++.
Copyright (C) 2006-2010 Carl Philipp Reh (sefi@s-e-f-i.de)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/


#include <sge/systems/audio_player_default.hpp>
#include <sge/systems/instance.hpp>
#include <sge/systems/list.hpp>
#include <sge/audio/player.hpp>
#include <sge/config/media_path.hpp>
#include <sge/log/global.hpp>
#include <sge/audio/listener.hpp>
#include <sge/audio/file.hpp>
#include <sge/audio/sound/base.hpp>
#include <sge/audio/sound/positional.hpp>
#include <sge/audio/exception.hpp>
#include <sge/audio/multi_loader.hpp>
#include <sge/input/keyboard/action.hpp>
#include <sge/input/keyboard/device.hpp>
#include <sge/input/mouse/axis_event.hpp>
#include <sge/input/mouse/device.hpp>
#include <sge/systems/viewport/manage_resize.hpp>
#include <sge/exception.hpp>
#include <sge/extension_set.hpp>
#include <fcppt/assign/make_container.hpp>
#include <fcppt/log/activate_levels.hpp>
#include <fcppt/io/cerr.hpp>
#include <boost/lexical_cast.hpp>
#include <exception>
#include <iostream>
#include <ostream>
#include <vector>
#include <cstdlib>

int main(
	int argc,
	char *argv[])
try
{
	if (argc != 2)
	{
		std::cerr << "usage: doppler [pitch-intensity]\n";
		return EXIT_FAILURE;
	}

	fcppt::log::activate_levels(
		sge::log::global(),
		fcppt::log::level::debug);

	sge::systems::instance sys(
		sge::systems::list()
		(sge::systems::audio_player_default())
		(sge::systems::audio_loader(
				sge::audio::loader_capabilities_field::null(),
				fcppt::assign::make_container<sge::extension_set>(
					FCPPT_TEXT("ogg")))));

	sge::audio::file_ptr const af_siren(
		sys.audio_loader().load(
			sge::config::media_path()
			/ FCPPT_TEXT("siren.ogg")));

	sys.audio_player()->listener().position(
		sge::audio::vector::null());

	sys.audio_player()->listener().linear_velocity(
		sge::audio::vector(
			boost::lexical_cast<sge::audio::scalar>(
				argv[1]),
			0,
			0));

	sge::audio::sound::positional_ptr const sound_siren(
		sys.audio_player()->create_positional_stream(
			af_siren,
			sge::audio::sound::positional_parameters()
			.position(
				sge::audio::vector(
					1,
					0,
					0))
			.linear_velocity(
				sge::audio::vector(
					0,
					1,
					0))));

	// pitched_frequency = frequency * (speed_of_sound - doppler_factor * vls) / (speed_of_sound - doppler_factor * vss)
	// set speed of sound to 1, doppler factor to 1:
	// pitched_frequency = frequency * (1 - vls) / (1 - vss)
	// Set vss = 0 by setting the source velocity orthogonal to the vector from the source to the listener:
	// pitched_frequency = frequency * (1 - vls)
	// Voila, you can now modulate a sound by setting the listener velocity.
	sys.audio_player()->speed_of_sound(
		static_cast<sge::audio::scalar>(
			1));

	sound_siren->play(
		sge::audio::sound::repeat::loop);

	while(true)
		sound_siren->update();
}
catch(sge::exception const &e)
{
	fcppt::io::cerr << e.string() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
catch(std::exception const &e)
{
	fcppt::io::cerr << e.what() << FCPPT_TEXT('\n');
	return EXIT_FAILURE;
}
