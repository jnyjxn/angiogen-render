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
*   @file       TestMassAttenuationCoefficients.cxx
*
*   @brief      Unit testing: mass attenuation coefficients,
*   			the unit test corresponds to validation_01,
*   			last date of check: Passed test on 01 Apr 2019.
*
*
*   @version    1.0
*
*   @date       01/04/2019
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
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <numeric>

#include "gtest/gtest.h"

#ifdef HAS_GLEW
#include <GL/glew.h>
#endif

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif

#ifndef __InvalidInternalFormalException_h
#include "gVirtualXRay/InvalidInternalFormalException.h"
#endif

#ifndef __PhotonCrossSection_h
#include "gVirtualXRay/PhotonCrossSection.h"
#endif

#ifndef __InvalidImageSizeException_h
#include "gVirtualXRay/InvalidImageSizeException.h"
#endif

#ifndef __NoDetectorException_h
#include "gVirtualXRay/NoDetectorException.h"
#endif

#ifndef __NoBeamException_h
#include "gVirtualXRay/NoBeamException.h"
#endif


//******************************************************************************
//  Name space
//******************************************************************************
using namespace gVirtualXRay;
//using namespace std;


//******************************************************************************
//  Function declaration
//******************************************************************************
void runTestHU(const char* aFileName, short anHounsfieldValue, const char* aGnuplotScriptFileName = 0);
void runTestElement(const char* aFileName, const char* anElementSymbol, const char* aGnuplotScriptFileName = 0);
void runTestMixture(const char* aFileName, const char* aMixture, const char* aGnuplotScriptFileName = 0);
void runTestCompound(const char* aFileName, const char* aMixture, const char* aGnuplotScriptFileName = 0);
double getSum(const std::vector<double>& aVector);
double getAverage(const std::vector<double>& aVector);
double getVariance(const std::vector<double>& aVector);
double getStdDev(const std::vector<double>& aVector);
double getNCC(const std::vector<double>& aVector1, const std::vector<double>& aVector2);
double maxRelativeError(const std::vector<double>& aVector1, const std::vector<double>& aVector2);
void buildGnuplotHeader(std::ofstream& anOutputStream, const char* aTest);


//******************************************************************************
//  Const
//******************************************************************************
const short HU_bone(1330);
const short HU_brain(37);
const short HU_gas(-998);
const short HU_soft_tissue(52);
const short HU_water(0);


//-----------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestTi90Al6V4MassAttenuationCoefficients)
//-----------------------------------------------------------------------------
{
    runTestMixture("data/Ti90Al6V4.tsv", "Ti90Al6V4", "Ti90Al6V4");
}


//----------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestHydrogenMassAttenuationCoefficients)
//----------------------------------------------------------------------------
{
    runTestElement("data/hydrogen.tsv", "hydrogen", "hydrogen");
    runTestElement("data/hydrogen.tsv", "HYDROGEN");
    runTestElement("data/hydrogen.tsv", "H");
}


//--------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestCarbonMassAttenuationCoefficients)
//--------------------------------------------------------------------------
{
    runTestElement("data/carbon.tsv", "carbon", "carbon");
    runTestElement("data/carbon.tsv", "CARBON");
    runTestElement("data/carbon.tsv", "C");
}


//----------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestNitrogenMassAttenuationCoefficients)
//----------------------------------------------------------------------------
{
    runTestElement("data/nitrogen.tsv", "nitrogen", "nitrogen");
    runTestElement("data/nitrogen.tsv", "NITROGEN");
    runTestElement("data/nitrogen.tsv", "N");
}


//--------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestOxygenMassAttenuationCoefficients)
//--------------------------------------------------------------------------
{
    runTestElement("data/oxygen.tsv", "oxygen", "oxygen");
    runTestElement("data/oxygen.tsv", "OXYGEN");
    runTestElement("data/oxygen.tsv", "O");
}


//--------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestSodiumMassAttenuationCoefficients)
//--------------------------------------------------------------------------
{
    runTestElement("data/sodium.tsv", "sodium", "sodium");
    runTestElement("data/sodium.tsv", "SODIUM");
    runTestElement("data/sodium.tsv", "Na");
}


//-----------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestMagnesiumMassAttenuationCoefficients)
//-----------------------------------------------------------------------------
{
    runTestElement("data/magnesium.tsv", "magnesium", "magnesium");
    runTestElement("data/magnesium.tsv", "MAGNESIUM");
    runTestElement("data/magnesium.tsv", "Mg");
}


