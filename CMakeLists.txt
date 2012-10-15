cmake_minimum_required(VERSION 2.6)

project(
  fruitcut)

include(
  FcpptCMakeUtils)

include(
  FindFcpptThreads)

include(
  AwlMainGenerator)

find_package(
	Boost 1.49.0 REQUIRED COMPONENTS
	chrono
	date_time
	thread
	system)

include_directories(
  ${Boost_INCLUDE_DIR})
link_directories(
  ${Boost_LIBRARY_DIRS})

add_definitions(
        -D BOOST_ASIO_SEPARATE_COMPILATION)
# boost end

# fcppt begin
find_package(
  Fcppt REQUIRED)
include_directories(
  ${Fcppt_INCLUDE_DIRS})
ADD_DEFINITIONS(
  ${Fcppt_DEFINITIONS})
# fcppt end

# awl begin
find_package(
  Awl REQUIRED)
include_directories(
  ${Awl_INCLUDE_DIR})
add_definitions(
  ${Awl_DEFINITIONS})
# awl end

# mizuiro begin
find_package(
  Mizuiro REQUIRED)
include_directories(
  ${Mizuiro_INCLUDE_DIR})
add_definitions(
  ${Mizuiro_DEFINITIONS})
# mizuiro end

# bullet begin
find_package(
  Bullet REQUIRED)
include_directories(
  ${BULLET_INCLUDE_DIRS})
# bullet end

# cg begin
find_package(
	Cg REQUIRED)
include_directories(
	${Cg_INCLUDE_DIR})
add_definitions(
	${Cg_DEFINITIONS})
# cg end

# sge begin
FIND_PACKAGE(
	SGE REQUIRED COMPONENTS
	audio
	camera
	cegui
	charconv
	cg
	config
	console
	core
	fontdraw
	fontbitmap
	font
	image
	image2d
	input
	line_drawer
	log
	modelmd3
	parse
	renderer
	shader
	sprite
	systems
	texture
	viewport
	window)

include_directories(${SGE_INCLUDE_DIRS})
add_definitions(${SGE_DEFINITIONS})
# sge end

# cegui begin
find_package(
	CEGUI REQUIRED)

link_directories(
	"${CEGUI_LIBRARY_DIRS}")

include_directories(
  ${CEGUI_INCLUDE_DIRS})
# cegui end

# get rid of a pretty useless warning
if(CMAKE_COMPILER_IS_GNUCXX OR FCPPT_UTILS_COMPILER_IS_CLANGPP)
	add_definitions(-Wno-old-style-cast)
elseif(MSVC)
	# Disable "this" used in initializer list warning
	add_definitions("/wd4355")
endif()

set(
	INSTALL_MEDIA_DIR
	${INSTALL_DATA_DIR}/media
)

include(
	SGECustomPath
)

sge_config_add_custom_path(
	fruitcut
	media
	${CMAKE_SOURCE_DIR}/media
	${INSTALL_MEDIA_DIR}
)

install(
	DIRECTORY
	${CMAKE_SOURCE_DIR}/media/
	DESTINATION
	${INSTALL_MEDIA_DIR}
)

option(
  ENABLE_TEST "Build the tests" ON)

include_directories(
	${CMAKE_SOURCE_DIR}/include)

if(ENABLE_TEST)
	enable_testing()
	add_subdirectory(
		src/test)
endif()

add_subdirectory(
	src/sandbox)

