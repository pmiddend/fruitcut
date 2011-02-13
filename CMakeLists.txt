CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(fruitcut)

# This allows ENDIFs with no specifier _what_ ends
SET (CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

SET(EXTRA_CMAKE_MODULE_PATH "" CACHE PATH "Extra paths to search for cmake modules")
SET(CMAKE_MODULE_PATH "${EXTRA_CMAKE_MODULE_PATH}")

IF(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
	MESSAGE(
		FATAL_ERROR
		"In-source builds are not permitted. "
		"Make a separate folder for building:\n"
		"mkdir build; cd build; cmake ..\n"
		"Before that, remove the files that cmake"
		"just created:\nrm -rf CMakeCache.txt CMakeFiles")
ENDIF()

# boost begin
SET(Boost_ADDITIONAL_VERSIONS "1.41" "1.41.0" "1.42" "1.42.0" "1.43" "1.43.0" "1.44" "1.44.0" "1.45.0")

FIND_PACKAGE(
	Boost 1.41.0 REQUIRED COMPONENTS
	system)

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
	SGE REQUIRED COMPONENTS console sprite shader config time texture systems camera fonttext fontbitmap)

INCLUDE_DIRECTORIES(${SGE_INCLUDE_DIRS})

ADD_DEFINITIONS(${SGE_CFLAGS})
# sge end

# clang check begin
INCLUDE (CheckCXXSourceRuns)

CHECK_CXX_SOURCE_RUNS(
	"#include <cstdlib>
	int main()
	{
	#ifdef __clang__
		return EXIT_SUCCESS;
	#else
		return EXIT_FAILURE;
	#endif
	}
	"
	COMPILER_IS_CLANGPP)
# clang check end

IF (CMAKE_COMPILER_IS_GNUCXX OR COMPILER_IS_CLANGPP)
	ADD_DEFINITIONS("
		-fvisibility=hidden -ansi -pedantic-errors -Wall"
		"-Wextra -Wconversion -Wfloat-equal -Wredundant-decls"
		"-Winit-self -Woverloaded-virtual"
		"-Wshadow"
		"-Wold-style-cast -Wnon-virtual-dtor -Wctor-dtor-privacy -Wsign-promo"
		"-Wstrict-aliasing=1 -Wmissing-declarations -Wno-long-long")
	# we don't use long long but boost does :(

	ADD_DEFINITIONS(-D BOOST_FILESYSTEM_NO_DEPRECATED)
ENDIF ()

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
	# json
	json/config_wrapper.cpp json/merge_trees.cpp json/process_option.cpp json/string_to_value.cpp
	# input 
	input/state.cpp input/state_manager.cpp
	sound_controller.cpp
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
	pp/filter/blur.cpp
	pp/filter/ssaa.cpp
	pp/screen_vf/create_quad.cpp
	# particle
	particle/system.cpp
	particle/objects/base.cpp
	particle/objects/simple.cpp
	particle/objects/permanent_sprite.cpp
	particle/sprite/render_flipped.cpp
	particle/point_sprite/render_flipped.cpp
	# line drawer
	line_drawer/object.cpp
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
	app/fruitcut.cpp
	app/fruit/prototype.cpp
	app/fruit/object.cpp
	app/machine.cpp 
	app/postprocessing.cpp 
	app/name.cpp
	app/fruit/hull/projected.cpp
	app/cut_mesh.cpp
	app/fruit/hull/trail_intersection.cpp
	app/fruit/manager.cpp
	app/events/tick.cpp
	app/mesh_to_shape.cpp
	app/model_to_mesh.cpp
	app/asio.cpp
	app/mesh_to_vertex_buffer.cpp
	app/states/intro.cpp
	app/states/ingame.cpp
	app/states/paused.cpp
	app/states/running.cpp)

TARGET_LINK_LIBRARIES(
	app
	fruitcut_library
	${FCPPT_LIBRARIES}
	${SGE_CORE_LIBRARY}
	${SGE_shader_LIBRARY}
	${SGE_console_LIBRARY}
	${SGE_systems_LIBRARY}
	${SGE_camera_LIBRARY}
	${SGE_parse_LIBRARY}
	${SGE_sprite_LIBRARY}
	${BULLET_LIBRARIES}
	${Boost_LIBRARIES})

ADD_SUBDIRECTORY(
	sandbox)
