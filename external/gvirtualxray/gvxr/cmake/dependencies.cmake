# ASSIMP #######################################################################
IF (USE_assimp)
    # Use system library
    IF (USE_SYSTEM_ASSIMP)
        FIND_PACKAGE( assimp )
    ENDIF (USE_SYSTEM_ASSIMP)

    # The system library was not found
    # Build ASSIMP instead
    IF (NOT assimp_FOUND OR NOT USE_SYSTEM_ASSIMP)
        MESSAGE (STATUS "Build ASSIMP")
        SET (USE_SYSTEM_ASSIMP OFF)
        INCLUDE(BuildASSIMP)
        build_assimp( assimp ${PROJECT_BINARY_DIR} )
    ENDIF (NOT assimp_FOUND OR NOT USE_SYSTEM_ASSIMP)
ENDIF(USE_assimp)

IF (assimp_FOUND)
    MESSAGE(STATUS "Use ASSIMP.")

  	SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${ASSIMP_INCLUDE_DIRS}")
  	SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_ASSIMP")
  	SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${ASSIMP_LIBRARY_DIRS})
  	SET (optionalLibs ${optionalLibs} ${ASSIMP_LIBRARIES})
ELSE (assimp_FOUND)
	MESSAGE (WARNING "ASSIMP not found, the use of ASSIMP will be disable.")
ENDIF (assimp_FOUND)


# OpenMP ######################################################################
FIND_PACKAGE(OpenMP)

IF (OPENMP_FOUND)
    MESSAGE (STATUS "Use OpenMP")

    SET (USE_OPENMP ON PARENT_SCOPE)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}" PARENT_SCOPE)
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_OPENMP")
    SET (requiredLibs ${requiredLibs} ${OpenMP_EXE_LINKER_FLAGS} OpenMP::OpenMP_CXX)
ELSE (OPENMP_FOUND)
    MESSAGE (WARNING "OpenMP not found, the use of OpenMP will be disable.")
    SET (USE_OPENMP OFF)
ENDIF (OPENMP_FOUND)


# Vulkan ######################################################################
#FIND_PACKAGE(Vulkan)

IF (Vulkan_FOUND)
    MESSAGE (STATUS "Use VULKAN")

    SET (USE_VULKAN ON)
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${Vulkan_INCLUDE_DIRS}")
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_VULKAN" "-DGLFW_INCLUDE_VULKAN")

    SET (optionalLibs ${optionalLibs} ${Vulkan_LIBRARIES})
ELSE (Vulkan_FOUND)
    MESSAGE (WARNING "Vulkan not found, the use of Vulkan will be disable.")
    SET (USE_VULKAN OFF)
ENDIF (Vulkan_FOUND)


# GZIP ########################################################################
FIND_PACKAGE(GZIP)

IF (NOT GZIP_FOUND)
    IF (WIN32)

        file(DOWNLOAD http://gnuwin32.sourceforge.net/downlinks/gzip-bin-zip.php ${PROJECT_BINARY_DIR}/gzip/gzip-bin.zip)
		execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${PROJECT_BINARY_DIR}/gzip/gzip-bin.zip WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/gzip)
        SET (GZIP ${PROJECT_BINARY_DIR}/gzip/bin/gzip.exe)
        SET (GZIP_TOOL ${GZIP} CACHE FILEPATH "Where gzip.exe is." FORCE)

        FIND_PACKAGE(GZIP REQUIRED)
        IF (NOT GZIP_FOUND)
            MESSAGE(FATAL_ERROR "GZIP not found. You can find it on gzip's website at: http://www.gzip.org/")
        ENDIF (NOT GZIP_FOUND)

    ELSE (WIN32)
        MESSAGE(FATAL_ERROR "GZIP not found.")
    ENDIF (WIN32)
ENDIF (NOT GZIP_FOUND)


# OpenGL ######################################################################

IF (WIN32 OR APPLE)
    FIND_PACKAGE(OpenGL REQUIRED)
    SET (requiredLibs ${requiredLibs} ${OPENGL_LIBRARIES})

ELSE (WIN32 OR APPLE)
    FIND_PACKAGE(OpenGL REQUIRED COMPONENTS OpenGL)
    SET (requiredLibs ${requiredLibs} OpenGL::OpenGL)

    FIND_PACKAGE(OpenGL COMPONENTS GLX EGL GLU)
    IF (OpenGL_GLX_FOUND)
    	 SET (optionalLibs ${optionalLibs} OpenGL::GLX)
    ENDIF (OpenGL_GLX_FOUND)

    IF (OpenGL_EGL_FOUND)
        SET (optionalLibs ${optionalLibs} OpenGL::EGL)
    ENDIF (OpenGL_EGL_FOUND)

    IF (OpenGL_GLU_FOUND)
        SET (optionalLibs ${optionalLibs} OpenGL::GLU)
    ENDIF (OpenGL_GLU_FOUND)
