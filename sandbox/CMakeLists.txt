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
	fruitlib
	${Fcppt_LIBRARIES}
	${SGE_LIBRARIES}
	${Boost_LIBRARIES})

ADD_EXECUTABLE(
	fruitcut_example_parse_url
	parse_url/main.cpp)

TARGET_LINK_LIBRARIES(
	fruitcut_example_parse_url
	${Fcppt_LIBRARIES}
	${Boost_LIBRARIES})
