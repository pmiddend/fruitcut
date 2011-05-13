ADD_EXECUTABLE(
	trail_test
	trail_test.cpp mouse_trailer.cpp sword_particle.cpp)

TARGET_LINK_LIBRARIES(
	trail_test
	fruitcut_library
	${FCPPT_LIBRARIES}
	${SGE_LIBRARIES}
	${Boost_LIBRARIES})

ADD_EXECUTABLE(
	duration_string
	duration_string.cpp)

TARGET_LINK_LIBRARIES(
	duration_string
	${FCPPT_LIBRARIES}
	${Boost_LIBRARIES})

#ADD_EXECUTABLE(
#	font_test
#	font.cpp)

#TARGET_LINK_LIBRARIES(
#	font_test
#	fruitcut_library
#	${FCPPT_LIBRARIES}
#	${SGE_LIBRARIES}
#	${Boost_LIBRARIES})

ADD_EXECUTABLE(
	splat_test
	splat.cpp splat_collector.cpp splatter.cpp splat_particle.cpp)

TARGET_LINK_LIBRARIES(
	splat_test
	fruitcut_library
	${FCPPT_LIBRARIES}
	${SGE_LIBRARIES}
	${Boost_LIBRARIES})

ADD_EXECUTABLE(
	convolution_test
	convolution.cpp)

TARGET_LINK_LIBRARIES(
	convolution_test
	fruitcut_library
	${FCPPT_LIBRARIES}
	${SGE_LIBRARIES}
	${Boost_LIBRARIES})

ADD_EXECUTABLE(
	geometry
	geometry.cpp)

TARGET_LINK_LIBRARIES(
	geometry
	${FCPPT_LIBRARIES}
	${Boost_LIBRARIES})