ENDIF (WIN32 OR APPLE)

SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${OPENGL_INCLUDE_DIR}")
SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DGLFW_INCLUDE_GLCOREARB")

IF (OpenGL_EGL_FOUND)
    MESSAGE (STATUS "EGL found.")
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_EGL")
ENDIF (OpenGL_EGL_FOUND)

IF (OpenGL_GLU_FOUND)
    MESSAGE (STATUS "GLU found.")
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_GLU")
ENDIF (OpenGL_GLU_FOUND)


# GLFW3 ########################################################################
INCLUDE (configureGlfw)


# GLEW #########################################################################
# Deals with cross-compilation using Mac OS X (building GLEW does not work in this case)
IF (USE_SYSTEM_GLEW)
    # Use system library
    FIND_PACKAGE(GLEW)
ENDIF (USE_SYSTEM_GLEW)

# The system library was not found
# Build GLEW instead
IF (NOT GLEW_FOUND OR NOT USE_SYSTEM_GLEW)
    MESSAGE (STATUS "Build GLEW")
    SET (USE_SYSTEM_GLEW OFF)
    INCLUDE(BuildGLEW)
    build_glew( glew ${PROJECT_BINARY_DIR} )
ENDIF (NOT GLEW_FOUND OR NOT USE_SYSTEM_GLEW)

IF (GLEW_FOUND)
    MESSAGE (STATUS "Use GLEW")

    SET (HAS_GLEW ON)
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DGLEW_STATIC")
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_GLEW")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${GLEW_INCLUDE_DIR}")
    SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${GLEW_LIBRARY_DIR})
    SET (requiredLibs "${requiredLibs}" "${GLEW_LIBRARY}")
ENDIF (GLEW_FOUND)


# TIFF #########################################################################
IF (USE_LIBTIFF)
    # Use system library
    IF (USE_SYSTEM_LIBTIFF)
        FIND_PACKAGE( TIFF )
    ENDIF ()

    # The system library was not found
    # Build LibTIFF instead
    IF (NOT TIFF_FOUND OR NOT USE_SYSTEM_LIBTIFF)
        MESSAGE (STATUS "Build LibTIFF")
        SET (USE_SYSTEM_LIBTIFF OFF)
        INCLUDE(BuildLibTIFF)
        build_libtiff( LibTIFF ${PROJECT_BINARY_DIR} )
    ENDIF (NOT TIFF_FOUND OR NOT USE_SYSTEM_LIBTIFF)
ENDIF (USE_LIBTIFF)

IF (TIFF_FOUND)
    MESSAGE (STATUS "Use LibTIFF")

    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_LIBTIFF" "-DHAS_TIFF")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${TIFF_INCLUDE_DIRS}")
    SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${TIFF_LIBRARY_DIR})
    SET (requiredLibs "${requiredLibs}" "${TIFF_LIBRARIES}")
ELSE (TIFF_FOUND)
    MESSAGE (WARNING "LibTIFF not found, the use of LibTIFF will be disable.")
ENDIF (TIFF_FOUND)


# GDCM #########################################################################
IF (USE_GDCM)
    # Use system library
    IF (USE_SYSTEM_GDCM)
        FIND_PACKAGE( GDCM )
    ENDIF(USE_SYSTEM_GDCM)

    # The system library was not found
    # Build GDCM instead
    IF (NOT GDCM_FOUND OR NOT USE_SYSTEM_GDCM)
        MESSAGE (STATUS "Build GDCM")
        SET (USE_SYSTEM_GDCM OFF)
        INCLUDE(BuildGDCM)
        build_gdcm( gdcm ${PROJECT_BINARY_DIR} )
    ENDIF (NOT GDCM_FOUND OR NOT USE_SYSTEM_GDCM)
ENDIF(USE_GDCM)

IF (GDCM_FOUND)
    MESSAGE(STATUS "Use GDCM.")

    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_GDCM")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${GDCM_INCLUDE_DIRS}")
    SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${GDCM_LIBRARY_DIR})
    SET (requiredLibs "${requiredLibs}" "${GDCM_LIBRARIES}")
ELSE (GDCM_FOUND)
    MESSAGE (WARNING "GDCM not found, the use of GDCM will be disable.")