//----------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestPhosphorMassAttenuationCoefficients)
//----------------------------------------------------------------------------
{
    runTestElement("data/phosphor.tsv", "phosphor", "phosphor");
    runTestElement("data/phosphor.tsv", "PHOSPHOR");
    runTestElement("data/phosphor.tsv", "P");
}


//--------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestSulfurMassAttenuationCoefficients)
//--------------------------------------------------------------------------
{
    runTestElement("data/sulfur.tsv", "sulfur", "sulfur");
    runTestElement("data/sulfur.tsv", "SULFUR");
    runTestElement("data/sulfur.tsv", "S");
}

//---------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestClorineMassAttenuationCoefficients)
//---------------------------------------------------------------------------
{
    runTestElement("data/clorine.tsv", "chlorine", "chlorine");
    runTestElement("data/clorine.tsv", "CHLORINE");
    runTestElement("data/clorine.tsv", "Cl");
}


//-------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestArgonMassAttenuationCoefficients)
//-------------------------------------------------------------------------
{
    runTestElement("data/argon.tsv", "argon", "argon");
    runTestElement("data/argon.tsv", "ARGON");
    runTestElement("data/argon.tsv", "Ar");
}


//-----------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestPotassiumMassAttenuationCoefficients)
//-----------------------------------------------------------------------------
{
    runTestElement("data/potassium.tsv", "potassium", "potassium");
    runTestElement("data/potassium.tsv", "POTASSIUM");
    runTestElement("data/potassium.tsv", "K");
}


//---------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestCalciumMassAttenuationCoefficients)
//---------------------------------------------------------------------------
{
    runTestElement("data/calcium.tsv", "calcium", "calcium");
    runTestElement("data/calcium.tsv", "CALCIUM");
    runTestElement("data/calcium.tsv", "Ca");
}


//----------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestTitatiumMassAttenuationCoefficients)
//----------------------------------------------------------------------------
{
    runTestElement("data/titanium.tsv", "titanium", "titanium");
    runTestElement("data/titanium.tsv", "TITANIUM");
    runTestElement("data/titanium.tsv", "Ti");
}

//--------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestCopperMassAttenuationCoefficients)
//--------------------------------------------------------------------------
{
    runTestElement("data/copper.tsv", "copper", "copper");
    runTestElement("data/copper.tsv", "COPPER");
    runTestElement("data/copper.tsv", "Cu");
}


//------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestZincMassAttenuationCoefficients)
//------------------------------------------------------------------------
{
    runTestElement("data/zinc.tsv", "zinc", "zinc");
    runTestElement("data/zinc.tsv", "ZINC");
    runTestElement("data/zinc.tsv", "Zn");
}

//--------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestSilverMassAttenuationCoefficients)
//--------------------------------------------------------------------------
{
    runTestElement("data/silver.tsv", "silver", "silver");
    runTestElement("data/silver.tsv", "SILVER");
    runTestElement("data/silver.tsv", "Ag");
}


//-----------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestTinMassAttenuationCoefficients)
//-----------------------------------------------------------------------
{
    runTestElement("data/tin.tsv", "tin", "tin");
    runTestElement("data/tin.tsv", "TIN");
    runTestElement("data/tin.tsv", "Sn");
}


//------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestIronMassAttenuationCoefficients)
//------------------------------------------------------------------------
{
    runTestElement("data/iron.tsv", "iron", "iron");
    runTestElement("data/iron.tsv", "IRON");
    runTestElement("data/iron.tsv", "Fe");
}


//--------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestIodineMassAttenuationCoefficients)
//--------------------------------------------------------------------------
{
    runTestElement("data/iodine.tsv", "iodine", "iodine");
    runTestElement("data/iodine.tsv", "IODINE");
    runTestElement("data/iodine.tsv", "I");
}


//------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestBoneMassAttenuationCoefficients)
//------------------------------------------------------------------------
{
    runTestHU("data/bone.tsv", HU_bone, "bone");
}


//-------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestBrainMassAttenuationCoefficients)
//-------------------------------------------------------------------------
{
    runTestHU("data/brain.tsv", HU_brain, "brain");
}

/*
//-----------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestGasMassAttenuationCoefficients)
//-----------------------------------------------------------------------
{
    runTestHU("data/gas.tsv", HU_gas, "gas");
}
*/

