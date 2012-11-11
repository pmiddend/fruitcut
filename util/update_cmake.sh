#!/bin/bash

function die()
{
	exit -1
}

function update_cmake_file()
{
	local cmakefile="$1"

	update_cmake \
		"${cmakefile}" \
		"${@:2}" \
		|| die

	mv "${cmakefile}".new "${cmakefile}" || die
}

update_cmake_file \
	fruitlib/CMakeLists.txt \
	FRUITLIB_FILES \
	fruitlib/src \
	fruitlib/include

update_cmake_file \
	fruitapp/CMakeLists.txt \
	FRUITAPP_FILES \
	-n \
	fruitapp/src \
	fruitapp/src/gui \
	fruitapp/include/fruitapp \
	fruitapp/include/fruitapp/gui \
	-r \
	fruitapp/src/bonsu \
	fruitapp/src/depths \
	fruitapp/src/background \
	fruitapp/src/light \
	fruitapp/src/cursor \
	fruitapp/src/fruit \
	fruitapp/src/shadow_map \
	fruitapp/src/game_logic \
	fruitapp/src/highscore \
	fruitapp/src/point_sprite \
	fruitapp/src/projection_manager \
	fruitapp/src/postprocessing \
	fruitapp/src/states \
	fruitapp/src/viewport \
	fruitapp/src/gui/dialogs \
	fruitapp/src/gui/dummy \
	fruitapp/include/fruitapp/bonsu \
	fruitapp/include/fruitapp/depths \
	fruitapp/include/fruitapp/fruit \
	fruitapp/include/fruitapp/background \
	fruitapp/include/fruitapp/game_logic \
	fruitapp/include/fruitapp/shadow_map \
	fruitapp/include/fruitapp/postprocessing \
	fruitapp/include/fruitapp/highscore \
	fruitapp/include/fruitapp/point_sprite \
	fruitapp/include/fruitapp/projection_manager \
	fruitapp/include/fruitapp/states \
	fruitapp/include/fruitapp/viewport \
	fruitapp/include/fruitapp/gui/dialogs \
	fruitapp/include/fruitapp/gui/dummy

update_cmake_file \
	fruitapp/CMakeLists.txt \
	FRUITAPP_CEGUI_FILES \
	fruitapp/src/gui/ce \
	fruitapp/include/fruitapp/gui/ce

update_cmake_file \
	fruitserver/CMakeLists.txt \
	FRUITSERVER_FILES \
	fruitserver/src \
	fruitserver/include