ENDIF (GDCM_FOUND)


# ZLIB #########################################################################
# Use system library
IF (USE_SYSTEM_ZLIB)
    FIND_PACKAGE( ZLIB )
ENDIF(USE_SYSTEM_ZLIB)

# The system library was not found
# Build Zlib instead
IF(NOT ZLIB_FOUND OR NOT USE_SYSTEM_ZLIB)
    MESSAGE (STATUS "Build Zlib")
    INCLUDE(BuildZLIB)
    build_zlib( zlib ${PROJECT_BINARY_DIR} )
ENDIF (NOT ZLIB_FOUND OR NOT USE_SYSTEM_ZLIB)

IF (ZLIB_FOUND)
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${ZLIB_INCLUDE_DIR}")
    SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${ZLIB_LIBRARY_DIR})
    SET (requiredLibs "${requiredLibs}" "${ZLIB_LIBRARY}")
ENDIF (ZLIB_FOUND)


# VCG ##########################################################################
IF (USE_VCG)
    # Use system library
    IF (USE_SYSTEM_VCG)
        FIND_PACKAGE( VCG )
    ENDIF(USE_SYSTEM_VCG)

    # The system library was not found
    # Build VCG instead
    IF(NOT VCG_FOUND OR NOT USE_SYSTEM_VCG)
        MESSAGE (STATUS "Build VCG")
        INCLUDE(BuildVCG)
        build_vcg( vcg ${PROJECT_BINARY_DIR} )
    ENDIF (NOT VCG_FOUND OR NOT USE_SYSTEM_VCG)

    IF (VCG_FOUND)
        MESSAGE (STATUS "USE VCG")
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${VCG_INCLUDE_DIR}")
    ELSE (VCG_FOUND)
        MESSAGE (WARNING "VCG not found, the use of VCG will be disable.")
    ENDIF (VCG_FOUND)
ENDIF (USE_VCG)


# ITK #########################################################################
IF (USE_ITK)
    FIND_PACKAGE(ITK)
ENDIF (USE_ITK)

IF (ITK_FOUND)
    include(${ITK_USE_FILE})
    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_ITK")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${ITK_INCLUDE_DIRS}")
    SET (requiredLibs ${requiredLibs} ${ITK_LIBRARIES})
ELSE(ITK_FOUND)
    MESSAGE(WARNING "ITK not found, it will be disable.")
    SET (USE_ITK OFF)
ENDIF (ITK_FOUND)


# FFTW3 ########################################################################
IF (USE_FFTW3)

    # Use system library
    IF (USE_SYSTEM_FFTW3)
        FIND_LIBRARY(FFTW3_LIBRARIES NAMES fftw3 libfftw3 libfftw3_omp)
    ENDIF (USE_SYSTEM_FFTW3)

    # The system library was not found
    # Build FFTW3 instead (Unixes only)
    IF (NOT USE_SYSTEM_FFTW3 OR NOT FFTW3_LIBRARIES)
        IF (UNIX)
            MESSAGE (STATUS "Build FFTW3")
            SET (USE_SYSTEM_FFTW3 OFF)
            INCLUDE(BuildFFTW3)
            build_fftw3( fftw3 ${PROJECT_BINARY_DIR} )
        ELSE (UNIX)
            MESSAGE (WARNING "Building FFTW3 on windows automatically is not supported. Either disable the Fast Fourier Transform, or build it by hand.")
        ENDIF (UNIX)
    ENDIF (NOT USE_SYSTEM_FFTW3 OR NOT FFTW3_LIBRARIES)

    IF (FFTW3_LIBRARIES)
        MESSAGE(STATUS "Use FFTW3.")

        SET (requiredLibs ${requiredLibs} ${FFTW3_LIBRARIES})
        SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_FFTW")
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${FFTW3_INCLUDE_DIR}")
        SET (gVirtualXRay_LIBRARY_DIRS ${gVirtualXRay_LIBRARY_DIRS} ${FFTW3_LIBRARY_DIR})
        SET (requiredLibs "${requiredLibs}" "${FFTW3_LIBRARY}")
    ELSE ()
        MESSAGE (WARNING "FFTW3 not found, the use of FFT will be disable.")
    ENDIF ()

ENDIF (USE_FFTW3)



