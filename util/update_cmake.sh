#!/bin/sh

set -e -u

fruitlib_dir() {
	echo "fruitlib/src/$1" "fruitlib/include/fruitlib/$1"
}

update_cmake \
	fruitlib/CMakeLists.txt \
	FRUITLIB_FILES \
	-n \
	fruitlib/src \
	fruitlib/include/fruitlib \
	$(fruitlib_dir scenic) \
	$(fruitlib_dir scenic/adaptors) \
	-r \
	$(fruitlib_dir audio) \
	$(fruitlib_dir font) \
	$(fruitlib_dir json) \
	$(fruitlib_dir log) \
	$(fruitlib_dir math) \
	$(fruitlib_dir physics) \
	$(fruitlib_dir pp) \
	$(fruitlib_dir resource_tree) \
	$(fruitlib_dir scenic/delta) \
	$(fruitlib_dir scenic/events) \
	$(fruitlib_dir signal_stack_printer) \
	$(fruitlib_dir time_format) \
	fruitlib/include/fruitlib/animation \
	fruitlib/include/fruitlib/geometry_traits \
	fruitlib/include/fruitlib/rectangle_manager \
	fruitlib/include/fruitlib/scenic/detail

update_cmake \
	fruitlib/CMakeLists.txt \
	FRUITLIB_CEGUI_FILES \
	$(fruitlib_dir scenic/adaptors/ce)

update_cmake \
	fruitapp/CMakeLists.txt \
	FRUITAPP_FILES \
	-n \
	fruitapp/src \
	fruitapp/src/gui \
	fruitapp/include/fruitapp \
	fruitapp/include/fruitapp/gui \
	-r \
	fruitapp/src/config_variables \
	fruitapp/src/font_particle \
	fruitapp/src/background \
	fruitapp/src/light \
	fruitapp/src/cursor \
	fruitapp/src/graphics_settings \
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
	fruitapp/include/fruitapp/depths \
	fruitapp/include/fruitapp/fruit \
	fruitapp/include/fruitapp/background \
	fruitapp/include/fruitapp/game_logic \
	fruitapp/include/fruitapp/shadow_map \
	fruitapp/include/fruitapp/config_variables \
	fruitapp/include/fruitapp/postprocessing \
	fruitapp/include/fruitapp/font_particle \
	fruitapp/include/fruitapp/highscore \
	fruitapp/include/fruitapp/graphics_settings \
	fruitapp/include/fruitapp/point_sprite \
	fruitapp/include/fruitapp/projection_manager \
	fruitapp/include/fruitapp/states \
	fruitapp/include/fruitapp/viewport \
	fruitapp/include/fruitapp/gui/dialogs \
	fruitapp/include/fruitapp/gui/dummy

update_cmake \
	fruitapp/CMakeLists.txt \
	FRUITAPP_CEGUI_FILES \
	fruitapp/src/gui/ce \
	fruitapp/include/fruitapp/gui/ce

update_cmake \
	fruitserver/CMakeLists.txt \
	FRUITSERVER_FILES \
	fruitserver/src \
	fruitserver/include