//--------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestTissueMassAttenuationCoefficients)
//--------------------------------------------------------------------------
{
    runTestHU("data/tissue.tsv", HU_soft_tissue, "soft_tissue");
}

/*
//-------------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestWaterMassAttenuationCoefficients)
//-------------------------------------------------------------------------
{
    runTestHU("data/water.tsv", HU_water, "water");
}*/

/*
//-----------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestH2OMassAttenuationCoefficients)
//-----------------------------------------------------------------------
{
    runTest("data/water.tsv", "H2O");
}*/


//----------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestH2OMassAttenuationCoefficients)
//----------------------------------------------------------------------
{
    runTestCompound("data/H2O.tsv", "H2O", "H2O");
}


//-----------------------------------------------------------------------
TEST(TestMassAttenuationCoefficients, TestSiCMassAttenuationCoefficients)
//-----------------------------------------------------------------------
{
    runTestCompound("data/SiC.tsv", "SiC", "SiC");
}


//------------------------------------------------
void runTestHU(const char* aFileName,
               short anHounsfieldValue,
			   const char* aGnuplotScriptFileName)
//------------------------------------------------
{
    std::cout << "***********************" << std::endl;
    std::cout << "HU: " << anHounsfieldValue << std::endl;
    std::cout << "***********************" << std::endl;

    // Open the files
    std::ifstream reference_data_stream(aFileName);
    std::ofstream gnuplot_stream;
    std::ofstream temp_stream;
    std::string temp_file_name;

    if (aGnuplotScriptFileName) temp_file_name = std::string(aGnuplotScriptFileName) + "_tmp.tsv";

    if (aGnuplotScriptFileName)
	{
    	gnuplot_stream.open((std::string(aGnuplotScriptFileName) + ".plt").data());
    	temp_stream.open(temp_file_name.data());
	}

    // The file is not open
    if (!reference_data_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }

    if (aGnuplotScriptFileName && !gnuplot_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aGnuplotScriptFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }
    else if (aGnuplotScriptFileName && !temp_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aGnuplotScriptFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }
    else
    {
    	temp_stream << "Photon Energy\tTot. w/ Coherent" << std::endl;

    	buildGnuplotHeader(gnuplot_stream, aGnuplotScriptFileName);
    	gnuplot_stream << "plot \'" << aFileName       << "\' using 1:2 w l title \'XCOM\' lw 6, " <<
    			               "\'" << temp_file_name  << "\' using 1:2 w l title \'gVirtualXRay\' lw 3" << std::endl;
    }

    // Initialise the photon cross-section
    PhotonCrossSection photon_cross_section;
    photon_cross_section.useHU(anHounsfieldValue);

    std::vector<double> reference_vector;
    std::vector<double> test_vector;
    std::vector<double> energy_set;

    double current_energy = 0;
    double last_energy    = 0;
    double mass_attenuation_coefficient;
    double temp;

    while (reference_data_stream.good())
    {
        reference_data_stream >> current_energy >> mass_attenuation_coefficient >> temp;
        current_energy *= MeV;

        if (current_energy == last_energy)
        {
        	current_energy += 4.0 * eV;

        	test_vector.pop_back();
			energy_set.pop_back();

			test_vector.push_back(photon_cross_section.getMassAttenuationCoefficient(last_energy - 4.0 * eV)  / (cm2 / g));
			energy_set.push_back(last_energy - 4.0 * eV);
        }

        if (current_energy != last_energy)
        {
        	last_energy = current_energy;

			reference_vector.push_back(mass_attenuation_coefficient);
			test_vector.push_back(photon_cross_section.getMassAttenuationCoefficient(current_energy)  / (cm2 / g));
			energy_set.push_back(current_energy);
        }
    }

	if (temp_stream.is_open())
	{
		for (unsigned int i = 0; i < test_vector.size(); ++i)
		{
			temp_stream << energy_set[i] << "\t" << test_vector[i] << std::endl;
		}
	}

	ASSERT_NEAR(getNCC(reference_vector, test_vector), 1.0, 1e-3);
	ASSERT_NEAR(maxRelativeError(reference_vector, test_vector), 0.0, 0.1);
}


//-----------------------------------------------------
void runTestElement(const char* aFileName,
		            const char* anElementSymbol,
		            const char* aGnuplotScriptFileName)
