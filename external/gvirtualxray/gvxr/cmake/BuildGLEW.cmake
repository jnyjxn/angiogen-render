macro(build_glew install_prefix staging_prefix)

FIND_PACKAGE(OpenGL REQUIRED)

IF (NOT OPENGL_GLU_FOUND)
	MESSAGE (FATAL_ERROR "Glu not found, GLEW cannot be built.")
ENDIF ()

#SET (GLEW_VERSION_STRING 2.2)
#SET (GLEW_VERSION_MAJOR  2.2)
#SET (GLEW_VERSION_MINOR  0)


SET (GLEW_INSTALL_DIR ${staging_prefix}/${install_prefix}-install)

ExternalProject_Add(getGLEW
  URL  "https://sourceforge.net/projects/glew/files/glew/2.2.0/glew-2.2.0.tgz/download"
  URL_MD5 "3579164bccaef09e36c0af7f4fd5c7c7"
  UPDATE_COMMAND ""
  SOURCE_DIR  ${staging_prefix}/${install_prefix}-source
  #BINARY_DIR  ${staging_prefix}/${install_prefix}-build
  #INSTALL_DIR ${GLEW_INSTALL_DIR}
  LIST_SEPARATOR :::
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  CMAKE_GENERATOR ""
  INSTALL_COMMAND ""
)

ExternalProject_Add(glew
  DOWNLOAD_COMMAND ""
  UPDATE_COMMAND ""
  SOURCE_DIR  ${staging_prefix}/${install_prefix}-source/build/cmake
  BINARY_DIR  ${staging_prefix}/${install_prefix}-build
  INSTALL_DIR ${GLEW_INSTALL_DIR}
  LIST_SEPARATOR :::
  #BUILD_IN_SOURCE 1
  CMAKE_GENERATOR ${CMAKE_GEN}
  CMAKE_ARGS
        ${LOCAL_CMAKE_BUILD_OPTIONS}
				-DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
				-DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
				-DCMAKE_BUILD_TYPE:STRING=Release
      -DCMAKE_INSTALL_PREFIX:PATH=${GLEW_INSTALL_DIR}
)

ADD_DEPENDENCIES(glew getGLEW)

SET(GLEW_INCLUDE_DIR ${GLEW_INSTALL_DIR}/include  ${GLEW_INSTALL_DIR}/include)
SET(GLEW_DIR         ${GLEW_INSTALL_DIR})
SET(GLEW_FOUND ON)

IF (UNIX)
    SET(GLEW_LIBRARY     ${GLEW_INSTALL_DIR}/lib/libGLEW.a )
ENDIF (UNIX)

IF (APPLE)
    SET(GLEW_LIBRARY     ${GLEW_INSTALL_DIR}/lib/libGLEW.a )
ENDIF (APPLE)

if (WIN32)
    if (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
	    SET(GLEW_LIBRARY
		optimized ${GLEW_INSTALL_DIR}/lib/libglew32.lib
		debug     ${GLEW_INSTALL_DIR}/lib/libglew32d.lib
        )
    ELSE ()
        SET(GLEW_LIBRARY
           ${GLEW_INSTALL_DIR}/lib/libglew32.a
        )
    ENDIF ()
endif (WIN32)


# copy libraries into install tree
install(DIRECTORY ${GLEW_INSTALL_DIR}/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLEW_INSTALL_DIR}/lib/     DESTINATION ${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLEW_INSTALL_DIR}/lib64/   DESTINATION ${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLEW_INSTALL_DIR}/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLEW_INSTALL_DIR}/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLEW_INSTALL_DIR}/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")


endmacro(build_glew)
