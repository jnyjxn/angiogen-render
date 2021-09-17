#
#
# Copyright (c) 2014, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
# http://www.fpvidal.net/
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# 3. Neither the name of the Bangor University nor the names of its contributors
# may be used to endorse or promote products derived from this software without
# specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


# Set the policy
cmake_policy(SET CMP0011 NEW)


# Fix linkage problem with windows and cross-compilation #######################
IF (WIN32)
    ADD_DEFINITIONS(-DGLEW_STATIC)
    ADD_DEFINITIONS(-DFREEGLUT_STATIC)
    SET (CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lwinmm")
ENDIF (WIN32)


# GLFW ########################################################################

IF (USE_SYSTEM_GLFW)
    FIND_PACKAGE(glfw3)
ENDIF (USE_SYSTEM_GLFW)

IF(glfw3_FOUND)

    LINK_DIRECTORIES(${GLFW3_LIBRARY_DIR})
#SET(optionalLibs ${optionalLibs} glfw3)

	IF (APPLE)
  	FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(IOKIT_LIBRARY IOKit)
		FIND_LIBRARY(QUARTZ_LIBRARY Quartz)
  	FIND_LIBRARY(CORE_VIDEO_LIBRARY CoreVideo)

    SET(optionalLibs ${optionalLibs} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${QUARTZ_LIBRARY} ${CORE_VIDEO_LIBRARY})
	    ENDIF(APPLE)

  IF (UNIX)
    FIND_PACKAGE (X11 REQUIRED)
		iF (X11_FOUND)
  		INCLUDE_DIRECTORIES(${X11_INCLUDE_DIRS})
    	SET(optionalLibs dl pthread ${optionalLibs} ${X11_X11_LIB} ${X11_Xrandr_LIB} ${X11_Xinput_LIB} ${X11_Xxf86vm_LIB} ${X11_Xcursor_LIB} ${X11_Xinerama_LIB})
  	ELSE (X11_FOUND)
    	MESSAGE(FATAL_ERROR "X11 not found.")
		ENDIF (X11_FOUND)
  ENDIF(UNIX)

    INCLUDE_DIRECTORIES(${GLFW3_INCLUDE_DIR})
SET(optionalLibs ${optionalLibs} ${GLFW3_LIBRARY})

ELSE(glfw3_FOUND)
    MESSAGE(WARNING "GLFW3 not found.")
ENDIF(glfw3_FOUND)
