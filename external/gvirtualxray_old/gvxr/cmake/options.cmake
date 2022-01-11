# The options ##################################################################
OPTION(USE_assimp         "Use the Open-Asset-Importer-Lib (ASSIMP) to load 3D meshes" ON)
OPTION(USE_OPENSCENEGRAPH "Use the OpenSceneGraph to load 3D meshes" OFF)
OPTION(USE_ITK            "Use ITK"                 OFF)
OPTION(USE_FFTW3          "Use FFTW3"               OFF)
OPTION(USE_FREETYPE       "Use FreeType"            OFF)
OPTION(USE_GDCM "Use GDCM (Grassroots DICOM library) to save DICOM files" OFF)
OPTION(USE_LIBTIFF "Use LibTIFF to save TIFF files" ON)


OPTION(USE_SYSTEM_GLFW       "Use System GLFW"       OFF)
OPTION(USE_SYSTEM_ASSIMP   "Use System ASSIMP"      OFF)
OPTION(USE_SYSTEM_GLEW    "Use System GLEW"      OFF)

IF (WIN32 OR MSVC)
    OPTION(USE_SYSTEM_ZLIB       "Use system ZLIB "       OFF)
    OPTION(USE_SYSTEM_LIBTIFF       "Use System LibTiff"       OFF)
ELSE (WIN32 OR MSVC)
    OPTION(USE_SYSTEM_ZLIB       "Use system ZLIB "       ON)
    OPTION(USE_SYSTEM_LIBTIFF       "Use System LibTiff"       ON)
ENDIF (WIN32 OR MSVC)


IF (BUILD_EXAMPLES OR BUILD_TUTORIALS)

    IF (NOT APPLE)
        OPTION(USE_FLTK "Build the programs (demos, and tutorials) that make use of the Fast Light Toolkit (FLTK)" ON)
    ELSE (NOT APPLE)
        OPTION(USE_FLTK "Build the programs (demos, and tutorials) that make use of the Fast Light Toolkit (FLTK)" OFF)
    ENDIF (NOT APPLE)

    IF (USE_FLTK)
        IF (WIN32 OR MSVC)
            OPTION(USE_SYSTEM_FLTK   "Use System FLTK"       OFF)
        ELSE (WIN32 OR MSVC)
            OPTION(USE_SYSTEM_FLTK   "Use System FLTK"       ON)
        ENDIF (WIN32 OR MSVC)
    ENDIF (USE_FLTK)

    OPTION(USE_GLUT "Build the programs (demos, and tutorials) that make use of the OpenGL Utility Toolkit (GLUT)" ON)
    IF (USE_GLUT)
        IF (NOT APPLE)
            OPTION(USE_SYSTEM_GLUT   "Use System GLUT"       OFF)
        ELSE (NOT APPLE)
            OPTION(USE_SYSTEM_GLUT   "Use System GLUT"       ON)
        ENDIF (NOT APPLE)
    ENDIF (USE_GLUT)

    IF (NOT APPLE)
	OPTION(USE_QT4  "Build the programs (demos, and tutorials) that make use of Qt4" OFF)
    ELSE (NOT APPLE)
        OPTION(USE_QT4  "Build the programs (demos, and tutorials) that make use of Qt4" OFF)
    ENDIF (NOT APPLE)

    OPTION(USE_QT5  "Build the programs (demos, and tutorials) that make use of Qt5" ON)

    IF (WIN32)
        IF (EX_PLATFORM EQUAL 32)
            OPTION(USE_QT4  "Build the programs (demos, and tutorials) that make use of Qt4" OFF)
        ENDIF ()
    ENDIF (WIN32)

ELSE ()
    SET (USE_FLTK OFF)
    SET (USE_GLUT OFF)
    SET (USE_QT4 OFF)
    SET (USE_QT5 OFF)
ENDIF ()

OPTION(USE_SYSTEM_XCOM       "Use System XCOM database"       OFF)

IF (USE_FFTW3)
    OPTION(USE_SYSTEM_FFTW3       "Use System FFTW3 "       ON)
ENDIF (USE_FFTW3)

IF (USE_FREETYPE)
    OPTION(USE_SYSTEM_FREETYPE       "Use System FreeType "       ON)
ENDIF (USE_FREETYPE)

IF (USE_GDCM)
    IF (${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin" AND ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        OPTION(USE_SYSTEM_GDCM       "Use System GDCM"       ON)
    ELSE ()
        OPTION(USE_SYSTEM_GDCM       "Use System GDCM"       OFF)
   ENDIF ()
ENDIF (USE_GDCM)
