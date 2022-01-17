# make a custom GLFW configuration file
macro(build_glfw install_prefix staging_prefix)

    SET (USE_SUPERBUILD ON)
    INCLUDE(ExternalProject)

    SET (GLFW3_VERSION_STRING 3.2 FORCE)
    SET (GLFW3_VERSION_MAJOR  3.2 FORCE)
    SET (GLFW3_VERSION_MINOR  0   FORCE)

    set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
    if(APPLE)
        list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
            -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
            -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
            -DMACOSX_RPATH:BOOL=ON
            -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
        )
    endif()


    ExternalProject_Add(glfw
        URL  "https://github.com/GLFW/GLFW/releases/download/3.2/GLFW-3.2.zip"
        URL_MD5 "d51fa09602abce26a65096ce97b4831c"
        UPDATE_COMMAND ""
        SOURCE_DIR  ${staging_prefix}/${install_prefix}-source
        BINARY_DIR  ${staging_prefix}/${install_prefix}-build
        INSTALL_DIR ${staging_prefix}/${install_prefix}-install
        LIST_SEPARATOR :::
        CMAKE_GENERATOR ${CMAKE_GEN}
        CMAKE_ARGS
            -DCMAKE_BUILD_TYPE:STRING=Release
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DCMAKE_INSTALL_PREFIX:PATH=${staging_prefix}/${install_prefix}-install
            -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
            -DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
            -DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}
            -DCMAKE_EXE_LINKER_FLAGS:STRING=${LINKER_FLAGS}
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE}
            -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
            -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
            -DMACOSX_RPATH:BOOL=ON
            -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
            -DCMAKE_INSTALL_PREFIX:PATH=${staging_prefix}/${install_prefix}-install
            -DGLFW_BUILD_DOCS:BOOL=OFF
            -DGLFW_BUILD_EXAMPLES:BOOL=OFF
            -DGLFW_BUILD_TESTS:BOOL=OFF
            -DDGLFW_USE_EGL:BOOL=ON
    )

    SET (GLFW3_INCLUDE_DIR ${staging_prefix}/${install_prefix}-install/include CACHE PATH "Where to find GLFW3 header files" FORCE)
    SET (glfw3_DIR         ${staging_prefix}/${install_prefix}-install/lib/cmake/glfw3 CACHE PATH "Where to find Cmake files on GLFW3" FORCE)
    SET (glfw3_FOUND ON FORCE)
    SET (GLFW3_LIBRARY_DIR "${staging_prefix}/${install_prefix}-install/lib")
    SET (GLFW3_LIBRARY     ${staging_prefix}/${install_prefix}-install/lib/libglfw3.a CACHE FILEPATH "Path to the GLFW3 library" FORCE)

    IF (WIN32)
        IF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
            SET(GLFW3_LIBRARY     ${staging_prefix}/${install_prefix}-install/lib/glfw3.lib CACHE FILEPATH "Path to the GLFW3 library" FORCE)
        ENDIF ()
    ENDIF (WIN32)

    install(DIRECTORY ${staging_prefix}/${install_prefix}-install/include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/third_party/include COMPONENT Development FILES_MATCHING PATTERN "*.*")
    install(DIRECTORY ${staging_prefix}/${install_prefix}-install/lib/     DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/third_party/lib     COMPONENT Development FILES_MATCHING PATTERN "*.*")
    install(DIRECTORY ${staging_prefix}/${install_prefix}-install/lib64/   DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/third_party/lib64   COMPONENT Development FILES_MATCHING PATTERN "*.*")
    install(DIRECTORY ${staging_prefix}/${install_prefix}-install/bin/     DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/third_party/bin     COMPONENT Development FILES_MATCHING PATTERN "*.*")
    install(DIRECTORY ${staging_prefix}/${install_prefix}-install/share/   DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/third_party/share   COMPONENT Development FILES_MATCHING PATTERN "*.*")
    install(DIRECTORY ${staging_prefix}/${install_prefix}-install/man/     DESTINATION ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/third_party/man     COMPONENT Development FILES_MATCHING PATTERN "*.*")


    SET (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${staging_prefix}/${install_prefix}-install/lib/cmake/glfw3 ${staging_prefix}/${install_prefix}-install/lib/cmake  ${staging_prefix}/${install_prefix}-install/lib)
    SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${staging_prefix}/${install_prefix}-install/lib/cmake/glfw3 ${staging_prefix}/${install_prefix}-install/lib/cmake  ${staging_prefix}/${install_prefix}-install/lib)



endmacro(build_glfw)
