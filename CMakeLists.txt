CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(fruitcut)

INCLUDE(FcpptCMakeUtils)
INCLUDE(AwlMainGenerator)

FIND_PACKAGE(
	Boost 1.47.0 REQUIRED COMPONENTS
	chrono
	date_time
	system)

IF(
	MSVC AND ${Boost_VERSION} STREQUAL "104800"
)
	ADD_DEFINITIONS(
		-D BOOST_FUSION_DONT_USE_PREPROCESSED_FILES
	)
ENDIF()

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
# boost end

# fcppt begin
FIND_PACKAGE(Fcppt REQUIRED)
INCLUDE_DIRECTORIES(${Fcppt_INCLUDE_DIRS})
ADD_DEFINITIONS(${Fcppt_DEFINITIONS})
# fcppt end

# awl begin
FIND_PACKAGE(Awl REQUIRED)
INCLUDE_DIRECTORIES(${Awl_INCLUDE_DIR})
ADD_DEFINITIONS(${Awl_DEFINITIONS})
# awl end

# mizuiro begin
FIND_PACKAGE(Mizuiro REQUIRED)
INCLUDE_DIRECTORIES(${Mizuiro_INCLUDE_DIR})
ADD_DEFINITIONS(${Mizuiro_DEFINITIONS})
# mizuiro end

# bullet begin
FIND_PACKAGE(Bullet REQUIRED)
INCLUDE_DIRECTORIES(SYSTEM ${BULLET_INCLUDE_DIRS})
# bullet end


# sge begin
FIND_PACKAGE(
	SGE REQUIRED COMPONENTS
	audio
	camera
	cegui
	charconv
	config
	console
	core
	fontbitmap
	fonttext
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

INCLUDE_DIRECTORIES(${SGE_INCLUDE_DIRS})
ADD_DEFINITIONS(${SGE_DEFINITIONS})
# sge end

# cegui begin
FIND_PACKAGE(
	CEGUI REQUIRED)

INCLUDE_DIRECTORIES(SYSTEM ${CEGUI_INCLUDE_DIRS})
# cegui end

# get rid of a pretty useless warning
IF(CMAKE_COMPILER_IS_GNUCXX OR FCPPT_UTILS_COMPILER_IS_CLANGPP)
	ADD_DEFINITIONS(-Wno-old-style-cast)
ELSEIF(MSVC)
	# Disable "this" used in initializer list warning
	ADD_DEFINITIONS("/wd4355")
ENDIF()

SET(
	INSTALL_MEDIA_DIR
	${INSTALL_DATA_DIR}/media
)

INCLUDE(
	SGECustomPath
)

SGE_CONFIG_ADD_CUSTOM_PATH(
	fruitcut
	media
	${CMAKE_SOURCE_DIR}/media
	${INSTALL_MEDIA_DIR}
)

INSTALL(
	DIRECTORY
	${CMAKE_SOURCE_DIR}/media/
	DESTINATION
	${INSTALL_MEDIA_DIR}
)

OPTION(ENABLE_TEST "Build the tests" ON)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

IF(ENABLE_TEST)
	ENABLE_TESTING()
	ADD_SUBDIRECTORY(src/test)
ENDIF()

ADD_SUBDIRECTORY(src/sandbox)

SET(
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
	src/fruitlib/font/cache.cpp
	src/fruitlib/font/drawer/object.cpp
	src/fruitlib/font/drawer/parameters.cpp
	src/fruitlib/font/drawer/scoped_color.cpp
	src/fruitlib/font/drawer/scoped_transformation.cpp
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
	src/fruitlib/scenic/events/viewport_change.cpp
	src/fruitlib/scenic/no_parent.cpp
	src/fruitlib/scenic/parent.cpp
	src/fruitlib/scoped_frame_limiter.cpp
	src/fruitlib/time_format/milliseconds.cpp
	src/fruitlib/time_format/minutes.cpp
	src/fruitlib/time_format/seconds.cpp
	src/fruitlib/utf8_file_to_fcppt_string.cpp
)

FCPPT_UTILS_ADD_SOURCE_GROUPS("${FRUITLIB_FILES}")

SET(
	APP_FILES
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
	src/fruitapp/fruit/mesh_to_point_cloud.cpp
	src/fruitapp/fruit/mesh_to_shape.cpp
	src/fruitapp/fruit/mesh_to_vertex_buffer.cpp
	src/fruitapp/fruit/model_to_mesh.cpp
	src/fruitapp/fruit/object.cpp
	src/fruitapp/fruit/object_parameters.cpp
	src/fruitapp/fruit/parameters_from_prototype.cpp
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
)

FCPPT_UTILS_ADD_SOURCE_GROUPS("${APP_FILES}")

# library begin
ADD_LIBRARY(
	fruitlib STATIC ${FRUITLIB_FILES})

TARGET_LINK_LIBRARIES(
	fruitlib
	${Fcppt_core_LIBRARIES}
	${Fcppt_filesystem_LIBRARIES}
	${CEGUI_LIBRARIES}
	${SGE_LIBRARIES}
	${BULLET_LIBRARIES}
	${Boost_LIBRARIES})
# library end

# app begin
awl_utils_add_portable_executable(
	fruitcut
	"fruitapp::main"
	${APP_FILES})

TARGET_LINK_LIBRARIES(
	fruitcut
	fruitlib
	${Fcppt_core_LIBRARIES}
	${Fcppt_filesystem_LIBRARIES}
	${Awl_LIBRARIES}
	${SGE_LIBRARIES}
	${CEGUI_LIBRARIES}
	${BULLET_LIBRARIES}
	${Boost_LIBRARIES})

INSTALL(
	TARGETS
	fruitcut
	DESTINATION
	${INSTALL_BINARY_DIR}
)
# app end

# server begin
OPTION(ENABLE_SERVER "Build the server (beware: it's POSIX only!)" OFF)

# this has to be defined here so update_cmake recognizes it
SET(
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

IF(ENABLE_SERVER)

	FCPPT_UTILS_ADD_SOURCE_GROUPS("${SERVER_FILES}")

	ADD_EXECUTABLE(
		fruitcut_server
		${SERVER_FILES})

	INSTALL(
		TARGETS
		fruitcut_server
		DESTINATION
		${INSTALL_BINARY_DIR}
	)
ENDIF()
# server end
