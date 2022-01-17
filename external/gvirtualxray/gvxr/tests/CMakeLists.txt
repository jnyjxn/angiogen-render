#
#
# Copyright (c) 2019, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
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


# Use C++ 11
set(CMAKE_CXX_STANDARD 11) # C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++11 is required (I.e. not optional)
set(CMAKE_CXX_EXTENSIONS OFF) # without compiler extensions like gnu++11




if(${CMAKE_VERSION} VERSION_GREATER_EQUAL  "3.13.0")
    cmake_policy(SET CMP0075 NEW)
endif()




# Enable unit testing
enable_testing()
include_directories(${GTEST_INCLUDE_DIRS})
link_directories(${GTEST_LIBS_DIR})

# Functions needed
# does this system provide the log and exp functions?
SET(CMAKE_REQUIRED_LIBRARIES m)

include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX("cmath" HAVE_CMATH_H)

include(CheckFunctionExists)
check_function_exists(cos HAVE_COS)
check_function_exists(sin HAVE_SIN)
check_function_exists(log HAVE_LOG)
check_function_exists(exp HAVE_EXP)
check_function_exists(pow HAVE_POW)
check_function_exists(sqrt HAVE_SQRT)
check_function_exists(abs HAVE_ABS)
check_function_exists(cbrt HAVE_CBRT)




# Valiation_02
add_executable(TestDensity                     TestDensity.cxx)

# Validation_01
add_executable(TestMassAttenuationCoefficients TestMassAttenuationCoefficients.cxx)

IF (glfw3_FOUND)
    add_executable(TestCubeWithIndex    TestCubeWithIndex)
    add_executable(TestCubeWithoutIndex TestCubeWithoutIndex)

    # Validation_??
    add_executable(TestLBuffer TestLBuffer.cxx)

    # Validation_03
    add_executable(TestBeerLambertLawMonoChromatic TestBeerLambertLawMonoChromatic.cxx)

    # Validation_05
    add_executable(TestBeerLambertLawPolyChromatic TestBeerLambertLawPolyChromatic.cxx)

    # Validation_08
    add_executable(TestSinogramFromMesh            TestSinogramFromMesh.cxx)

    # Validation_??
    #add_executable(TestRadonTransformFromVoxels   TestRadonTransformFromVoxels.cxx)

    # Validation_??
    #add_executable(TestSinogramFromVoxels          TestSinogramFromVoxels.cxx)

    # Validation_06
    add_executable(TestPointSource                 TestPointSource.cxx)

    # Validation_07
    add_executable(TestCubeSource                  TestCubeSource.cxx)

    # Validation_09
    add_executable(TestUncenteredSource            TestUncenteredSource.cxx)

    # Validation_??
    #add_executable(TestPhantomMonoChromatic        TestPhantomMonoChromatic.cxx)

    # Validation_04
    add_executable(TestRadonTransformFromMesh       TestRadonTransformFromMesh.cxx)

    add_executable(TestOrtho                        TestOrtho.cxx)

    IF (FFTW3_LIBRARIES)
	    # Validation_04
        add_executable(TestFBP                     TestFBP.cxx)
    ENDIF()
ENDIF()


list(APPEND GTEST_LIBRARIES gtest gtest_main)
separate_arguments(GTEST_LIBRARIES)

target_link_libraries(TestDensity                     ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} "${GTEST_LIBRARIES}")
target_link_libraries(TestMassAttenuationCoefficients ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})

target_link_libraries(TestCubeWithoutIndex            ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})
target_link_libraries(TestCubeWithIndex               ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})
target_link_libraries(TestLBuffer                     ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})
target_link_libraries(TestBeerLambertLawMonoChromatic ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})
target_link_libraries(TestBeerLambertLawPolyChromatic ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})
target_link_libraries(TestSinogramFromMesh            ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})
#target_link_libraries(TestSinogramFromVoxels          ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})
target_link_libraries(TestPointSource                 ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})
target_link_libraries(TestCubeSource                  ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})
target_link_libraries(TestUncenteredSource            ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})
#target_link_libraries(TestPhantomMonoChromatic        ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})
target_link_libraries(TestRadonTransformFromMesh      ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})
target_link_libraries(TestOrtho                      ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})

IF (FFTW3_LIBRARIES)
    target_link_libraries(TestFBP                     ${gVirtualXRay_LIBRARY} ${optionalLibs} ${requiredLibs} ${GTEST_LIBRARIES})
ENDIF()