set(
	FRUITLIB_FILES
	src/fruitlib/audio/group/buffer.cpp
	src/fruitlib/audio/group/player.cpp
	src/fruitlib/audio/group/sound_base.cpp
	src/fruitlib/audio/group/sound_positional.cpp
	src/fruitlib/audio/music_controller.cpp
	src/fruitlib/audio/pool.cpp
	src/fruitlib/audio/sound_controller.cpp
	src/fruitlib/average_colors.cpp
	src/fruitlib/create_command_line_parameters.cpp
	src/fruitlib/exception.cpp
	src/fruitlib/fcppt_string_to_utf8_file.cpp
	src/fruitlib/font/cache.cpp
	src/fruitlib/font/object.cpp
	src/fruitlib/font/object_parameters.cpp
	src/fruitlib/font/scene_node.cpp
	src/fruitlib/json/parse_projection.cpp
	src/fruitlib/json/parse_rgba8_color.cpp
	src/fruitlib/log/scoped.cpp
	src/fruitlib/log/scoped_sequence_from_json.cpp
	src/fruitlib/log/string_to_location.cpp
	src/fruitlib/math/view_plane_distance.cpp
	src/fruitlib/math/view_plane_rect.cpp
	src/fruitlib/media_path.cpp
	src/fruitlib/performance_timer.cpp
	src/fruitlib/perspective_projection_information.cpp
	src/fruitlib/perspective_projection_information_to_matrix.cpp
	src/fruitlib/physics/debugger.cpp
	src/fruitlib/physics/group/object.cpp
	src/fruitlib/physics/nodes/debugger.cpp
	src/fruitlib/physics/nodes/world.cpp
	src/fruitlib/physics/null_collision_filter.cpp
	src/fruitlib/physics/rigid_body/collision_data.cpp
	src/fruitlib/physics/rigid_body/object.cpp
	src/fruitlib/physics/rigid_body/parameters.cpp
	src/fruitlib/physics/rigid_body/scoped.cpp
	src/fruitlib/physics/world.cpp
	src/fruitlib/pp/filter/add.cpp
	src/fruitlib/pp/filter/base.cpp
	src/fruitlib/pp/filter/binary.cpp
	src/fruitlib/pp/filter/blur.cpp
	src/fruitlib/pp/filter/desaturate.cpp
	src/fruitlib/pp/filter/highlight.cpp
	src/fruitlib/pp/filter/inject_texture.cpp
	src/fruitlib/pp/filter/manager.cpp
	src/fruitlib/pp/filter/nullary.cpp
	src/fruitlib/pp/filter/render_to_texture.cpp
	src/fruitlib/pp/filter/ssaa.cpp
	src/fruitlib/pp/filter/unary.cpp
	src/fruitlib/pp/filter/wrapper.cpp
	src/fruitlib/pp/screen_vf/quad.cpp
	src/fruitlib/pp/system.cpp
	src/fruitlib/pp/texture/descriptor.cpp
	src/fruitlib/pp/texture/instance.cpp
	src/fruitlib/pp/texture/manager.cpp
	src/fruitlib/pp/texture/use_screen_size.cpp
	src/fruitlib/resource_tree/path.cpp
	src/fruitlib/scenic/adaptors/camera.cpp
	src/fruitlib/scenic/adaptors/console.cpp
	src/fruitlib/scenic/adaptors/gui_system.cpp
	src/fruitlib/scenic/adaptors/line_drawer.cpp
	src/fruitlib/scenic/base.cpp
	src/fruitlib/scenic/delta/clock.cpp
	src/fruitlib/scenic/events/base.cpp
	src/fruitlib/scenic/events/render.cpp
	src/fruitlib/scenic/events/update.cpp
	src/fruitlib/scenic/no_parent.cpp
	src/fruitlib/scenic/parent.cpp
	src/fruitlib/scoped_frame_limiter.cpp
	src/fruitlib/signal_stack_printer/object.cpp
	src/fruitlib/time_format/milliseconds.cpp
	src/fruitlib/time_format/minutes.cpp
	src/fruitlib/time_format/seconds.cpp
	src/fruitlib/utf8_file_to_fcppt_string.cpp
	src/fruitlib/utf8_file_to_fcppt_string_exn.cpp
)

fcppt_utils_add_source_groups(
  "${FRUITLIB_FILES}")