//-----------------------------------------------------
{
    std::cout << "***********************" << std::endl;
    std::cout << anElementSymbol << std::endl;
    std::cout << "***********************" << std::endl;

    // Open the files
    std::ifstream reference_data_stream(aFileName);
    std::ofstream gnuplot_stream;
    std::ofstream temp_stream;
    std::string temp_file_name;

    if (aGnuplotScriptFileName) temp_file_name = std::string(aGnuplotScriptFileName) + "_tmp.tsv";

    if (aGnuplotScriptFileName)
	{
    	gnuplot_stream.open((std::string(aGnuplotScriptFileName) + ".plt").data());
    	temp_stream.open(temp_file_name.data());
	}

    // The file is not open
    if (!reference_data_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }

    if (aGnuplotScriptFileName && !gnuplot_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aGnuplotScriptFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }
    else if (aGnuplotScriptFileName && !temp_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aGnuplotScriptFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }
    else if (temp_stream.is_open())
    {
    	temp_stream << "Photon Energy\tTot. w/ Coherent" << std::endl;

    	buildGnuplotHeader(gnuplot_stream, aGnuplotScriptFileName);
    	gnuplot_stream << "plot \'" << aFileName       << "\' using 1:2 w l title \'XCOM\' lw 6, " <<
    			               "\'" << temp_file_name  << "\' using 1:2 w l title \'gVirtualXRay\' lw 3" << std::endl;
    }

    // Initialise the photon cross-section
    PhotonCrossSection photon_cross_section;
    photon_cross_section.useElement(anElementSymbol);

    std::vector<double> reference_vector;
    std::vector<double> test_vector;
    std::vector<double> energy_set;

    double current_energy = 0;
    double last_energy    = 0;
    double mass_attenuation_coefficient;

    // Skip header
    char line[1024];
    reference_data_stream.getline(line, 1024);

    while (reference_data_stream.good())
    {
        reference_data_stream >> current_energy >> mass_attenuation_coefficient;
        current_energy *= MeV;

        if (current_energy == last_energy)
        {
        	current_energy += 4.0 * eV;

        	test_vector.pop_back();
			energy_set.pop_back();

			test_vector.push_back(photon_cross_section.getMassAttenuationCoefficient(last_energy - 4.0 * eV)  / (cm2 / g));
			energy_set.push_back(last_energy - 4.0 * eV);
        }

        if (current_energy != last_energy)
        {
        	last_energy = current_energy;

			reference_vector.push_back(mass_attenuation_coefficient);
			test_vector.push_back(photon_cross_section.getMassAttenuationCoefficient(current_energy)  / (cm2 / g));
			energy_set.push_back(current_energy);
        }
    }

	if (temp_stream.is_open())
	{
		for (unsigned int i = 0; i < test_vector.size(); ++i)
		{
			temp_stream << energy_set[i] << "\t" << test_vector[i] << std::endl;
		}
	}

	ASSERT_NEAR(getNCC(reference_vector, test_vector), 1.0, 1e-3);
	ASSERT_NEAR(maxRelativeError(reference_vector, test_vector), 0.0, 0.1);
}


//-----------------------------------------------------
void runTestMixture(const char* aFileName,
		            const char* aMixture,
					const char* aGnuplotScriptFileName)