# Add dependencies
 ADD_DEPENDENCIES(TestDensity   gVirtualXRay)
 ADD_DEPENDENCIES(TestMassAttenuationCoefficients   gVirtualXRay)

 ADD_DEPENDENCIES(TestCubeWithoutIndex              gVirtualXRay)
 ADD_DEPENDENCIES(TestCubeWithIndex                 gVirtualXRay)
 ADD_DEPENDENCIES(TestLBuffer                       gVirtualXRay)
 ADD_DEPENDENCIES(TestBeerLambertLawMonoChromatic   gVirtualXRay)
 ADD_DEPENDENCIES(TestBeerLambertLawPolyChromatic   gVirtualXRay)
 ADD_DEPENDENCIES(TestSinogramFromMesh              gVirtualXRay)
 #ADD_DEPENDENCIES(TestSinogramFromVoxels            gVirtualXRay)
 ADD_DEPENDENCIES(TestPointSource                   gVirtualXRay)
 ADD_DEPENDENCIES(TestCubeSource                    gVirtualXRay)
 ADD_DEPENDENCIES(TestUncenteredSource              gVirtualXRay)
 #ADD_DEPENDENCIES(TestPhantomMonoChromatic          gVirtualXRay)
 ADD_DEPENDENCIES(TestRadonTransformFromMesh        gVirtualXRay)
 ADD_DEPENDENCIES(TestOrtho                         gVirtualXRay)

IF (FFTW3_LIBRARIES)
     ADD_DEPENDENCIES(TestFBP                           gVirtualXRay)
ENDIF()

add_test (TestDensity TestDensity)
add_test (TestMassAttenuationCoefficients TestMassAttenuationCoefficients)

add_test (TestCubeWithoutIndex            TestCubeWithoutIndex)
add_test (TestCubeWithIndex               TestCubeWithIndex)
#add_test (TestLBuffer                     TestLBuffer)
add_test (TestBeerLambertLawMonoChromatic TestBeerLambertLawMonoChromatic)
add_test (TestBeerLambertLawPolyChromatic TestBeerLambertLawPolyChromatic)
add_test (TestSinogramFromMesh            TestSinogramFromMesh)
#add_test (TestSinogramFromVoxels          TestSinogramFromVoxels)
add_test (TestPointSource                 TestPointSource)
add_test (TestCubeSource                  TestCubeSource)
add_test (TestUncenteredSource            TestUncenteredSource)
#add_test (TestPhantomMonoChromatic        TestPhantomMonoChromatic)
#add_test (TestRadonTransformFromMesh      TestRadonTransformFromMesh)
add_test (TestOrtho                       TestOrtho)

#IF (FFTW3_LIBRARIES)
#    add_test (TestFBP                     TestFBP)
#ENDIF()

FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/point_source)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/SBP)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/FBP)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/ART)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/MART)
FILE (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/data/SIRT)

CONFIGURE_FILE (data/hydrogen.tsv   data/hydrogen.tsv   COPYONLY)
CONFIGURE_FILE (data/carbon.tsv     data/carbon.tsv   COPYONLY)
CONFIGURE_FILE (data/nitrogen.tsv   data/nitrogen.tsv   COPYONLY)
CONFIGURE_FILE (data/oxygen.tsv     data/oxygen.tsv   COPYONLY)
CONFIGURE_FILE (data/sodium.tsv	data/sodium.tsv		COPYONLY)
CONFIGURE_FILE (data/magnesium.tsv	data/magnesium.tsv	COPYONLY)
CONFIGURE_FILE (data/phosphor.tsv	data/phosphor.tsv	COPYONLY)
CONFIGURE_FILE (data/sulfur.tsv	data/sulfur.tsv	COPYONLY)
CONFIGURE_FILE (data/clorine.tsv	data/clorine.tsv	COPYONLY)
CONFIGURE_FILE (data/argon.tsv	data/argon.tsv	COPYONLY)
CONFIGURE_FILE (data/potassium.tsv	data/potassium.tsv	COPYONLY)
CONFIGURE_FILE (data/calcium.tsv	data/calcium.tsv	COPYONLY)
CONFIGURE_FILE (data/titanium.tsv	data/titanium.tsv	COPYONLY)
CONFIGURE_FILE (data/copper.tsv	data/copper.tsv	COPYONLY)
CONFIGURE_FILE (data/zinc.tsv	data/zinc.tsv	COPYONLY)
CONFIGURE_FILE (data/silver.tsv	data/silver.tsv	COPYONLY)
CONFIGURE_FILE (data/tin.tsv	data/tin.tsv	COPYONLY)
CONFIGURE_FILE (data/iron.tsv	data/iron.tsv	COPYONLY)
CONFIGURE_FILE (data/iodine.tsv	data/iodine.tsv	COPYONLY)
CONFIGURE_FILE (data/Ti90Al6V4.tsv	data/Ti90Al6V4.tsv	COPYONLY)
CONFIGURE_FILE (data/H2O.tsv	data/H2O.tsv	COPYONLY)
CONFIGURE_FILE (data/SiC.tsv	data/SiC.tsv	COPYONLY)
CONFIGURE_FILE (runGnuplot.sh    runGnuplot.sh		COPYONLY)