# FREETYPE ####################################################################
IF (USE_FREETYPE)
    # Use system library
    IF (USE_SYSTEM_FREETYPE)
        FIND_PACKAGE(Freetype)
    ENDIF (USE_SYSTEM_FREETYPE)

    # The system library was not found
    # Build Freetype instead
    IF (NOT USE_SYSTEM_FREETYPE OR NOT FREETYPE_FOUND)
        MESSAGE (STATUS "Build Freetype")
        INCLUDE(BuildFreeType)
        build_freetype( freetype ${PROJECT_BINARY_DIR} )
    ENDIF (NOT USE_SYSTEM_FREETYPE OR NOT FREETYPE_FOUND)
ENDIF (USE_FREETYPE)

IF (USE_FREETYPE AND FREETYPE_FOUND)
    MESSAGE (STATUS "Use Freetype")

    SET (gVirtualXRay_ADD_DEFINITIONS ${gVirtualXRay_ADD_DEFINITIONS} "-DHAS_FREETYPE")
    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${FREETYPE_INCLUDE_DIRS}")
    SET (optionalLibs "${optionalLibs}" "${FREETYPE_LIBRARIES}")

    FIND_PACKAGE(PNG)
    IF (PNG_FOUND)
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${PNG_INCLUDE_DIRS}")
        SET(optionalLibs ${optionalLibs} ${PNG_LIBRARIES})
    ENDIF (PNG_FOUND)

    FIND_PACKAGE(BZip2)
    IF (BZIP2_FOUND)
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${BZIP2_INCLUDE_DIR}")
        SET(optionalLibs ${optionalLibs} ${BZIP2_LIBRARIES})
    ENDIF (BZIP2_FOUND)

    #FIND_PACKAGE(HARFBUZZ)
    #IF (HARFBUZZ_FOUND)
    #    SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${HARFBUZZ_INCLUDE_DIRS}")
    #    SET(optionalLibs ${optionalLibs} ${HARFBUZZ_LIBRARIES})
    #ENDIF (HARFBUZZ_FOUND)

ELSE (USE_FREETYPE AND FREETYPE_FOUND)
    MESSAGE (STATUS "Freetype not found, the use of Freetype will be disable.")
ENDIF (USE_FREETYPE AND FREETYPE_FOUND)


# getopt ######################################################################
IF (WIN32 AND MSVC)

    FIND_PACKAGE(Git)

    IF (GIT_FOUND)
        INCLUDE (ExternalProject)
        ExternalProject_Add(getopt
            PREFIX ${CMAKE_CURRENT_BINARY_DIR}/getopt
            #DOWNLOAD_DIR ${CMAKE_CURRENT_BINARY_DIR}/download/getopt
            GIT_REPOSITORY "https://github.com/skandhurkat/Getopt-for-Visual-Studio.git"
            CONFIGURE_COMMAND ""
            UPDATE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ""
        )

        INCLUDE_DIRECTORIES (${CMAKE_CURRENT_BINARY_DIR}/getopt/src/getopt)
        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${CMAKE_CURRENT_BINARY_DIR}/getopt/src/getopt")

        # copy libraries into install tree
        INSTALL (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/getopt/src/getopt
            DESTINATION ${INSTALL_DIR}/third_party
            COMPONENT Development)

        SET (gVirtualXRay_INCLUDE_DIRS ${gVirtualXRay_INCLUDE_DIRS} "${INSTALL_DIR}/third_party/getopt")

        SET (USE_EXTERNAL_GETOPT ON)
        SET (HAS_GETOPT ON)
    ELSE (GIT_FOUND)
        SET (HAS_GETOPT OFF)
        MESSAGE (WARNING "Install Git to download Getopt (it is not available in MS Visual Studio). If you don't (which is fine) command line tools won't be built.")
    ENDIF (GIT_FOUND)
ELSE (WIN32 AND MSVC)
    SET (HAS_GETOPT ON)
ENDIF (WIN32 AND MSVC)


# XCOM Database ################################################################
# XCOM: Photon Cross Sections Database
# http://www.nist.gov/pml/data/xcom/
# http://physics.nist.gov/PhysRefData/Xcom/Text/download.html

IF (USE_SYSTEM_XCOM)
    find_path (XCOM_PATH MDATX3.001 MDATX3.100 NO_DEFAULT_PATH)
    IF (NOT XCOM_PATH)
        MESSAGE(WARNING "XCOM, Photon Cross Sections Database, not found. Its use will be disable. You can find it on the NIST's website at: http://physics.nist.gov/PhysRefData/Xcom/Text/download.html")
    ENDIF (NOT XCOM_PATH)

ELSE(USE_SYSTEM_XCOM)
    INCLUDE(BuildXCOM)
    build_xcom( ${PROJECT_BINARY_DIR} )
ENDIF (USE_SYSTEM_XCOM)