SET(
	APP_FILES
	src/fruitapp/asio_implementation.cpp
	src/fruitapp/background.cpp
	src/fruitapp/config_variables.cpp
	src/fruitapp/current_commit.cpp
	src/fruitapp/cursor_sound.cpp
	src/fruitapp/cursor_trail.cpp
	src/fruitapp/directional_light_source.cpp
	src/fruitapp/exception.cpp
	src/fruitapp/fruit/cut_context.cpp
	src/fruitapp/fruit/cut_mesh.cpp
	src/fruitapp/fruit/cut_mesh_result.cpp
	src/fruitapp/fruit/default_render_node.cpp
	src/fruitapp/fruit/hull/projected.cpp
	src/fruitapp/fruit/hull/trail_intersection.cpp
	src/fruitapp/fruit/manager.cpp
	src/fruitapp/fruit/material/from_json.cpp
	src/fruitapp/fruit/material/object.cpp
	src/fruitapp/fruit/mesh.cpp
	src/fruitapp/fruit/mesh_to_point_cloud.cpp
	src/fruitapp/fruit/mesh_to_shape.cpp
	src/fruitapp/fruit/mesh_to_vertex_buffer.cpp
	src/fruitapp/fruit/model_to_mesh.cpp
	src/fruitapp/fruit/object.cpp
	src/fruitapp/fruit/object_from_prototype.cpp
	src/fruitapp/fruit/prototype.cpp
	src/fruitapp/fruit/prototype_from_json.cpp
	src/fruitapp/fruit/shadow_render_node.cpp
	src/fruitapp/fruit/spawner.cpp
	src/fruitapp/game_logic/object.cpp
	src/fruitapp/gui/button.cpp
	src/fruitapp/gui/combobox.cpp
	src/fruitapp/gui/progress_slider.cpp
	src/fruitapp/gui/table/column.cpp
	src/fruitapp/gui/table/model.cpp
	src/fruitapp/gui/table/view.cpp
	src/fruitapp/highscore/entry.cpp
	src/fruitapp/highscore/entry_set_to_json.cpp
	src/fruitapp/highscore/get_model.cpp
	src/fruitapp/highscore/json_to_entry_set.cpp
	src/fruitapp/highscore/post_model.cpp
	src/fruitapp/highscore/provider/connection_base.cpp
	src/fruitapp/highscore/provider/file/connection.cpp
	src/fruitapp/highscore/provider/file/object.cpp
	src/fruitapp/highscore/provider/net/connection.cpp
	src/fruitapp/highscore/provider/net/object.cpp
	src/fruitapp/highscore/provider/object_base.cpp
	src/fruitapp/highscore/providers_from_json.cpp
	src/fruitapp/light_source_from_json.cpp
	src/fruitapp/load_user_config.cpp
	src/fruitapp/logo.cpp
	src/fruitapp/machine.cpp
	src/fruitapp/machine_impl.cpp
	src/fruitapp/main.cpp
	src/fruitapp/name.cpp
	src/fruitapp/overlay.cpp
	src/fruitapp/point_sprite/base.cpp
	src/fruitapp/point_sprite/splatter/object.cpp
	src/fruitapp/point_sprite/splatter/parameters.cpp
	src/fruitapp/point_sprite/system_node.cpp
	src/fruitapp/postprocessing.cpp
	src/fruitapp/projection_manager/object.cpp
	src/fruitapp/quick_log.cpp
	src/fruitapp/renderable.cpp
	src/fruitapp/scene.cpp
	src/fruitapp/scoped_pp_activation.cpp
	src/fruitapp/scoped_scene_activation.cpp
	src/fruitapp/scoped_time_factor.cpp
	src/fruitapp/screen_shooter.cpp
	src/fruitapp/shadow_map.cpp
	src/fruitapp/splatter_generator.cpp
	src/fruitapp/states/gameover/choose_name.cpp
	src/fruitapp/states/gameover/ranking.cpp
	src/fruitapp/states/gameover/superstate.cpp
	src/fruitapp/states/ingame/paused.cpp
	src/fruitapp/states/ingame/running.cpp
	src/fruitapp/states/ingame/superstate.cpp
	src/fruitapp/states/loading.cpp
	src/fruitapp/states/menu/highscore.cpp
	src/fruitapp/states/menu/main.cpp
	src/fruitapp/states/menu/settings.cpp
	src/fruitapp/states/menu/superstate.cpp
	src/fruitapp/sword_trail.cpp
	src/fruitapp/viewport/manager.cpp
)

