#ifndef FRUITAPP_SYSTEMS_HPP_INCLUDED
#define FRUITAPP_SYSTEMS_HPP_INCLUDED

#include <sge/systems/cursor_demuxer.hpp>
#include <sge/systems/cursor_option_field.hpp>
#include <sge/systems/instance_fwd.hpp>
#include <sge/systems/keyboard_collector.hpp>
#include <sge/systems/mouse_collector.hpp>
#include <sge/systems/renderer_caps.hpp>
#include <sge/systems/with_audio_loader.hpp>
#include <sge/systems/with_audio_player.hpp>
#include <sge/systems/with_font.hpp>
#include <sge/systems/with_image2d.hpp>
#include <sge/systems/with_input.hpp>
#include <sge/systems/with_renderer.hpp>
#include <sge/systems/with_window.hpp>
#include <fcppt/config/external_begin.hpp>
#include <boost/mpl/vector/vector10.hpp>
#include <fcppt/config/external_end.hpp>


namespace fruitapp
{
typedef
sge::systems::instance
<
	boost::mpl::vector7
	<
		sge::systems::with_renderer<sge::systems::renderer_caps::ffp>,
		sge::systems::with_window,
		sge::systems::with_font,
		sge::systems::with_audio_player,
		sge::systems::with_audio_loader,
		sge::systems::with_image2d,
		sge::systems::with_input
		<
			boost::mpl::vector3
			<
				sge::systems::keyboard_collector,
				sge::systems::mouse_collector,
				sge::systems::cursor_demuxer
			>
		>
	>
>
systems;
}

#endif
