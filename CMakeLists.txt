CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(fruitcut)

INCLUDE(FcpptCMakeUtils)

FIND_PACKAGE(
	Boost 1.45.0 REQUIRED COMPONENTS
	system
	date_time)

INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(SYSTEM ${CMAKE_SOURCE_DIR}/geometry)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
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
	SGE REQUIRED COMPONENTS viewport console sprite shader config time texture systems camera fonttext fontbitmap cegui line_drawer)

INCLUDE_DIRECTORIES(SYSTEM ${SGE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${AWL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(SYSTEM ${MIZUIRO_INCLUDE_DIRS})

ADD_DEFINITIONS(${SGE_CFLAGS})
# sge end

# cegui begin
FIND_PACKAGE(
	CEGUI REQUIRED)

INCLUDE_DIRECTORIES(${CEGUI_INCLUDE_DIRS})
# cegui end

# TODO
SET(
	media_path
	${CMAKE_INSTALL_PREFIX}/share/games/${PROJECT_NAME})

# TODO
SET_SOURCE_FILES_PROPERTIES(
	src/media_path.cpp
	PROPERTIES
	COMPILE_FLAGS
	-DMEDIA_PATH="\\\"${media_path}\\\""
)

SET_SOURCE_FILES_PROPERTIES(
	src/media_path.cpp
	PROPERTIES
	COMPILE_FLAGS
	-DMEDIA_PATH="\\\"${CMAKE_INSTALL_PREFIX}/share/games/${PROJECT_NAME}/media\\\""
)

ENABLE_TESTING()
ADD_SUBDIRECTORY(src/test)

SET(
	files 
	# base 
	src/media_path.cpp 
	src/resource_tree/path.cpp 
	src/exception.cpp
	src/average_colors.cpp
	# json
	src/json/parse_rgba8_color.cpp 
	src/json/merge_trees.cpp 
	src/json/process_option.cpp 
	src/json/string_to_value.cpp
	src/json/is_null.cpp
	# log
	src/log/scoped.cpp
	src/log/string_to_location.cpp
	src/log/scoped_sequence_from_json.cpp
	# input 
	src/input/state.cpp 
	src/input/state_manager.cpp
	src/audio/sound_controller.cpp
	src/audio/music_controller.cpp
	src/cursor_trail.cpp
	# pp 
	src/pp/filter/base.cpp 
	src/pp/filter/wrapper.cpp
	src/pp/filter/render_to_texture.cpp
	src/pp/filter/highlight.cpp
	src/pp/filter/desaturate.cpp
	src/pp/filter/inject_texture.cpp
	src/pp/filter/add.cpp
	src/pp/system.cpp
	src/pp/texture/descriptor.cpp
	src/pp/texture/filter_wrapper.cpp
	src/pp/texture/manager.cpp
	src/pp/texture/instance.cpp
	src/pp/texture/use_screen_size.cpp 
	src/pp/filter/blur.cpp
	src/pp/filter/ssaa.cpp
	src/pp/screen_vf/quad.cpp
	# random
	src/create_rng.cpp
	# font
	src/font/intrusive_scene_node.cpp
	src/font/cache.cpp
	src/font/object_parameters.cpp
	src/font/scene_node.cpp
	src/font/object.cpp
	src/font/drawer/object.cpp
	src/font/drawer/scoped_color.cpp
	src/font/drawer/scoped_transformation.cpp
	src/font/drawer/parameters.cpp
#	src/font/damped_oscillation.cpp
	# time format
	src/time_format/seconds.cpp
	src/time_format/milliseconds.cpp
	# particle
	src/particle/system.cpp
	src/particle/objects/base.cpp
	src/particle/objects/simple.cpp
	src/particle/objects/permanent_sprite.cpp
	# scenic
	src/scenic/exception.cpp
	src/scenic/nodes/base.cpp
	src/scenic/nodes/camera.cpp
	src/scenic/nodes/intrusive.cpp
	src/scenic/nodes/console.cpp
	src/scenic/nodes/cursor_trail.cpp
	src/scenic/nodes/group.cpp
	src/scenic/nodes/gui_system.cpp
	src/scenic/nodes/immortal.cpp
	src/scenic/nodes/intrusive_group.cpp
	src/scenic/nodes/intrusive_with_callbacks.cpp
	src/scenic/nodes/line_drawer.cpp
	src/scenic/nodes/music_controller.cpp
	src/scenic/nodes/sound_controller.cpp
	src/scenic/nodes/with_color.cpp
	src/scenic/nodes/with_lifetime.cpp
	src/scenic/nodes/with_scale.cpp
	# physics
	src/physics/world.cpp
	src/physics/debugger.cpp
	src/physics/null_collision_filter.cpp
	src/physics/rigid_body/parameters.cpp
	src/physics/nodes/debugger.cpp
	src/physics/nodes/world.cpp
	src/physics/rigid_body/scoped_body.cpp
	src/physics/rigid_body/object.cpp)

ADD_LIBRARY(
	fruitcut_library STATIC ${files})

ADD_EXECUTABLE(
	fruitcut
	src/app/scene.cpp
	src/app/logo.cpp
	src/app/scoped_scene_activation.cpp
	src/app/background.cpp
	src/app/game_logic.cpp
	src/app/events/tick.cpp
	src/app/config_wrapper.cpp
	src/app/fruit/cut_context.cpp
	src/app/splatter_generator.cpp
	src/app/fruitcut.cpp
	src/app/fruit/prototype_from_json.cpp
	src/app/fruit/cut_mesh.cpp
	src/app/fruit/object_parameters.cpp
	src/app/fruit/parameters_from_prototype.cpp
	src/app/fruit/cut_mesh_result.cpp
	src/app/fruit/hull/projected.cpp
	src/app/fruit/hull/trail_intersection.cpp
	src/app/fruit/manager.cpp
	src/app/fruit/spawner.cpp
	src/app/fruit/mesh_to_point_cloud.cpp
	src/app/fruit/mesh_to_shape.cpp
	src/app/fruit/mesh_to_vertex_buffer.cpp
	src/app/fruit/model_to_mesh.cpp
	src/app/fruit/object.cpp
	src/app/fruit/prototype.cpp
	src/app/machine.cpp 
	src/app/name.cpp
	src/app/point_sprite/system_node.cpp
	src/app/point_sprite/base.cpp
	src/app/point_sprite/splatter/parameters.cpp
	src/app/point_sprite/splatter/object.cpp
	src/app/postprocessing.cpp 
	src/app/states/ingame/superstate.cpp
	src/app/states/ingame/running.cpp
	src/app/states/ingame/paused.cpp
	src/app/states/gameover/superstate.cpp
	src/app/states/gameover/choose_name.cpp
	src/app/states/gameover/highscore.cpp
	src/app/states/intro.cpp)

TARGET_LINK_LIBRARIES(
	fruitcut
	fruitcut_library
	${FCPPT_LIBRARIES}
	${SGE_LIBRARIES}
	${BULLET_LIBRARIES}
	${Boost_LIBRARIES})

ADD_SUBDIRECTORY(
	src/sandbox)
