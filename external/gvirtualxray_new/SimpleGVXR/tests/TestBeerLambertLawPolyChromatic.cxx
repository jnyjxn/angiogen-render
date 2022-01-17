/*

Copyright (c) 2019, Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
http://www.fpvidal.net/
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

3. Neither the name of the Bangor University nor the names of its contributors
may be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/


/**
********************************************************************************
*
*   @file       TestBeerLambertLawPolyChromatic.cxx
*
*   @brief      Unit testing: Beer-Lambert Law, Polychromatism Case,
*   			the unit test corresponds to validation_05,
*   			last date of check: Passed test on 08 May 2019.
*
*   @version    1.0
*
*   @date       08/05/2019
*   @author     Dr Franck P. Vidal
*
*   @section    License
*               BSD 3-Clause License.
*
*               For details on use and redistribution please refer
*               to http://opensource.org/licenses/BSD-3-Clause
*
*   @section    Copyright
*               (c) by Dr Franck P. Vidal (franck.p.vidal@fpvidal.net),
*               http://www.fpvidal.net/, Apr 2019, 2019, version 1.0,
*               BSD 3-Clause License
*
********************************************************************************
*/


//******************************************************************************
//  Include
//******************************************************************************
#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#include <cmath>
#include "gtest/gtest.h"

#ifndef __SimpleGVXR_h
#include "SimpleGVXR.h"
#endif


//******************************************************************************
//  Global variables
//******************************************************************************
int g_main_window_width(1280 / 2.0);
int g_main_window_height(800 / 2.0);

double g_incident_energy_in_keV[] = {100.0, 200.0, 300.0};
double g_number_of_photon_in_energy_channel[] = { 10.0, 20.0, 10.0};

const double g_scaling_factor(2.0);
double g_resolution_in_mm = (0.15 * g_scaling_factor); // 1mm per pixel
int g_number_of_pixels[] = {int(600 / g_scaling_factor), 1};
double g_source_position_in_cm[]   = {0.0, 0.0, -10.0};
double g_detector_position_in_cm[] = {0.0, 0.0,  10.0};
double g_detector_up_vector[]      = {0.0, 1.0,  0.0};

double g_literature_result_in_keV(0.0);
double g_expected_result_in_keV(0.0);


//******************************************************************************
//  Function declaration
//******************************************************************************
void loadSTLFile();
void loadDetector();
void loadSource();


//---------------------------------------------------------------------
TEST (TestBeerLambertLawPolyChromatic, TestBeerLambertLawPolyChromatic)
//---------------------------------------------------------------------
{
    // Make sure the detector has an odd number of columns
    if ((g_number_of_pixels[0] % 2) == 0)
        {
        g_number_of_pixels[0] = g_number_of_pixels[0] + 1;
        }

    // Make sure the detector has an odd number of rows
    if ((g_number_of_pixels[1] % 2) == 0)
        {
        g_number_of_pixels[1] = g_number_of_pixels[1] + 1;
    }

    // Create a windowed mode window and its OpenGL context
#ifdef HAS_EGL
    createWindow(-1, 0, "EGL");
#else
    createWindow();
#endif
    setWindowSize(g_main_window_width, g_main_window_height);

    // Load the data
    loadDetector();
    loadSource();

    // Add the geometry to the X-ray renderer
    loadSTLFile();
    addPolygonMeshAsInnerSurface("inside");
    addPolygonMeshAsOuterSurface("outside");

    // Compute the X-ray image
    std::vector<std::vector<float> > x_ray_image = computeXRayImage();

    std::vector<float> row = x_ray_image[x_ray_image.size() / 2];
    float central_pixel = row[row.size() / 2];

    float simulated_result_in_keV = central_pixel / getUnitOfEnergy("keV");

    std::cout << "On GPU, with full precision, the energy (in keV) transmitted orthogonally throw the middle of cube and cylinder actually is:" << std::endl;
    std::cout << "\t\t           = " << /*std::scientific <<*/ simulated_result_in_keV << " keV" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_GPU_full_precision| keV" << std::endl;
    std::cout << "                     : |" << /*std::scientific <<*/ g_expected_result_in_keV << " - " << simulated_result_in_keV << "| keV" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(g_expected_result_in_keV - simulated_result_in_keV) << " keV" << std::endl << std::endl;

    std::cout << "The relative error is: |I_expected - I_GPU_half_precision| / I_expected" << std::endl;
    std::cout << "                     : " << std::fixed << 100.0 * std::abs(g_expected_result_in_keV - simulated_result_in_keV) / g_expected_result_in_keV << " \%" << std::endl;

    destroyAllWindows();

    ASSERT_NEAR(1, simulated_result_in_keV / g_expected_result_in_keV, 0.01);
}


