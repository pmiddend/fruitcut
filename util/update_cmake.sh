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
	fruitapp/src \
	fruitapp/include

update_cmake_file \
	fruitserver/CMakeLists.txt \
	FRUITSERVER_FILES \
	fruitserver/src \
	fruitserver/include
