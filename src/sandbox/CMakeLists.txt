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
	fruitcut_example_duration_string
	duration_string/duration_string.cpp)

TARGET_LINK_LIBRARIES(
	fruitcut_example_duration_string
	${Fcppt_LIBRARIES}
	${Boost_LIBRARIES})

ADD_EXECUTABLE(
	fruitcut_example_geometry
	geometry/geometry.cpp)

TARGET_LINK_LIBRARIES(
	fruitcut_example_geometry
	${Fcppt_LIBRARIES}
	${Boost_LIBRARIES})

ADD_EXECUTABLE(
	fruitcut_example_json_output
	json_output/json_output.cpp)

TARGET_LINK_LIBRARIES(
	fruitcut_example_json_output
	fruitcut_library
	${Fcppt_LIBRARIES}
	${SGE_LIBRARIES}
	${Boost_LIBRARIES})