fcppt_utils_add_source_groups(
  "${APP_FILES}")

# library begin
add_library(
	fruitlib STATIC ${FRUITLIB_FILES})

target_link_libraries(
	fruitlib
	${Fcppt_core_LIBRARIES}
	${Fcppt_filesystem_LIBRARIES}
	${CEGUI_LIBRARIES}
	${SGE_LIBRARIES}
	${Cg_LIBRARIES}
	${BULLET_LIBRARIES}
	${Boost_LIBRARIES})
# library end

# app begin
awl_utils_add_portable_executable(
	fruitcut
	"fruitapp::main"
	${APP_FILES})

target_link_libraries(
	fruitcut
	fruitlib

	${SGE_LIBRARIES}
	${Cg_LIBRARIES}

	${FcpptThreads_LIBRARIES}

	${Fcppt_core_LIBRARIES}
	${Fcppt_filesystem_LIBRARIES}
	${Awl_LIBRARIES}
	${CEGUI_LIBRARIES}
	${BULLET_LIBRARIES}
	${Boost_LIBRARIES})

install(
	TARGETS
	fruitcut
	DESTINATION
	${INSTALL_BINARY_DIR}
)
# app end

# server begin
option(ENABLE_SERVER "Build the server (beware: it's POSIX only!)" OFF)

# this has to be defined here so update_cmake recognizes it
set(
	SERVER_FILES
	src/fruitserver/ascii/from_ascii_char.cpp
	src/fruitserver/ascii/from_byte_sequence.cpp
	src/fruitserver/ascii/from_native.cpp
	src/fruitserver/ascii/is_digit.cpp
	src/fruitserver/ascii/is_letter.cpp
	src/fruitserver/ascii/to_byte_sequence.cpp
	src/fruitserver/ascii/to_native.cpp
	src/fruitserver/ascii/to_native_char.cpp
	src/fruitserver/ascii/to_native_translation.cpp
	src/fruitserver/command_processor.cpp
	src/fruitserver/controller.cpp
	src/fruitserver/current_datetime.cpp
	src/fruitserver/generate_datetime.cpp
	src/fruitserver/highscore_entry.cpp
	src/fruitserver/highscore_from_plain_file.cpp
	src/fruitserver/highscore_to_json.cpp
	src/fruitserver/highscore_to_plain_file.cpp
	src/fruitserver/listener/base.cpp
	src/fruitserver/listener/posix_select.cpp
	src/fruitserver/output_tm.cpp
	src/fruitserver/parse_command.cpp
	src/fruitserver/program_options/detail/bad_cast.cpp
	src/fruitserver/program_options/detail/option_base.cpp
	src/fruitserver/program_options/detail/option_holder_base.cpp
	src/fruitserver/program_options/detail/type_info.cpp
	src/fruitserver/program_options/help_was_needed.cpp
	src/fruitserver/program_options/make_command_line_parameters.cpp
	src/fruitserver/program_options/object.cpp
	src/fruitserver/program_options/option_sequence.cpp
	src/fruitserver/separator.cpp
	src/fruitserver/server.cpp
	src/fruitserver/std_error_string.cpp
)

if(ENABLE_SERVER)

	fcppt_utils_add_source_groups("${SERVER_FILES}")

	add_executable(
		fruitcut_server
		${SERVER_FILES})

	install(
		TARGETS
		fruitcut_server
		DESTINATION
		${INSTALL_BINARY_DIR}
	)
endif()
# server end