//----------------
void loadSTLFile()
//----------------
{
	// Set the geometry
	makeCube("outside",
	        3.0,
	        "cm",
	        "root");

	makeCylinder("inside",
			20,
	        3.0,
	        1.0,
			"cm",
	        "root");

    // HU values
    double HU_water(0.0);
    double HU_bone(1329.885683);
    double HU_soft_tissue(51.714752);

    setHU( "inside", HU_bone);
    setHU("outside", HU_soft_tissue);

    std::cout << "We run the simulation as follows:" << std::endl;
    std::cout << "- A cube, which has edge length of 3 cm, is made of soft tissue (HU = " << std::floor(HU_soft_tissue + 0.5) << "). " << std::endl;
    std::cout << "- Inside, a cylinder is inserted. " <<
            "It is made of bone (HU = " << std::floor(HU_bone + 0.5) << "), its height is 3 cm and its diameter is 2 cm." << std::endl;
    std::cout << "- We consider the energy of the incident beam as follows: " << std::endl;

    double total_energy(0);
    for (unsigned int i = 0; i < sizeof(g_incident_energy_in_keV)/ sizeof(double); ++i)
    {
    	std::cout << g_number_of_photon_in_energy_channel[i] << " photons of " <<  g_incident_energy_in_keV[i] << " keV (i.e. " << g_incident_energy_in_keV[i] << " keV). " << std::endl;
		total_energy += g_number_of_photon_in_energy_channel[i] * g_incident_energy_in_keV[i];
    }
    std::cout << std::endl;

    std::cout << "Total input energy = " << total_energy << std::endl << std::endl;

    std::cout << "From the literature, we know:" << std::endl;
    std::cout.precision(4);

    for (unsigned int i = 0; i < sizeof(g_incident_energy_in_keV)/ sizeof(double); ++i)
    {
    }

    std::cout <<
        "HU = "     << std::fixed      << HU_bone << "\t" <<
        "rho = "    << std::scientific << (getDensityFromHU(HU_bone)) << " g/cm3,\t" <<
        "energy = " << std::fixed      << g_incident_energy_in_keV[0] << " keV,\t" <<
        "mu/rho = " << std::scientific << 1.855E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << (getDensityFromHU(HU_bone)) * 1.855E-01 << " cm-1" << std::endl;

    std::cout <<
        "HU =   "   << std::fixed      << HU_soft_tissue << "\t" <<
        "rho = "    << std::scientific << (getDensityFromHU(HU_soft_tissue)) << " g/cm3,\t" <<
        "energy = " << std::fixed      << g_incident_energy_in_keV[0] << " keV,\t" <<
        "mu/rho = " << std::scientific << 1.693E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << (getDensityFromHU(HU_soft_tissue)) * 1.693E-01 << " cm-1" << std::endl << std::endl;

    std::cout <<
        "HU = "     << std::fixed      << HU_bone << "\t" <<
        "rho = "    << std::scientific << (getDensityFromHU(HU_bone)) << " g/cm3,\t" <<
        "energy = " << std::fixed      << g_incident_energy_in_keV[1]  << " keV,\t" <<
        "mu/rho = " << std::scientific << 1.309E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << (getDensityFromHU(HU_bone)) * 1.309E-01 << " cm-1" << std::endl;

    std::cout <<
        "HU =   "     << std::fixed    << HU_soft_tissue << "\t" <<
        "rho = "    << std::scientific << (getDensityFromHU(HU_soft_tissue)) << " g/cm3,\t" <<
        "energy = " << std::fixed      << g_incident_energy_in_keV[1]  << " keV,\t" <<
        "mu/rho = " << std::scientific << 1.358E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << (getDensityFromHU(HU_soft_tissue)) * 1.358E-01 << " cm-1" << std::endl << std::endl;

    std::cout <<
        "HU = "     << std::fixed      << HU_bone << "\t" <<
        "rho = "    << std::scientific << (getDensityFromHU(HU_bone)) << " g/cm3,\t" <<
        "energy = " << std::fixed      << g_incident_energy_in_keV[2]  << " keV,\t" <<
        "mu/rho = " << std::scientific << 1.113E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << (getDensityFromHU(HU_bone)) * 1.113E-01 << " cm-1" << std::endl;

    std::cout <<
        "HU =   "   << std::fixed      << HU_soft_tissue << "\t" <<
        "rho = "    << std::scientific << (getDensityFromHU(HU_soft_tissue)) << " g/cm3,\t" <<
        "energy = " << std::fixed      << g_incident_energy_in_keV[2]  << " keV,\t" <<
        "mu/rho = " << std::scientific << 1.175E-01 << " cm2/g,\t" <<
        "mu = "     << std::scientific << (getDensityFromHU(HU_soft_tissue)) * 1.175E-01 << " cm-1" << std::endl << std::endl;

    //g_literature_result_in_keV = g_incident_energy * exp(-(4.280e-01 * 2 + 1.932e-01 * 1));

    g_literature_result_in_keV = 0.0;
    g_literature_result_in_keV += (g_number_of_photon_in_energy_channel[0] * g_incident_energy_in_keV[0]) * exp(-(((getDensityFromHU(HU_bone)) * 1.855E-01) * 2.0 + ((getDensityFromHU(HU_soft_tissue)) * 1.693E-01) * 1.0));
    g_literature_result_in_keV += (g_number_of_photon_in_energy_channel[1] * g_incident_energy_in_keV[1]) * exp(-(((getDensityFromHU(HU_bone)) * 1.309E-01) * 2.0 + ((getDensityFromHU(HU_soft_tissue)) * 1.358E-01) * 1.0));
    g_literature_result_in_keV += (g_number_of_photon_in_energy_channel[2] * g_incident_energy_in_keV[2]) * exp(-(((getDensityFromHU(HU_bone)) * 1.113E-01) * 2.0 + ((getDensityFromHU(HU_soft_tissue)) * 1.175E-01) * 1.0));

    std::cout.precision(3);
    std::cout << "The energy (in keV) transmitted orthogonally throw the middle of cube and cylinder should be:" << std::endl;
    std::cout << "\t\tI_reference = I_0 * exp(-Sum(mu * x))" << std::endl;
    std::cout << "\t\t            = " << std::scientific << g_literature_result_in_keV << " keV" << std::endl << std::endl;

    std::cout << "To obtain linear attenuation coefficients (mu values) in our implementation, densities (rho values) and mass attenuation coefficients (mu/rho values) are estimated as follows:" << std::endl;

    std::cout.precision(4);
    for (unsigned int i = 0; i < sizeof(g_incident_energy_in_keV)/ sizeof(double); ++i)
        {
        std::cout <<
            "HU = "     << std::fixed      << HU_bone << "\t" <<
            "rho = "    << std::scientific << getDensityFromHU(HU_bone) << " g/cm3,\t" <<
            "energy = " << std::fixed      << g_incident_energy_in_keV[i] << " keV,\t" <<
            "mu/rho = " << std::scientific << getMassAttenuationFromHU(HU_bone, g_incident_energy_in_keV[i], "keV") << " cm2/g,\t" <<
            "mu = "     << std::scientific << getMuFromHU(HU_bone, g_incident_energy_in_keV[i], "keV") << " cm-1" << std::endl;

        std::cout <<
            "HU =   "   << std::fixed      << HU_soft_tissue << "\t" <<
            "rho = "    << std::scientific << getDensityFromHU(HU_soft_tissue) << " g/cm3,\t" <<
            "energy = " << std::fixed      << g_incident_energy_in_keV[i] << " keV,\t" <<
            "mu/rho = " << std::scientific << getMassAttenuationFromHU(HU_soft_tissue, g_incident_energy_in_keV[i], "keV") << " cm2/g,\t" <<
            "mu = "     << std::scientific << getMuFromHU(HU_soft_tissue, g_incident_energy_in_keV[i], "keV") << " cm-1" << std::endl << std::endl;
        }

    g_expected_result_in_keV = 0.0;
    for (unsigned int i = 0; i < sizeof(g_incident_energy_in_keV)/ sizeof(double); ++i)
        {
        double photon_number(g_number_of_photon_in_energy_channel[i]);
        double incident_energy(g_incident_energy_in_keV[i]);

        g_expected_result_in_keV += (photon_number * incident_energy) * exp(-(getMuFromHU(HU_bone, incident_energy, "keV") * 2 + getMuFromHU(HU_soft_tissue, incident_energy, "keV") * 1));
        }

    std::cout << "The energy (in keV) transmitted orthogonally throw the middle of cube and cylinder should be:" << std::endl;
    std::cout << "\t\tI_expected = I_0 * exp(-Sum(mu * x))" << std::endl;
    std::cout << "\t\tI_expected = " << "(" << std::fixed << g_incident_energy_in_keV[0] << " x " << g_number_of_photon_in_energy_channel[0] << ") x exp(-(" << getMuFromHU(HU_bone, g_incident_energy_in_keV[0], "keV") << " x 2 + " << getMuFromHU(HU_soft_tissue, g_incident_energy_in_keV[0], "keV") << " x 1)) +" << std::endl;
    std::cout << "\t\t             " << "(" << std::fixed << g_incident_energy_in_keV[1] << " x " << g_number_of_photon_in_energy_channel[1] << ") x exp(-(" << getMuFromHU(HU_bone, g_incident_energy_in_keV[1], "keV") << " x 2 + " << getMuFromHU(HU_soft_tissue, g_incident_energy_in_keV[1], "keV") << " x 1)) +" << std::endl;
    std::cout << "\t\t             " << "(" << std::fixed << g_incident_energy_in_keV[2] << " x " << g_number_of_photon_in_energy_channel[2] << ") x exp(-(" << getMuFromHU(HU_bone, g_incident_energy_in_keV[2], "keV") << " x 2 + " << getMuFromHU(HU_soft_tissue, g_incident_energy_in_keV[2], "keV") << " x 1))" << std::endl;
    std::cout.precision(3);
    std::cout << "\t\tI_expected = " << (g_incident_energy_in_keV[0] * g_number_of_photon_in_energy_channel[0]) * exp(-(getMuFromHU(HU_bone, g_incident_energy_in_keV[0], "keV") * 2.0 + getMuFromHU(HU_soft_tissue, g_incident_energy_in_keV[0], "keV") * 1.0)) << " +" << std::endl;
    std::cout << "\t\t             " << (g_incident_energy_in_keV[1] * g_number_of_photon_in_energy_channel[1]) * exp(-(getMuFromHU(HU_bone, g_incident_energy_in_keV[1], "keV") * 2.0 + getMuFromHU(HU_soft_tissue, g_incident_energy_in_keV[1], "keV") * 1.0)) << " +" << std::endl;
    std::cout << "\t\t             " << (g_incident_energy_in_keV[2] * g_number_of_photon_in_energy_channel[2]) * exp(-(getMuFromHU(HU_bone, g_incident_energy_in_keV[2], "keV") * 2.0 + getMuFromHU(HU_soft_tissue, g_incident_energy_in_keV[2], "keV") * 1.0))         << std::endl;
    std::cout << "\t\t           = " << /*std::scientific*/ std::fixed << g_expected_result_in_keV << " keV" << std::endl << std::endl;

    std::cout << "The absolute error is: |I_expected - I_reference| keV" << std::endl;
    std::cout << "                     : |" << /*std::scientific <<*/ g_expected_result_in_keV << " - " << g_literature_result_in_keV << "| keV" << std::endl;
    std::cout << "                     : " << std::fixed << std::abs(g_expected_result_in_keV - g_literature_result_in_keV) << " keV" << std::endl << std::endl;
}


//-----------------
void loadDetector()
//-----------------
{
	setDetectorPixelSize(g_resolution_in_mm, g_resolution_in_mm , "mm");
    setDetectorNumberOfPixels(g_number_of_pixels[0], g_number_of_pixels[1]);
    setDetectorPosition(g_detector_position_in_cm[0], g_detector_position_in_cm[1], g_detector_position_in_cm[2], "cm");
    setDetectorUpVector(g_detector_up_vector[0], g_detector_up_vector[1], g_detector_up_vector[2]);
}


//---------------
void loadSource()
//---------------
{
    // Set the energy
    for (unsigned int i = 0; i < sizeof(g_incident_energy_in_keV)/ sizeof(double); ++i)
    {
    	addEnergyBinToSpectrum(g_incident_energy_in_keV[i], "keV", g_number_of_photon_in_energy_channel[i]);
    }

    // Set the source position
	setSourcePosition(g_source_position_in_cm[0], g_source_position_in_cm[1], g_source_position_in_cm[2], "cm");

    // Use a parallel source
	useParallelBeam();
}
