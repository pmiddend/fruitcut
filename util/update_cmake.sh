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
	CMakeLists.txt \
	FRUITLIB_FILES \
	src/fruitlib \

update_cmake_file \
	CMakeLists.txt \
	APP_FILES \
	src/fruitapp \

update_cmake_file \
	CMakeLists.txt \
	SERVER_FILES \
	src/server \