CONFIGURE_FILE (data/bone.csv   data/bone.tsv   COPYONLY)
CONFIGURE_FILE (data/brain.csv  data/brain.tsv  COPYONLY)
CONFIGURE_FILE (data/gas.csv    data/gas.tsv    COPYONLY)
CONFIGURE_FILE (data/tissue.csv data/tissue.tsv COPYONLY)
CONFIGURE_FILE (data/water.csv  data/water.tsv  COPYONLY)
CONFIGURE_FILE (data/phantom_sinogram.mhd      data/phantom_sinogram.mhd  COPYONLY)
CONFIGURE_FILE (data/phantom_sinogram.raw      data/phantom_sinogram.raw  COPYONLY)
CONFIGURE_FILE (data/phantom_projections.mhd   data/phantom_projections.mhd  COPYONLY)
CONFIGURE_FILE (data/phantom_projections.raw   data/phantom_projections.raw  COPYONLY)
CONFIGURE_FILE (data/point_source/gate_simulation.mhd       data/point_source/gate_simulation.mhd  COPYONLY)
CONFIGURE_FILE (data/point_source/gate_simulation.raw       data/point_source/gate_simulation.raw  COPYONLY)
CONFIGURE_FILE (data/cube_source/gate_simulation.mhd       data/cube_source/gate_simulation.mhd  COPYONLY)
CONFIGURE_FILE (data/cube_source/gate_simulation.raw       data/cube_source/gate_simulation.raw  COPYONLY)
CONFIGURE_FILE (data/uncentered_source/gate_simulation.mhd       data/uncentered_source/gate_simulation.mhd  COPYONLY)
CONFIGURE_FILE (data/uncentered_source/gate_simulation.raw       data/uncentered_source/gate_simulation.raw  COPYONLY)

CONFIGURE_FILE (data/CT_reconstruction/ct_volume_in_mu_80keV.mhd   data/CT_reconstruction/ct_volume_in_mu_80keV.mhd  COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/ct_volume_in_mu_80keV.raw   data/CT_reconstruction/ct_volume_in_mu_80keV.raw  COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/projection_set.mhd          data/CT_reconstruction/projection_set.mhd         COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/projection_set.raw          data/CT_reconstruction/projection_set.raw         COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/sinogram.mhd                data/CT_reconstruction/sinogram.mhd               COPYONLY)
CONFIGURE_FILE (data/CT_reconstruction/sinogram.raw                data/CT_reconstruction/sinogram.raw               COPYONLY)


# add the install targets
install (DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/data             DESTINATION ${INSTALL_DIR}/test)

install (TARGETS TestDensity          DESTINATION ${INSTALL_DIR}/test)
install (TARGETS TestMassAttenuationCoefficients          DESTINATION ${INSTALL_DIR}/test)

install (TARGETS TestCubeWithoutIndex             DESTINATION ${INSTALL_DIR}/test)
install (TARGETS TestCubeWithIndex             DESTINATION ${INSTALL_DIR}/test)
install (TARGETS TestLBuffer             DESTINATION ${INSTALL_DIR}/test)
install (TARGETS TestBeerLambertLawMonoChromatic             DESTINATION ${INSTALL_DIR}/test)
install (TARGETS TestBeerLambertLawPolyChromatic             DESTINATION ${INSTALL_DIR}/test)
install (TARGETS TestSinogramFromMesh             DESTINATION ${INSTALL_DIR}/test)
#install (TARGETS TestSinogramFromVoxels             DESTINATION ${INSTALL_DIR}/test)
install (TARGETS TestPointSource             DESTINATION ${INSTALL_DIR}/test)
install (TARGETS TestCubeSource             DESTINATION ${INSTALL_DIR}/test)
install (TARGETS TestUncenteredSource             DESTINATION ${INSTALL_DIR}/test)
#install (TARGETS TestPhantomMonoChromatic             DESTINATION ${INSTALL_DIR}/test)
install (TARGETS TestRadonTransformFromMesh            DESTINATION ${INSTALL_DIR}/test)
install (TARGETS TestOrtho             DESTINATION ${INSTALL_DIR}/test)

IF (FFTW3_LIBRARIES)
    install (TARGETS TestFBP             DESTINATION ${INSTALL_DIR}/test)
ENDIF()
