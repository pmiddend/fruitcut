CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(fruitcut)

INCLUDE(FcpptCMakeUtils)

FIND_PACKAGE(
	Boost 1.41.0 REQUIRED COMPONENTS
	system
	date_time)

INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_SOURCE_DIR}/geometry)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
ADD_DEFINITIONS(-D BOOST_ASIO_SEPARATE_COMPILATION)
# boost end

# fcppt begin
FIND_PACKAGE(
	Fcppt REQUIRED)

INCLUDE_DIRECTORIES(SYSTEM ${FCPPT_INCLUDE_DIRS})
LINK_DIRECTORIES(${FCPPT_LIBRARY_DIRS})
# fcppt end

# bullet begin
FIND_PACKAGE(Bullet REQUIRED)
INCLUDE_DIRECTORIES(SYSTEM ${BULLET_INCLUDE_DIRS})
# bullet end

# sge begin
FIND_PACKAGE(
	SGE REQUIRED COMPONENTS console sprite shader config time texture systems camera fonttext fontbitmap cegui)

INCLUDE_DIRECTORIES(SYSTEM ${SGE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${AWL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${MIZUIRO_INCLUDE_DIRS})

ADD_DEFINITIONS(${SGE_CFLAGS})
# sge end

# TODO
SET(
	media_path
	${CMAKE_INSTALL_PREFIX}/share/games/${PROJECT_NAME})

# TODO
SET_SOURCE_FILES_PROPERTIES(
	media_path.cpp
	PROPERTIES
	COMPILE_FLAGS
	-DMEDIA_PATH="\\\"${media_path}\\\""
)

SET_SOURCE_FILES_PROPERTIES(
	media_path.cpp
	PROPERTIES
	COMPILE_FLAGS
	-DMEDIA_PATH="\\\"${CMAKE_INSTALL_PREFIX}/share/games/${PROJECT_NAME}/media\\\""
)

ENABLE_TESTING()
ADD_SUBDIRECTORY(test)

SET(files 
	# base 
	media_path.cpp 
	# environment
	environment/make_cache_path.cpp
	environment/make_config_path.cpp
	environment/getenv.cpp
	# json
	json/parse_rgba8_color.cpp 
	json/merge_trees.cpp 
	json/process_option.cpp 
	json/string_to_value.cpp
	json/is_null.cpp
	# log
	log/scoped.cpp
	log/string_to_location.cpp
	log/scoped_sequence_from_json.cpp
	# input 
	input/state.cpp input/state_manager.cpp
	audio/sound_controller.cpp
	audio/music_controller.cpp
	audio/detail/sound_group.cpp
	cursor_trail.cpp
	# pp 
	pp/filter/base.cpp 
	pp/filter/wrapper.cpp
	pp/filter/render_to_texture.cpp
	pp/filter/highlight.cpp
	pp/filter/desaturate.cpp
	pp/filter/inject_texture.cpp
	pp/filter/add.cpp
	pp/system.cpp
	pp/texture/descriptor.cpp
	pp/texture/filter_wrapper.cpp
	pp/texture/manager.cpp
	pp/texture/instance.cpp
	pp/texture/use_screen_size.cpp 
	pp/filter/blur.cpp
	pp/filter/ssaa.cpp
	pp/screen_vf/quad.cpp
	# random
	create_rng.cpp
	# font
	font/system.cpp
	font/cache.cpp
	font/scoped_transformation.cpp
	font/drawer.cpp
	font/particle/base.cpp
	font/particle/animated.cpp
	font/particle/base_parameters.cpp
	font/damped_oscillation.cpp
	# time format
	time_format/seconds.cpp
	time_format/milliseconds.cpp
	# particle
	particle/system.cpp
	particle/objects/base.cpp
	particle/objects/simple.cpp
	particle/objects/permanent_sprite.cpp
	# line drawer
	line_drawer/object.cpp
	line_drawer/scoped_lock.cpp
	line_drawer/line.cpp
	# physics
	physics/world.cpp
	physics/debugger.cpp
	physics/null_collision_filter.cpp
	physics/rigid_body/parameters.cpp
	physics/rigid_body/scoped_body.cpp
	physics/rigid_body/object.cpp)

ADD_LIBRARY(
	fruitcut_library STATIC ${files})

ADD_EXECUTABLE(
	app
	app/background.cpp
	app/game_logic.cpp
	app/events/tick.cpp
	app/config_wrapper.cpp
	app/fruitcut.cpp
	app/fruit/cut_mesh.cpp
	app/fruit/hull/projected.cpp
	app/fruit/hull/trail_intersection.cpp
	app/fruit/manager.cpp
	app/fruit/spawner.cpp
	app/fruit/mesh_to_point_cloud.cpp
	app/fruit/mesh_to_shape.cpp
	app/fruit/mesh_to_vertex_buffer.cpp
	app/fruit/model_to_mesh.cpp
	app/fruit/object.cpp
	app/fruit/prototype.cpp
	app/machine.cpp 
	app/name.cpp
	app/postprocessing.cpp 
	app/states/ingame/superstate.cpp
	app/states/ingame/running.cpp
	app/states/ingame/paused.cpp
	app/states/gameover/superstate.cpp
	app/states/gameover/choose_name.cpp
	app/states/gameover/highscore.cpp
	app/states/intro.cpp)

TARGET_LINK_LIBRARIES(
	app
	fruitcut_library
	${FCPPT_LIBRARIES}
	${SGE_CORE_LIBRARY}
	${SGE_shader_LIBRARY}
	${SGE_cegui_LIBRARY}
	${SGE_fonttext_LIBRARY}
	${SGE_fontbitmap_LIBRARY}
	${SGE_console_LIBRARY}
	${SGE_systems_LIBRARY}
	${SGE_camera_LIBRARY}
	${SGE_parse_LIBRARY}
	${SGE_sprite_LIBRARY}
	${BULLET_LIBRARIES}
	${Boost_LIBRARIES})

ADD_SUBDIRECTORY(
	sandbox)
