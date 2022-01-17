/**
********************************************************************************
*
*   @file       Image.cpp
*
*   @brief      Class to handle a greyscale image.
*
*   @version    1.0
*
*   @todo       Write here anything you did not implement.
*
*   @date       11/11/2016
*
*   @author     Franck Vidal
*
*
********************************************************************************
*/


//******************************************************************************
//  Define
//******************************************************************************
#define LINE_SIZE 4096


//******************************************************************************
//  Include
//******************************************************************************
#ifdef HAS_GLEW
#include "GL/glew.h"
#endif

#ifndef __gVirtualXRayConfig_h
#include "gVirtualXRay/gVirtualXRayConfig.h"
#endif

#include <fstream>

#ifndef __Image_h
#include "gVirtualXRay/Image.h"
#endif

#ifndef __CubeMesh_h
#include "gVirtualXRay/CubeMesh.h"
#endif

#ifndef __Exception_h
#include "gVirtualXRay/Exception.h"
#endif

#ifndef __XRayRenderer_h
#include "gVirtualXRay/XRayRenderer.h"
#endif


//******************************************************************************
//  namespace
//******************************************************************************
namespace gVirtualXRay {


template<> unsigned int Image<char>::g_mesh_type = GL_TRIANGLES;
template<> unsigned int Image<unsigned char>::g_mesh_type = GL_TRIANGLES;
template<> unsigned int Image<short>::g_mesh_type = GL_TRIANGLES;
template<> unsigned int Image<unsigned short>::g_mesh_type = GL_TRIANGLES;
template<> unsigned int Image<int>::g_mesh_type = GL_TRIANGLES;
template<> unsigned int Image<unsigned int>::g_mesh_type = GL_TRIANGLES;
template<> unsigned int Image<float>::g_mesh_type = GL_TRIANGLES;
template<> unsigned int Image<double>::g_mesh_type = GL_TRIANGLES;

template<> unsigned int Image<char>::g_vbo_type = GL_STATIC_DRAW;
template<> unsigned int Image<unsigned char>::g_vbo_type = GL_STATIC_DRAW;
template<> unsigned int Image<short>::g_vbo_type = GL_STATIC_DRAW;
template<> unsigned int Image<unsigned short>::g_vbo_type = GL_STATIC_DRAW;
template<> unsigned int Image<int>::g_vbo_type = GL_STATIC_DRAW;
template<> unsigned int Image<unsigned int>::g_vbo_type = GL_STATIC_DRAW;
template<> unsigned int Image<float>::g_vbo_type = GL_STATIC_DRAW;
template<> unsigned int Image<double>::g_vbo_type = GL_STATIC_DRAW;


//-----------------------------------------------------------------------
template<> void Image<float>::saveMHD(const std::string& aFileName,
    bool useDeflateCompressionIfPossible) const
//-----------------------------------------------------------------------
    {
    std::ofstream header_file(aFileName.data());

    if (!header_file.is_open())
        {
        throw Exception(__FILE__, __FUNCTION__, __LINE__, "Cannot open the file");
        }

    // Save the header file
    header_file << "ObjectType = Image" << std::endl;
    header_file << "NDims = 3" << std::endl;
    header_file << "BinaryData = True" << std::endl;
    header_file << "BinaryDataByteOrderMSB = False" << std::endl;
    header_file << "CompressedData = " << (useDeflateCompressionIfPossible? "True":"False") << std::endl;
    header_file << "TransformMatrix = 1 0 0 0 1 0 0 0 1" << std::endl;
    header_file << "Offset = 0 0 0" << std::endl;
    header_file << "CenterOfRotation = 0 0 0" << std::endl;
    header_file << "AnatomicalOrientation = RAI" << std::endl;
    header_file << "ElementSpacing = " << m_spacing.getX() << " " << m_spacing.getY() << " " << m_spacing.getZ() << std::endl;
    header_file << "DimSize = " << m_width << " " << m_height << " " << m_number_of_slices << std::endl;
    header_file << "ElementType = MET_FLOAT" << std::endl;

    std::string file_name_with_out_extension(aFileName.substr(0, aFileName.size() - 4));
    header_file << "ElementDataFile = " << file_name_with_out_extension + ".raw" << std::endl;

    // Save the RAW file
    saveRAW(file_name_with_out_extension + ".raw",
        useDeflateCompressionIfPossible);
}


//-----------------------------------------------------------------------------------------------
template<> void Image<short>::convertVoxelsToCubes(std::map<short, PolygonMesh>& aPolygonMeshSet,
                                                   short anAirThreshold) const
//-----------------------------------------------------------------------------------------------
{
    // Make sure there is no data in aPolygonMeshSet
    aPolygonMeshSet.clear();

    // Create the base cube
    CubeMesh<float> base_cube(cm);

    // The cube size is the same as the voxel size
    base_cube.applyScale(m_spacing.getX() / cm, m_spacing.getY() / cm, m_spacing.getZ() / cm);

    // Translate the cube so that its lower corner is on (0, 0, 0)
    base_cube.applyTranslation(m_spacing / 2.0);

    // Process every voxel
    for (unsigned int k(0); k < m_number_of_slices; ++k)
        {
        for (unsigned int j(0); j < m_height; ++j)
            {
            for (unsigned int i(0); i < m_width; ++i)
                {
                // Get the voxel value
                short voxel_value(getPixel(i, j ,k));

                // Ignore air and padding
                if (voxel_value > anAirThreshold)
                    {
                    // Copy the cube and translate it to the position of the voxel
                    PolygonMesh translated_cube(base_cube);
                    translated_cube.applyTranslation(VEC3(m_spacing[0] * i, m_spacing[1] * j, m_spacing[2] * k));

                    // Add the voxel
                    aPolygonMeshSet[voxel_value] += translated_cube;
                    aPolygonMeshSet[voxel_value].setHounsfieldValue(voxel_value);
                    }
                }
            }
        }
}


//-----------------------------------------------------------------------
template<> Image<float> Image<float>::convertHU2mu(double anEnergy) const
//-----------------------------------------------------------------------
{
    Image<float> temp(*this);

#ifdef NDEBUG
        #pragma omp parallel for
#endif
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
        {
        temp.m_p_image[i] = XRayRenderer::getMuFromHU(m_p_image[i],
                anEnergy);
        }

    return (temp);
}


//-------------------------------------------------------------------------
template<> Image<double> Image<double>::convertHU2mu(double anEnergy) const
//-------------------------------------------------------------------------
{
    Image<double> temp(*this);

#ifdef NDEBUG
        #pragma omp parallel for
#endif
    for (int i = 0; i < m_width * m_height * m_number_of_slices; ++i)
        {
        temp.m_p_image[i] = XRayRenderer::getMuFromHU(m_p_image[i],
                anEnergy);
        }

    return (temp);
}


};