//-----------------------------------------------------
{
    std::cout << "***********************" << std::endl;
    std::cout << aMixture << std::endl;
    std::cout << "***********************" << std::endl;

    // Open the files
    std::ifstream reference_data_stream(aFileName);
    std::ofstream gnuplot_stream;
    std::ofstream temp_stream;
    std::string temp_file_name;

    if (aGnuplotScriptFileName) temp_file_name = std::string(aGnuplotScriptFileName) + "_tmp.tsv";

    if (aGnuplotScriptFileName)
	{
    	gnuplot_stream.open((std::string(aGnuplotScriptFileName) + ".plt").data());
    	temp_stream.open(temp_file_name.data());
	}

    // The file is not open
    if (!reference_data_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }

    if (aGnuplotScriptFileName && !gnuplot_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aGnuplotScriptFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }
    else if (aGnuplotScriptFileName && !temp_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aGnuplotScriptFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }
    else
    {
    	temp_stream << "Photon Energy\tTot. w/ Coherent" << std::endl;

    	buildGnuplotHeader(gnuplot_stream, aGnuplotScriptFileName);
    	gnuplot_stream << "plot \'" << aFileName       << "\' using 1:2 w l title \'XCOM\' lw 6, " <<
    			               "\'" << temp_file_name  << "\' using 1:2 w l title \'gVirtualXRay\' lw 3" << std::endl;
    }

    // Initialise the photon cross-section
    PhotonCrossSection photon_cross_section;
    photon_cross_section.useMixture(aMixture);

    std::vector<double> reference_vector;
    std::vector<double> test_vector;
    std::vector<double> energy_set;

    double current_energy = 0;
    double last_energy    = 0;
    double mass_attenuation_coefficient;

    // Skip header
    char line[1024];
    reference_data_stream.getline(line, 1024);

    while (reference_data_stream.good())
    {
        reference_data_stream >> current_energy >> mass_attenuation_coefficient;
        current_energy *= MeV;

        if (current_energy == last_energy)
        {
        	current_energy += 4.0 * eV;

        	test_vector.pop_back();
			energy_set.pop_back();

			test_vector.push_back(photon_cross_section.getMassAttenuationCoefficient(last_energy - 4.0 * eV)  / (cm2 / g));
			energy_set.push_back(last_energy - 4.0 * eV);
        }

        if (current_energy != last_energy)
        {
        	last_energy = current_energy;

			reference_vector.push_back(mass_attenuation_coefficient);
			test_vector.push_back(photon_cross_section.getMassAttenuationCoefficient(current_energy)  / (cm2 / g));
			energy_set.push_back(current_energy);
        }
    }

	if (temp_stream.is_open())
	{
		for (unsigned int i = 0; i < test_vector.size(); ++i)
		{
			temp_stream << energy_set[i] << "\t" << test_vector[i] << std::endl;
		}
	}

	ASSERT_NEAR(getNCC(reference_vector, test_vector), 1.0, 1e-3);
	ASSERT_NEAR(maxRelativeError(reference_vector, test_vector), 0.0, 0.1);
}


//------------------------------------------------------
void runTestCompound(const char* aFileName,
		             const char* aCompound,
					 const char* aGnuplotScriptFileName)
//------------------------------------------------------
{
    std::cout << "***********************" << std::endl;
    std::cout << aCompound << std::endl;
    std::cout << "***********************" << std::endl;

    // Open the files
    std::ifstream reference_data_stream(aFileName);
    std::ofstream gnuplot_stream;
    std::ofstream temp_stream;
    std::string temp_file_name;

    if (aGnuplotScriptFileName) temp_file_name = std::string(aGnuplotScriptFileName) + "_tmp.tsv";

    if (aGnuplotScriptFileName)
	{
    	gnuplot_stream.open((std::string(aGnuplotScriptFileName) + ".plt").data());
    	temp_stream.open(temp_file_name.data());
	}

    // The file is not open
    if (!reference_data_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }

    if (aGnuplotScriptFileName && !gnuplot_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aGnuplotScriptFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }
    else if (aGnuplotScriptFileName && !temp_stream.is_open())
    {
        std::string error_message;
        error_message += "\"";
        error_message += aGnuplotScriptFileName;
        error_message += "\" does not exist";

        FAIL() << error_message;
    }
    else
    {
    	temp_stream << "Photon Energy\tTot. w/ Coherent" << std::endl;

    	buildGnuplotHeader(gnuplot_stream, aGnuplotScriptFileName);
    	gnuplot_stream << "plot \'" << aFileName       << "\' using 1:2 w l title \'XCOM\' lw 6, " <<
    			               "\'" << temp_file_name  << "\' using 1:2 w l title \'gVirtualXRay\' lw 3" << std::endl;
    }

    // Initialise the photon cross-section
    PhotonCrossSection photon_cross_section;
    photon_cross_section.useCompound(aCompound);

    std::vector<double> reference_vector;
    std::vector<double> test_vector;
    std::vector<double> energy_set;

    double current_energy = 0;
    double last_energy    = 0;
    double mass_attenuation_coefficient;

    // Skip header
    char line[1024];
    reference_data_stream.getline(line, 1024);

    while (reference_data_stream.good())
    {
        reference_data_stream >> current_energy >> mass_attenuation_coefficient;
        current_energy *= MeV;

        if (current_energy == last_energy)
        {
        	current_energy += 4.0 * eV;

        	test_vector.pop_back();
			energy_set.pop_back();

			test_vector.push_back(photon_cross_section.getMassAttenuationCoefficient(last_energy - 4.0 * eV)  / (cm2 / g));
			energy_set.push_back(last_energy - 4.0 * eV);
        }

        if (current_energy != last_energy)
        {
        	last_energy = current_energy;

			reference_vector.push_back(mass_attenuation_coefficient);
			test_vector.push_back(photon_cross_section.getMassAttenuationCoefficient(current_energy)  / (cm2 / g));
			energy_set.push_back(current_energy);
        }
    }

	if (temp_stream.is_open())
	{
		for (unsigned int i = 0; i < test_vector.size(); ++i)
		{
			temp_stream << energy_set[i] << "\t" << test_vector[i] << std::endl;
		}
	}

	ASSERT_NEAR(getNCC(reference_vector, test_vector), 1.0, 1e-3);
	ASSERT_NEAR(maxRelativeError(reference_vector, test_vector), 0.0, 0.1);
}


