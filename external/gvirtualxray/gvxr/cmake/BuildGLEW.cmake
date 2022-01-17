macro(build_glew install_prefix staging_prefix)

FIND_PACKAGE(OpenGL REQUIRED)

IF (NOT OPENGL_GLU_FOUND)
    MESSAGE (FATAL_ERROR "Glu not found, GLEW cannot be built.")
ENDIF ()

SET (GLEW_VERSION_STRING 2.1)
SET (GLEW_VERSION_MAJOR  2.1)
SET (GLEW_VERSION_MINOR  0)


SET (GLEW_INSTALL_DIR ${staging_prefix}/${install_prefix}-install)

ExternalProject_Add(getGLEW
  URL  "https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.zip/download"
  URL_MD5 "dff2939fd404d054c1036cc0409d19f1"
  UPDATE_COMMAND ""
  SOURCE_DIR  ${staging_prefix}/${install_prefix}-source
  #BINARY_DIR  ${staging_prefix}/${install_prefix}-build
  INSTALL_DIR ${GLEW_INSTALL_DIR}
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
  #BINARY_DIR  ${staging_prefix}/${install_prefix}-build
  INSTALL_DIR ${GLEW_INSTALL_DIR}
  LIST_SEPARATOR :::
  BUILD_IN_SOURCE 1
  CMAKE_GENERATOR ${CMAKE_GEN}
  CMAKE_ARGS
        ${LOCAL_CMAKE_BUILD_OPTIONS}
      -DCMAKE_BUILD_TYPE:STRING=Release
      -DCMAKE_INSTALL_PREFIX:PATH=${GLEW_INSTALL_DIR}
)

ADD_DEPENDENCIES(glew getGLEW)

SET(GLEW_INCLUDE_DIR ${GLEW_INSTALL_DIR}/include  ${GLEW_INSTALL_DIR}/include)
SET(GLEW_DIR         ${GLEW_INSTALL_DIR})
SET(GLEW_FOUND ON)
SET(GLEW_LIBRARY     ${GLEW_INSTALL_DIR}/lib/libGLEW.a )

# copy libraries into install tree
install(DIRECTORY ${GLEW_INSTALL_DIR}/include/ DESTINATION ${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLEW_INSTALL_DIR}/lib/   DESTINATION ${INSTALL_DIR}/third_party/lib   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLEW_INSTALL_DIR}/bin/     DESTINATION ${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLEW_INSTALL_DIR}/share/   DESTINATION ${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
install(DIRECTORY ${GLEW_INSTALL_DIR}/man/     DESTINATION ${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")


endmacro(build_glew)
