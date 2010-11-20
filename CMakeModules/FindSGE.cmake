# - Try to find SGE
#
# This module defines the following variables
#
#
#	SGE_FOUND
#	SGE_INCLUDE_DIRS
#	SGE_CORE_LIBRARY
#	SGE_GUI_LIBRARY
#	SGE_SHADER_LIBRARY
#	SGE_SYSTEMS_LIBRARY
#	SGE_CONSOLE_LIBRARY
#

IF (NOT SGE_ROOT)
	SET(SGE_ROOT CACHE PATH "")
ENDIF()

FIND_PATH(
	SGE_INCLUDE_DIRS
	NAMES sge/config.hpp
	HINTS
	${SGE_ROOT}/include
)

FIND_LIBRARY(
	SGE_CORE_LIBRARY
	NAMES sgecore
	HINTS ${SGE_ROOT}/lib
)

FIND_LIBRARY(
	SGE_GUI_LIBRARY
	NAMES sgegui
	HINTS ${SGE_ROOT}/lib
)

FIND_LIBRARY(
	SGE_SHADER_LIBRARY
	NAMES sgeshader
	HINTS ${SGE_ROOT}/lib
)

FIND_LIBRARY(
	SGE_CONSOLE_LIBRARY
	NAMES sgeconsole
	HINTS ${SGE_ROOT}/lib
)

FIND_LIBRARY(
	SGE_SYSTEMS_LIBRARY
	NAMES sgesystems
	HINTS ${SGE_ROOT}/lib
)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(
	SGE
	DEFAULT_MSG
	SGE_CORE_LIBRARY
	SGE_GUI_LIBRARY
	SGE_CONSOLE_LIBRARY
	SGE_SHADER_LIBRARY
	SGE_SYSTEMS_LIBRARY
	SGE_INCLUDE_DIRS
)

MARK_AS_ADVANCED(
	SGE_CORE_LIBRARY
	SGE_GUI_LIBRARY
	SGE_SHADER_LIBRARY
	SGE_CONSOLE_LIBRARY
	SGE_SYSTEMS_LIBRARY
	SGE_INCLUDE_DIRS
)