//-----------------------------------------------
double getSum(const std::vector<double>& aVector)
//-----------------------------------------------
{
    return (std::accumulate(aVector.begin(), aVector.end(), 0.0));
}


//---------------------------------------------------
double getAverage(const std::vector<double>& aVector)
//---------------------------------------------------
{
    return (getSum(aVector) / double(aVector.size()));
}


//----------------------------------------------------
double getVariance(const std::vector<double>& aVector)
//----------------------------------------------------
{
    double variance(0.0);
    double average(getAverage(aVector));

    for (std::vector<double>::const_iterator ite(aVector.begin());
            ite != aVector.end();
            ++ite)
    {
        variance += std::pow(*ite - average, 2);
    }

    return (variance / double(aVector.size()));
}


//--------------------------------------------------
double getStdDev(const std::vector<double>& aVector)
//--------------------------------------------------
{
    return (std::sqrt(getVariance(aVector)));
}


//------------------------------------------------
double getNCC(const std::vector<double>& aVector1,
              const std::vector<double>& aVector2)
//------------------------------------------------
{
    double ncc(0.0);

    double average1(getAverage(aVector1));
    double average2(getAverage(aVector2));

    double std_dev(getStdDev(aVector1) * getStdDev(aVector2));

    std::vector<double>::const_iterator ite1(aVector1.begin());
    std::vector<double>::const_iterator ite2(aVector2.begin());

    while (ite1 != aVector1.end())
    {
        ncc += (*ite1 - average1) * (*ite2 - average2);
        ++ite1;
        ++ite2;
    }

    ncc /= std_dev;
    ncc /= aVector1.size();

    return (ncc);
}


//----------------------------------------------------------
double maxRelativeError(const std::vector<double>& aVector1,
                        const std::vector<double>& aVector2)
//----------------------------------------------------------
{
    double max_relative_error(0.0);
    double relative_error(0.0);

    std::vector<double>::const_iterator ite1(aVector1.begin());
    std::vector<double>::const_iterator ite2(aVector2.begin());

    while (ite1 != aVector1.end())
    {
    	relative_error = std::abs(*ite1 - *ite2) / *ite1;
    	if (max_relative_error < relative_error) max_relative_error = relative_error;
        ++ite1;
        ++ite2;
    }

    return (max_relative_error);
}


//-----------------------------------------------------------------------
void buildGnuplotHeader(std::ofstream& anOutputStream, const char* aTest)
//-----------------------------------------------------------------------
{
	anOutputStream << "#!/usr/bin/env gnuplot" << std::endl;
	anOutputStream << std::endl;
	anOutputStream << "set term post eps enhanced color 'Helvetica' 32 size 9in,7in" << std::endl;
	anOutputStream << "set output '" << aTest << ".eps'" << std::endl;
	anOutputStream << std::endl;
	anOutputStream << "set grid" << std::endl;
	anOutputStream << std::endl;
	anOutputStream << "#set key bottom" << std::endl;
	anOutputStream << std::endl;
	anOutputStream << "set log xy" << std::endl;
	anOutputStream << "set format xy '10^{%T}'" << std::endl;
	anOutputStream << std::endl;
	anOutputStream << "set title '" << aTest << "'" << std::endl;
	anOutputStream << "set ylabel 'Mass attenuation coefficient ({/Symbol m}/{/Symbol r}) in cm^2/g'" << std::endl;
	anOutputStream << "set xlabel 'Photon energy in MeV'" << std::endl;
	anOutputStream << std::endl;
	anOutputStream << "set key autotitle columnhead" << std::endl;
}
