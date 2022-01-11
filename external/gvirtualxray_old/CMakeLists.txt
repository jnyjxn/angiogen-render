cmake_minimum_required(VERSION 3.12)

PROJECT(gVirtualXRay-Superbuild)


SET (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
INCLUDE (gvxrPreverntInSourceBuilds)

# Support for the CTest dashboard testing system
INCLUDE(CTest)

# C++ version ##################################################################
set(CMAKE_CXX_STANDARD 11) # C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++11 is required (I.e. not optional)
set(CMAKE_CXX_EXTENSIONS OFF) # without compiler extensions like gnu++11


# Flag required for shared libraries
SET (BUILD_SHARED_LIBS OFF)
IF (UNIX)
    IF (NOT APPLE)
        IF ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
            SET (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
            SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
        ENDIF ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "x86_64")
    ENDIF (NOT APPLE)
ENDIF (UNIX)


# for superbuild
SET (USE_SUPERBUILD ON)
INCLUDE(ExternalProject)


# Define NOMINMAX ##############################################################
IF (WIN32)
	SET (ADD_DEFINITIONS -DNOMINMAX)
ENDIF (WIN32)


INCLUDE (gvxr/cmake/options.cmake)
INCLUDE (Wrappers/cmake/options.cmake)


# Enable the unit tests
if (BUILD_TESTING)
    ENABLE_TESTING()
endif()


IF ( BUILD_WRAPPERS )
    MESSAGE (STATUS "Build gvxrWrappers(s): ON")
    MESSAGE (STATUS "SimpleGVXR will be built (needed by gvxrWrappers)")
ELSE ( BUILD_WRAPPERS )
    MESSAGE (STATUS "Build Wrapper(s): OFF")
ENDIF ( BUILD_WRAPPERS )


# Google Test ##################################################################
OPTION ( USE_SYSTEM_GTEST "Use an external version of GoogleTest. " OFF )
MARK_AS_ADVANCED ( USE_SYSTEM_GTEST )
IF ( BUILD_TESTING )
    IF ( USE_SYSTEM_GTEST )
        FIND_PACKAGE( GTest REQUIRED )
    ELSE( USE_SYSTEM_GTEST )
        INCLUDE(External_GTest)
        MESSAGE (STATUS "Build GTest: ON")
        list(APPEND ${CMAKE_PROJECT_NAME}_DEPENDENCIES GTest)

    ENDIF( USE_SYSTEM_GTEST )
ENDIF( BUILD_TESTING )


INCLUDE(gvxr/cmake/ConfigureExternalProjects.cmake)


# gVirtualXRay #################################################################
OPTION ( USE_SYSTEM_gVirtualXRay "Use an external version of gVirtualXRay. " OFF )
MARK_AS_ADVANCED ( USE_SYSTEM_gVirtualXRay )
IF ( USE_SYSTEM_gVirtualXRay )
    FIND_PACKAGE( gVirtualXRay REQUIRED )
ELSE( USE_SYSTEM_gVirtualXRay )
    INCLUDE(External_gVirtualXRay)
    MESSAGE (STATUS "Build gVirtualXRay: ON")
    list(APPEND ${CMAKE_PROJECT_NAME}_DEPENDENCIES gVirtualXRay)

    IF ( BUILD_TESTING AND NOT USE_SYSTEM_GTEST )
        ADD_DEPENDENCIES(gvxr googletest)
    ENDIF ()

ENDIF( USE_SYSTEM_gVirtualXRay )


# SimpleGVXR ###################################################################
OPTION ( USE_SYSTEM_SimpleGVXR "Use an external version of SimpleGVXR. " OFF )
MARK_AS_ADVANCED ( USE_SYSTEM_SimpleGVXR )
IF ( USE_SYSTEM_SimpleGVXR )
    FIND_PACKAGE( SimpleGVXR REQUIRED )
ELSE( USE_SYSTEM_SimpleGVXR )
    INCLUDE(External_SimpleGVXR)
    MESSAGE (STATUS "Build SimpleGVXR: ON")

    list(APPEND ${CMAKE_PROJECT_NAME}_DEPENDENCIES SimpleGVXR)

    IF ( BUILD_TESTING AND NOT USE_SYSTEM_GTEST )
        ADD_DEPENDENCIES(SimpleGVXR googletest)
    ENDIF ()

    IF ( NOT USE_SYSTEM_gVirtualXRay )
        ADD_DEPENDENCIES(SimpleGVXR gvxr)
    ENDIF ()

ENDIF( USE_SYSTEM_SimpleGVXR )



IF ( BUILD_WRAPPERS )

    INCLUDE(External_Wrappers)
    MESSAGE (STATUS "Build gvxrWrappers(s): ON")

    list(APPEND ${CMAKE_PROJECT_NAME}_DEPENDENCIES Wrappers)

    IF ( BUILD_TESTING AND NOT USE_SYSTEM_GTEST )
        ADD_DEPENDENCIES(Wrappers googletest)
    ENDIF ()

    IF ( NOT USE_SYSTEM_gVirtualXRay )
        ADD_DEPENDENCIES(Wrappers gvxr)
    ENDIF ()

    IF ( NOT USE_SYSTEM_SimpleGVXR )
        ADD_DEPENDENCIES(Wrappers SimpleGVXR)
    ENDIF ()

ENDIF ( BUILD_WRAPPERS )
