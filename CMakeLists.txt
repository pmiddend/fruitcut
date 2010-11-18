CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

PROJECT(fruitcut)

# This allows ENDIFs with no specifier _what_ ends
SET (CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

# So we can use FindFcppt.cmake, among others
SET (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMakeModules)

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
SET(Boost_ADDITIONAL_VERSIONS "1.41" "1.41.0" "1.42" "1.42.0" "1.43" "1.43.0" "1.44" "1.44.0")

FIND_PACKAGE(
	Boost 1.41.0 REQUIRED COMPONENTS
	system)

INCLUDE_DIRECTORIES(SYSTEM ${Boost_INCLUDE_DIR})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
# boost end

# fcppt begin
FIND_PACKAGE(
	Fcppt REQUIRED)

INCLUDE_DIRECTORIES(${FCPPT_INCLUDE_DIRS})
LINK_DIRECTORIES(${FCPPT_LIBRARY_DIRS})
# fcppt end

# sge begin
FIND_PACKAGE(
	SGE REQUIRED)

INCLUDE_DIRECTORIES(${SGE_INCLUDE_DIRS})

ADD_DEFINITIONS(${SGE_CFLAGS})
# sge end

IF (${CMAKE_COMPILER_IS_GNUCXX})
	ADD_DEFINITIONS("
		-fvisibility=hidden -ansi -pedantic-errors -Wall"
		"-Wextra -Wconversion -Wfloat-equal -Wredundant-decls"
		"-Wstrict-null-sentinel -Winit-self -Woverloaded-virtual"
		"-Wold-style-cast -Wnon-virtual-dtor -Wctor-dtor-privacy -Wsign-promo"
		"-Wstrict-aliasing=1 -Wmissing-declarations -Wno-long-long"
		"-std=c++0x")
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
	media_path.cpp mesh_to_vertex_buffer.cpp model_to_mesh.cpp machine.cpp cut_mesh.cpp
	# input
	input/state.cpp input/state_manager.cpp input/state_connection.cpp
	# json
	json/config_wrapper.cpp json/merge_trees.cpp json/process_option.cpp json/string_to_value.cpp
	# camera
	graphics/camera/projection/to_matrix.cpp graphics/camera/projection/perspective.cpp graphics/camera/object.cpp graphics/camera/parameters.cpp
	# events
	events/tick.cpp
	# states
	states/ingame.cpp states/freelook.cpp states/cut.cpp)

ADD_EXECUTABLE(
	cut_test
	cut_test.cpp
	${files})

TARGET_LINK_LIBRARIES(
	cut_test
	${FCPPT_LIBRARIES}
	${SGE_CORE_LIBRARY}
	${SGE_SHADER_LIBRARY}
	${SGE_CONSOLE_LIBRARY}
	${Boost_LIBRARIES})

# TODO
#INSTALL (DIRECTORY media/ DESTINATION share/games/sanguis/media/)
#INSTALL (TARGETS sanguis DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/)
