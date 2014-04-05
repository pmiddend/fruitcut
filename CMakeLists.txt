cmake_minimum_required(
  VERSION
  2.8.12)

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

find_package(
	fcppt REQUIRED)

find_package(
	awl REQUIRED)

find_package(
	mizuiro REQUIRED)

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

OPTION(
  USE_CEGUI
  "Use CEGUI for the gui instead of the dummy console gui (recommended)"
  ON)

set(
	sge_find_package_use_cegui
	"")

if(USE_CEGUI)
	set(
		sge_find_package_use_cegui
		"cegui")
endif()

FIND_PACKAGE(
	sge REQUIRED
	audio
	camera
	charconv
	${sge_find_package_use_cegui}
	cg
	config
	console
	consolegfx
	core
	fontdraw
	fontbitmap
	font
	imagecolor
	image2d
	input
	line_drawer
	log
	modelmd3
	parse
	parsejson
	renderer
	shader
	sprite
	systems
	texture
	viewport
	window
)

# cegui begin
if(USE_CEGUI)
	find_package(
		CEGUI REQUIRED)

	link_directories(
		"${CEGUI_LIBRARY_DIRS}")

	include_directories(
		${CEGUI_INCLUDE_DIRS})
endif()
# cegui end

# get rid of a pretty useless warning
if(CMAKE_COMPILER_IS_GNUCXX OR FCPPT_UTILS_COMPILER_IS_CLANGPP)
	add_definitions(-Wno-old-style-cast)
elseif(MSVC)
	# Disable "this" used in initializer list warning
	add_definitions("/wd4355")
endif()

add_subdirectory(
  fruitlib)

add_subdirectory(
  fruitapp)

#option(
#  ENABLE_TEST "Build the tests" ON)

#if(ENABLE_TEST)
#	enable_testing()
#	add_subdirectory(
#		src/test)
#endif()

#add_subdirectory(
#	src/sandbox)
# app end

# server begin
option(
  ENABLE_SERVER
  "Build the server (beware: it's POSIX only!)"
  OFF)

# this has to be defined here so update_cmake recognizes it
if(ENABLE_SERVER)
  add_subdirectory(
	fruitserver)
endif()
