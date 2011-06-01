# TODO
SET_SOURCE_FILES_PROPERTIES(
	../media_path.cpp
	PROPERTIES
	COMPILE_FLAGS
	-DMEDIA_PATH="\\\"${media_path}\\\""
)

SET_SOURCE_FILES_PROPERTIES(
	../media_path.cpp
	PROPERTIES
	COMPILE_FLAGS
	-DMEDIA_PATH="\\\"${CMAKE_INSTALL_PREFIX}/share/games/${PROJECT_NAME}/media\\\""
)

ADD_EXECUTABLE(
	example_trail
	trail/trail_test.cpp trail/mouse_trailer.cpp trail/sword_particle.cpp ../media_path.cpp)

TARGET_LINK_LIBRARIES(
	example_trail
	fruitcut_library
	${FCPPT_LIBRARIES}
	${SGE_LIBRARIES}
	${Boost_LIBRARIES})

ADD_EXECUTABLE(
	example_accumulate_colors
	accumulate_colors/accumulate_colors.cpp)

TARGET_LINK_LIBRARIES(
	example_accumulate_colors
	${FCPPT_LIBRARIES}
	${SGE_LIBRARIES}
	${Boost_LIBRARIES})

ADD_EXECUTABLE(
	example_duration_string
	duration_string/duration_string.cpp)

TARGET_LINK_LIBRARIES(
	example_duration_string
	${FCPPT_LIBRARIES}
	${Boost_LIBRARIES})

ADD_EXECUTABLE(
	example_splat
	splat/splat.cpp splat/splat_collector.cpp splat/splatter.cpp splat/splat_particle.cpp ../media_path.cpp)

TARGET_LINK_LIBRARIES(
	example_splat
	fruitcut_library
	${FCPPT_LIBRARIES}
	${SGE_LIBRARIES}
	${Boost_LIBRARIES})

ADD_EXECUTABLE(
	example_convolution
	convolution/convolution.cpp ../media_path.cpp)

TARGET_LINK_LIBRARIES(
	example_convolution
	fruitcut_library
	${FCPPT_LIBRARIES}
	${SGE_LIBRARIES}
	${Boost_LIBRARIES})

ADD_EXECUTABLE(
	example_geometry
	geometry/geometry.cpp)

TARGET_LINK_LIBRARIES(
	example_geometry
	${FCPPT_LIBRARIES}
	${Boost_LIBRARIES})
