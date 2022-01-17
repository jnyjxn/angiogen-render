
// File: index.xml

// File: _simple_g_v_x_r_8h.xml

%feature("docstring") makeCylinder "

Create a cylinder and set its label in the scenegraph (i.e. identifier). Note
that it is not added to the X-ray renderer.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `aNumberOfSectors` :  
    the number of sector defining the mesh  
* `aHeight` :  
    the height of the cylinder  
* `aRadius` :  
    the radius of the cylinder  
* `aUnitOfLength` :  
    the unit of length corresponding to aHeight and aRadius. Acceptable values
    are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\" (default value: \"cm\")  
* `aParent` :  
    the parent of the node in the scenegraph (default: \"root\")  
";

%feature("docstring") getVersionOfCoreGVXR "

Accessor on the full string version of the core gVirtualXRay library (gvxr).  

Returns
-------
the full version number  
";

%feature("docstring") addEnergyBinToSpectrum "

Add an energy bin to the beam spectrum (useful to generate polychromatism).  

Parameters
----------
* `anEnergy` :  
    the incident photon energy  
* `aUnitOfEnergy` :  
    the unit of energy corresponding to anEnergy. Acceptable values are:
    \"electronvolt\", \"eV\", \"kiloelectronvolt\", \"keV\",
    \"megaelectronvolt\", \"MeV\"  
* `aNumberOfPhotons` :  
    the number of incident photons  
";

%feature("docstring") moveToCenter "

Move the scenegraph to the centre.  
";

%feature("docstring") moveToCenter "

Move a polygon mesh to the centre.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh in the scenegraph  
";

%feature("docstring") saveLastSinogram "

Save the last sinogram that has been computed.  

Parameters
----------
* `aFileName` :  
    the name of the output file (default: 0 means that the filename will be
    automatically generated and the file saved in the current path)  
* `useCompression` :  
    use data compression is possible (default: true)  
";

%feature("docstring") getNodeTransformationMatrix "

Accessor on the transformation matrix of a given node.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  

Returns
-------
the scengraph's root node transformation matrix as a 4x4 array  
";

%feature("docstring") setSceneTransformationMatrix "

Set the 3-D scene transformation matrix.  

Parameters
----------
* `aMatrix` :  
    the transformation matrix as a 4x4 array  
";

%feature("docstring") makeCube "

Create a cube and set its label in the scenegraph (i.e. identifier). Note that
it is not added to the X-ray renderer.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `aLength` :  
    the size of an edge of the cube  
* `aUnitOfLength` :  
    the unit of length corresponding to aLength. Acceptable values are: \"um\",
    \"micrometre\", \"micrometer\", \"mm\", \"millimetre\", \"millimeter\",
    \"cm\", \"centimetre\", \"centimeter\", \"dm\", \"decimetre\",
    \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\", \"decametre\",
    \"decameter\", \"hm\", \"hectometre\", \"hectometer\", \"km\",
    \"kilometre\", \"kilometer\" (default value: \"cm\")  
* `aParent` :  
    the parent of the node in the scenegraph (default: \"root\")  
";

%feature("docstring") autoCreateOpenGLContext "

Create an OpenGL context automatically.  

Parameters
----------
* `aFlag` :  
    true to create the OpenGL context automatically, false otherwise (default
    value: true)  
";

%feature("docstring") loadMeshFile "

Load a polygon mesh from a file, set its label in the scenegraph (i.e.
identifier) and add it to the X-ray renderer.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `aFileName` :  
    the file where the polygon mesh data is stored  
* `aUnitOfLength` :  
    the unit of length corresponding to the data stored in the file. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") getLastXRayImage "

Accessor on the last X-ray image that has been computed.  

Returns
-------
the last X-ray image  
";

%feature("docstring") rotateModelView "

Restore the matrix from the model/view matrix stack, and remove the last matrix
of the stack. glMatrixMode(GL_MODELVIEW);glRotate() functions.  

Parameters
----------
* `anAngle` :  
    the angle of rotation in degrees  
* `x` :  
    the coordinate of the rotation axis along the X-axis  
* `y` :  
    the coordinate of the rotation axis along the Y-axis  
* `z` :  
    the coordinate of the rotation axis along the Z-axis  
";

%feature("docstring") setDetectorUpVector "

Set the up-vector defining the orientation of the X-ray detector.  

Parameters
----------
* `x` :  
    the component of the vector along the X-axis  
* `y` :  
    the component of the vector along the Y-axis  
* `z` :  
    the component of the vector along the Z-axis  
";

%feature("docstring") setMixture "

Set the mixture corresponding to the material properties of a polygon mesh.
Don't forget to set the density of the material.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `aMixture` :  
    the details about the mixture. It is given as a sequence of element symbol &
    relative weight, e.g. Ti90Al6V4.  
";

%feature("docstring") setNodeTransformationMatrix "

Set the transformation matrix of a given node.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `aMatrix` :  
    the transformation matrix as a 4x4 array  
";

%feature("docstring") getMinValue "
";

%feature("docstring") initShader "

Initialise visualisation shader.  
";

%feature("docstring") displayNode "

Display the scenegraph node using OpenGL.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
";

%feature("docstring") getMaxValue "
";

%feature("docstring") getPatchVersionOfCoreGVXR "

Accessor on the minor version of the core gVirtualXRay library (gvxr). This
number is changed when bug fixes have been added in a backwards-compatible
manner.  

Returns
-------
the patch version number  
";

%feature("docstring") getNodeAndChildrenBoundingBox "

Access the bounding box of a given node and all its children (if any). The
bounding box is given in the world coordinate system.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `aUnitOfLength` :  
    the unit of length corresponding to the bounding box. Acceptable values are:
    \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\" (default value: \"cm\")  

Returns
-------
the bounding box as: min_x, min_y, min_z, max_x, max_y, max_z  
";

%feature("docstring") createWindow "

Create an OpenGL context and display it in a window.  

Parameters
----------
* `aWindowID` :  
    the numerical ID of the context to create (default value: -1, means that the
    ID will be automatically generated)  
* `aVisibilityFlag` :  
    flag controling if the window should be visible (1) or hidden (0). (default
    value: 0)  
";

%feature("docstring") initialiseGLEW "

Initialise GLEW if it is supported.  
";

%feature("docstring") saveLastLBuffer "

Save the last L-buffer that has been computed.  

Parameters
----------
* `aFileName` :  
    the name of the output file (default: means that the filename will be
    automatically generated and the file saved in the current path)  
* `useCompression` :  
    use data compression is possible (default: true)  
";

%feature("docstring") getMassAttenuationFromHU "

Accessor on the HU's mass attenuation.  

Parameters
----------
* `HU` :  
    the HU  
* `anEnergy` :  
    the photon energy of interest  
* `aUnitOfEnergy` :  
    the unit of energy corresponding to anEnergy. Acceptable values are:
    \"electronvolt\", \"eV\", \"kiloelectronvolt\", \"keV\",
    \"megaelectronvolt\", \"MeV\"  

Returns
-------
the density of the corresponding HU (in cm2/g)  
";

%feature("docstring") getDetectorRightVector "

Accessor on the right-vector defining the orientation of the X-ray detector.  

Returns
-------
the right-vector  
";

%feature("docstring") scrollCallback "

Call this function when the user scrolls.  

Parameters
----------
* `xoffset` :  
    the scroll offset along the x-axis (not used)  
* `yoffset` :  
    the scroll offset along the y-axis (used for zooming in and out)  
";

%feature("docstring") invertNormalVectors "

Invert the normal vectors of a given polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
";

%feature("docstring") getPatchVersionOfSimpleGVXR "

Accessor on the minor version of SimpleGVXR. This number is changed when bug
fixes have been added in a backwards-compatible manner.  

Returns
-------
the patch version number  
";

%feature("docstring") getMinorVersionOfSimpleGVXR "

Accessor on the minor version of SimpleGVXR. This number is changed when new
functionalities have been added in a backwards-compatible manner.  

Returns
-------
the minor version number  
";

%feature("docstring") getMassAttenuationFromCompound "

Accessor on the compound's mass attenuation.  

Parameters
----------
* `aCompound` :  
    the details about the compound. It is given as a sequence of element symbol
    & number of atoms, e.g. H2O for water and SiC for silicon carbide.  
* `anEnergy` :  
    the photon energy of interest  
* `aUnitOfEnergy` :  
    the unit of energy corresponding to anEnergy. Acceptable values are:
    \"electronvolt\", \"eV\", \"kiloelectronvolt\", \"keV\",
    \"megaelectronvolt\", \"MeV\"  

Returns
-------
the mass attenuation of the corresponding compound (in cm2/g)  
";

%feature("docstring") saveLastCumulatedLBuffer "

Save the last cumulated L-buffer that has been computed.  

Parameters
----------
* `aFileName` :  
    the name of the output file (default: means that the filename will be
    automatically generated and the file saved in the current path)  
* `useCompression` :  
    use data compression is possible (default: true)  
";

%feature("docstring") getNodeOnlyBoundingBox "

Access the bounding box of a given node (without its children). The bounding box
is given in the world coordinate system.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `aUnitOfLength` :  
    the unit of length corresponding to the bounding box. Acceptable values are:
    \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\" (default value: \"cm\")  

Returns
-------
the bounding box as: min_x, min_y, min_z, max_x, max_y, max_z  
";

%feature("docstring") destroyAllWindows "

Close and destroy all the windows and OpenGL contexts that have been created.  
";

%feature("docstring") makeIsoSurface "

Create an iso-surface from a 3D image and set its label in the scenegraph (i.e.
identifier). Note that it is not added to the X-ray renderer.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `anIsoValue` :  
    the iso-value  
* `aVoxelDataSet` :  
    the voxel dataset  
* `aWidth` :  
    the number of voxel along the X-axis  
* `aHeight` :  
    the number of voxel along the Y-axis  
* `aDepth` :  
    the number of voxel along the Z-axis  
* `aSpacingX` :  
    the space between the centre of two successive voxels along the X-axis  
* `aSpacingY` :  
    the space between the centre of two successive voxels along the Y-axis  
* `aSpacingZ` :  
    the space between the centre of two successive voxels along the Z-axis  
* `aUnitOfLength` :  
    the unit of length corresponding to aSpacingX, aSpacingY and aSpacingZ.
    Acceptable values are: \"um\", \"micrometre\", \"micrometer\", \"mm\",
    \"millimetre\", \"millimeter\", \"cm\", \"centimetre\", \"centimeter\",
    \"dm\", \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\" (default value: \"cm\")  
* `aParent` :  
    the parent of the node in the scenegraph (default: \"root\")  
";

%feature("docstring") getDetectorPosition "

Accessor on the position of the X-ray detector.  

Parameters
----------
* `aUnitOfLength` :  
    the unit of length corresponding to the returned value. Acceptable values
    are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  

Returns
-------
the source position in a given unit of length  
";

%feature("docstring") getElementSymbol "

Accessor on the chemical element' symbol depending on its Z number.  

Parameters
----------
* `Z` :  
    the atomic number of the element  

Returns
-------
the name of the corresponding element  
";

%feature("docstring") getMuFromHU "

Accessor on the HU's linear attenuation coefficient.  

Parameters
----------
* `HU` :  
    the HU  
* `anEnergy` :  
    the photon energy of interest  
* `aUnitOfEnergy` :  
    the unit of energy corresponding to anEnergy. Acceptable values are:
    \"electronvolt\", \"eV\", \"kiloelectronvolt\", \"keV\",
    \"megaelectronvolt\", \"MeV\"  

Returns
-------
the linear attenuation coefficient (in cm-1)  
";

%feature("docstring") loadSceneGraph "

Load a scenegraph from a file and add its polygon meshes to the X-ray renderer.  

Parameters
----------
* `aFileName` :  
    the file where the polygon mesh data is stored  
* `aUnitOfLength` :  
    the unit of length corresponding to the data stored in the file. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") loadSpectrumFromTSV "

Load the incident beam energy spectrum from a TSV file.  

Parameters
----------
* `aFileName` :  
    name of the file to load  
* `aUnitOfEnergy` :  
    the unit of energy corresponding to anEnergy. Acceptable values are:
    \"electronvolt\", \"eV\", \"kiloelectronvolt\", \"keV\",
    \"megaelectronvolt\", \"MeV\"  
* `aNormalisationFlag` :  
    true to normalise the total energy to 1, false otherwise  
";

%feature("docstring") setDetectorPixelSize "

Set the pixel size.  

Parameters
----------
* `aWidth` :  
    the pixel size along the X-axis  
* `aHeight` :  
    the pixel size along the Y-axis  
* `aUnitOfLength` :  
    the unit of length corresponding to the aWidth and aHeight parameters.
    Acceptable values are: \"um\", \"micrometre\", \"micrometer\", \"mm\",
    \"millimetre\", \"millimeter\", \"cm\", \"centimetre\", \"centimeter\",
    \"dm\", \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") setMassAttenuationCoefficient "

Set the mass attenuation coefficient corresponding to the material properties of
a polygon mesh. Don't forget to set the density of the material.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `aCoefficient` :  
    the mass attenuation coefficient  
* `aUnit` :  
    the unit corresponding to aCoefficient. Acceptable values are: \"cm2/g\" and
    \"cm2.g-1\"  
";

%feature("docstring") lookAt "

Replace the modelling-viewing matrix by a viewing transformation matrix. It
behaves as the old gluLookAt function.  

Parameters
----------
* `eyeX` :  
    Specifies the position of the eye point along the X-axis  
* `eyeY` :  
    Specifies the position of the eye point along the Y-axis  
* `eyeZ` :  
    Specifies the position of the eye point along the Z-axis  
* `lookAtX` :  
    Specifies the position of the reference point along the X-axis  
* `lookAtY` :  
    Specifies the position of the reference point along the Y-axis  
* `lookAtZ` :  
    Specifies the position of the reference point along the Z-axis  
* `upX` :  
    Specifies the direction of the up vector along the X-axis  
* `upY` :  
    Specifies the direction of the up vector along the Y-axis  
* `upZ` :  
    Specifies the direction of the up vector along the Z-axis  
";

%feature("docstring") perspective "

Replace the projection matrix by a perspective projection matrix. It behaves as
the old gluPerspective function.  

Parameters
----------
* `fovy` :  
    specifies the field of view angle, in degrees, in the y-direction.  
* `aspect` :  
    Specifies the aspect ratio that determines the field of view in the x
    direction. The aspect ratio is the ratio of x (width) to y (height).  
* `zNear` :  
    Specifies the distance from the viewer to the near clipping plane (always
    positive)  
* `zFar` :  
    Specifies the distance from the viewer to the far clipping plane (always
    positive).  
";

%feature("docstring") computeProjectionSet "

Compute the projection set corresponding to the environment that has previously
been set, i.e.  

*   Detector position  
*   Detector orientation  
*   Detector size and resolution  
*   Source position  
*   Source shape  
*   Beam spectrum  
*   Scanned object geometries  
*   Scanned object material properties Note that the rotation vector is the same
    as the up-vector of the detector.  

    Parameters:  
    * `x` :  
        the position of the rotation centre along the X-axis  
    * `y` :  
        the position of the rotation centre along the Y-axis  
    * `z` :  
        the position of the rotation centre along the Z-axis  
    * `aUnitOfLength` :  
        the unit of length corresponding to the x, y and z parameters.
        Acceptable values are: \"um\", \"micrometre\", \"micrometer\", \"mm\",
        \"millimetre\", \"millimeter\", \"cm\", \"centimetre\", \"centimeter\",
        \"dm\", \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\",
        \"dam\", \"decametre\", \"decameter\", \"hm\", \"hectometre\",
        \"hectometer\", \"km\", \"kilometre\", \"kilometer\"  
    * `aNumberOfAngle` :  
        the total number of projections in the projection set  
    * `anAngleOffset` :  
        the angle in degrees between two successive projections  

    Returns:
    the corresponding projection set  
";

%feature("docstring") getMassAttenuationCoefficient "

Accessor on the mass attenuation coefficient at a given energy corresponding to
the material properties of a polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `anEnergy` :  
    the photon energy of interest  
* `aUnitOfEnergy` :  
    the unit of energy corresponding to anEnergy. Acceptable values are:
    \"electronvolt\", \"eV\", \"kiloelectronvolt\", \"keV\",
    \"megaelectronvolt\", \"MeV\"  

Returns
-------
the mass attenuation coefficient of the polygon mesh's material in cm2/g  
";

%feature("docstring") getSourcePosition "

Accessor on the position of the X-ray source.  

Parameters
----------
* `aUnitOfLength` :  
    the unit of length corresponding to the returned value. Acceptable values
    are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  

Returns
-------
the source position in a given unit of length  
";

%feature("docstring") setMonoChromatic "

Use a monochromatic beam spectrum (i.e. one single energy).  

Parameters
----------
* `anEnergy` :  
    the incident photon energy  
* `aUnitOfEnergy` :  
    the unit of energy corresponding to anEnergy. Acceptable values are:
    \"electronvolt\", \"eV\", \"kiloelectronvolt\", \"keV\",
    \"megaelectronvolt\", \"MeV\"  
* `aNumberOfPhotons` :  
    the number of incident photons  
";

%feature("docstring") applyCurrentLocalTransformation "

Apply the current local transformation to all the vertices of a given node in
the scenegraph. Note the the local transformation is then reset to the identity
matrix.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
";

%feature("docstring") pushModelViewMatrix "

Add the current matrix to the model/view matrix stack. It behaves as the
combination of the old glMatrixMode(GL_MODELVIEW);glPushMatrix() functions.  
";

%feature("docstring") computeXRayImage "

Compute the X-ray projection corresponding to the environment that has
previously been set, i.e.  

*   Detector position  
*   Detector orientation  
*   Detector size and resolution  
*   Source position  
*   Source shape  
*   Beam spectrum  
*   Scanned object geometries  
*   Scanned object material properties  

    Returns:
    the corresponding X-ray image  
";

%feature("docstring") setDetectorNumberOfPixels "

Set the number of pixels of the X-ray detector.  

Parameters
----------
* `aWidth` :  
    the number of pixels along the X-axis  
* `aHeight` :  
    the number of pixels along the Y-axis  
";

%feature("docstring") getUnitOfEnergy "

Accessor on the numerical value corresponding to a unit of energy.  

Parameters
----------
* `aUnitOfEnergy` :  
    the unit of energy. Acceptable values are: \"electronvolt\", \"eV\",
    \"kiloelectronvolt\", \"keV\", \"megaelectronvolt\", \"MeV\"  

Returns
-------
the corresponding numerical value  
";

%feature("docstring") setLinearAttenuationCoefficient "

Set the linear attenuation coefficient corresponding to the material properties
of a polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `aCoefficient` :  
    the liner attenuation coefficient  
* `aUnit` :  
    the unit corresponding to aCoefficient. Acceptable values are: \"cm-1\"  
";

%feature("docstring") getImageHeght "

Accessor on the height of a given image.  

Parameters
----------
* `anImage` :  
    the image  

Returns
-------
the height of the image (in number of pixels)  
";

%feature("docstring") createOpenGLContext "

Create an OpenGL context (the window won't be shown).  

Parameters
----------
* `aWindowID` :  
    the numerical ID of the context to create (default value: -1, means that the
    ID will be automatically generated)  
";

%feature("docstring") scaleNode "

Scale a polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh to transform  
* `x` :  
    the scaling factor along the X-axis  
* `y` :  
    the scaling factor along the Y-axis  
* `z` :  
    the scaling factor along the Z-axis  
* `aUnitOfLength` :  
    the unit of length corresponding to the x, y and z parameters. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") enableArtifactFilteringOnGPU "

Enable artefact filtering on GPU (it is fast but can be inaccurate).  
";

%feature("docstring") moveToCentre "

Move the scenegraph to the centre.  
";

%feature("docstring") moveToCentre "

Move a polygon mesh to the centre.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh in the scenegraph  
";

%feature("docstring") disableArtefactFiltering "

Disable artefact filtering altogether.  
";

%feature("docstring") rotateScene "

Rotate the 3-D scene (all the polygon meshes will be affected but not modified).  

Parameters
----------
* `anAngle` :  
    the rotation angle in degrees  
* `x` :  
    the component of the rotation vector along the X-axis  
* `y` :  
    the component of the rotation vector along the Y-axis  
* `z` :  
    the component of the rotation vector along the Z-axis  
";

%feature("docstring") addMesh "

Add a polygon mesh (aSource) to another one (aDestination). It corresponds to
the operation as follows: aDestination += aSource. Both aSource and aDestination
have to already exist in the scenegraph.  

Parameters
----------
* `aDestination` :  
    the polygon mesh that is going to be modified  
* `aSource` :  
    the polygon mesh that is going to be added to aDestination  
";

%feature("docstring") displayScene "

3-D visualisation of the 3-D scene (source, detector, and scanned objects). Note
that there is no interactive loop running.  

Parameters
----------
* `aSceneRotationFlag` :  
    true if the 3-D scene has to be rotated with the arc-ball method, false
    otherwise (default value: true)  
* `aWindowID` :  
    the numerical ID of the corresponding context (default value: -1, means that
    the active context will be used)  
";

%feature("docstring") setDetectorPosition "

Set the position of the X-ray detector.  

Parameters
----------
* `x` :  
    the position along the X-axis  
* `y` :  
    the position along the Y-axis  
* `z` :  
    the position along the Z-axis  
* `aUnitOfLength` :  
    the unit of length corresponding to the x, y and z parameters. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") getDiffuseColour "

Accessor on the diffuse colour of the material of a given polygon mesh.  

Returns
-------
the diffuse colour  
";

%feature("docstring") getLastSinogram "

Accessor on the last sinogram that has been computed.  

Returns
-------
the last sinogram  
";

%feature("docstring") subtractMesh "

Subtract a polygon mesh (aSource) from another one (aDestination). It
corresponds to the operation as follows: aDestination -= aSource. Both aSource
and aDestination have to already exist in the scenegraph.  

Parameters
----------
* `aDestination` :  
    the polygon mesh that is going to be modified  
* `aSource` :  
    the polygon mesh that is going to be subtracted from aDestination  
";

%feature("docstring") getShininess "

Accessor on the shininess of the material of a given polygon mesh.  

Returns
-------
the shininess coefficient  
";

%feature("docstring") getElementName "

Accessor on the chemical element's name depending on its Z number.  

Parameters
----------
* `Z` :  
    the atomic number of the element  

Returns
-------
the name of the corresponding element  
";

%feature("docstring") setHounsfieldValue "

Set the Hounsfield value corresponding to the material properties of a polygon
mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `HU` :  
    the Hounsfield value  
";

%feature("docstring") getMajorVersionOfCoreGVXR "

Accessor on the major version of the core gVirtualXRay library (gvxr). This
number is changed when incompatible API changes have been made.  

Returns
-------
the major version number  
";

%feature("docstring") disableArtifactFiltering "

Disable artefact filtering altogether.  
";

%feature("docstring") setColor "

Set the color of a given polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `R` :  
    the red channel  
* `G` :  
    the green channel  
* `B` :  
    the blue channel  
* `A` :  
    the alpha channel  
";

%feature("docstring") scaleScene "

Scale the 3-D scene (all the polygon meshes will be affected but not modified).  

Parameters
----------
* `x` :  
    the scaling factor along the X-axis  
* `y` :  
    the scaling factor along the Y-axis  
* `z` :  
    the scaling factor along the Z-axis  
";

%feature("docstring") scaleScene "

Scale the 3-D scene (all the polygon meshes will be affected but not modified).  

Parameters
----------
* `x` :  
    the scaling factor along the X-axis  
* `y` :  
    the scaling factor along the Y-axis  
* `z` :  
    the scaling factor along the Z-axis  
* `aUnitOfLength` :  
    the unit of length corresponding to the x, y and z parameters. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") saveLastXRayImage "

Save the last X-ray image that has been computed.  

Parameters
----------
* `aFileName` :  
    the name of the output file (default: means that the filename will be
    automatically generated and the file saved in the current path)  
* `useCompression` :  
    use data compression is possible (default: true)  
";

%feature("docstring") getPhotonCountEnergyBins "

Accessor on the number of photons per energy bin of the beam spectrum.  

Returns
-------
the list of number of photons  
";

%feature("docstring") useParallelBeam "

Use a parallel source (e.g. to mimic a source that is extremely far from the
scanned object and detector).  
";

%feature("docstring") getDensityFromElement "

Accessor on the chemical element's density.  

Parameters
----------
* `anElement` :  
    the name or symbol of the element  

Returns
-------
the density of the corresponding element (in g / cm3)  
";

%feature("docstring") getDensityFromElement "

Accessor on the chemical element's density.  

Parameters
----------
* `Z` :  
    the Z number of the element  

Returns
-------
the density of the corresponding element (in g / cm3)  
";

%feature("docstring") hideWindow "

Hide a window.  

Parameters
----------
* `aWindowID` :  
    the numerical ID of the corresponding context (default value: -1, means that
    the active context will be used)  
";

%feature("docstring") getSceneTransformationMatrix "

Accessor on the 3-D scene transformation matrix.  

Returns
-------
the 3-D scene transformation matrix as a 4x4 array  
";

%feature("docstring") getMinorVersionOfCoreGVXR "

Accessor on the minor version of the core gVirtualXRay library (gvxr). This
number is changed when new functionalities have been added in a backwards-
compatible manner.  

Returns
-------
the minor version number  
";

%feature("docstring") getDensity "

Accessor on the density corresponding to the material properties of a polygon
mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  

Returns
-------
the density of the polygon mesh's material in g/cm3  
";

%feature("docstring") setHU "

Set the Hounsfield value corresponding to the material properties of a polygon
mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `HU` :  
    the Hounsfield value  
";

%feature("docstring") setMu "

Set the linear attenuation coefficient corresponding to the material properties
of a polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `aCoefficient` :  
    the liner attenuation coefficient  
* `aUnit` :  
    the unit corresponding to aCoefficient. Acceptable values are: \"cm-1\"  
";

%feature("docstring") getChildLabel "

Accessor on the i-th child's label in the list of children of a given node in
the scenegraph.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `i` :  
    the ID of the child  

Returns
-------
the label corresponding to the i-th child  
";

%feature("docstring") stopArcBallRotation "

Call this function when the user stops an arc-ball rotation (e.g. release the
left-mouse button click in the 3-D visualisation window).  
";

%feature("docstring") setSourcePosition "

Set the position of the X-ray source.  

Parameters
----------
* `x` :  
    the position along the X-axis  
* `y` :  
    the position along the Y-axis  
* `z` :  
    the position along the Z-axis  
* `aUnitOfLength` :  
    the unit of length corresponding to the x, y and z parameters. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") useParallelSource "

Use a parallel source (e.g. to mimic a source that is extremely far from the
scanned object and detector).  
";

%feature("docstring") orthographic "

Replace the projection matrix by a orthographic projection matrix. It behaves as
the old glOrtho function.  

Parameters
----------
* `left` :  
    Specify the coordinates for the left vertical clipping plane  
* `right` :  
    Specify the coordinates for the right vertical clipping plane  
* `bottom` :  
    Specify the coordinates for the bottom horizontal clipping plane  
* `top` :  
    Specify the coordinates for the top horizontal clipping plane  
* `zNear` :  
    Specify the distances to the near clipping plane. The value must be positive  
* `zFar` :  
    Specify the distances to the far clipping plane. The value must be positive  
";

%feature("docstring") popModelViewMatrix "

Restore the matrix from the model/view matrix stack, and remove the last matrix
of the stack. glMatrixMode(GL_MODELVIEW);glPopMatrix() functions.  
";

%feature("docstring") getLastLBuffer "

Accessor on the last L-buffer that has been computed.  

Returns
-------
the last L-buffer  
";

%feature("docstring") emptyMesh "

Create an empty polygon mesh and set its label in the scenegraph (i.e.
identifier). Note that it is not added to the X-ray renderer.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `aParent` :  
    the parent of the node in the scenegraph (default: \"root\")  
";

%feature("docstring") addPolygonMeshAsOuterSurface "

Add a polygon mesh, given its label, to the X-ray renderer as an outer surface.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh in the scenegraph  
";

%feature("docstring") usePointSource "

Use a point source.  
";

%feature("docstring") getDetectorUpVector "

Accessor on the up-vector defining the orientation of the X-ray detector.  

Returns
-------
the up-vector  
";

%feature("docstring") getLastProjectionSet "

Accessor on the last projection set that has been computed.  

Returns
-------
the last projection set  
";

%feature("docstring") setColour "

Set the colour of a given polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `R` :  
    the red channel  
* `G` :  
    the green channel  
* `B` :  
    the blue channel  
* `A` :  
    the alpha channel  
";

%feature("docstring") setLocalTransformationMatrix "

Set the local transformation matrix of a given node in the scenegraph.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  
* `aMatrix` :  
    the local transformation matrix  
";

%feature("docstring") initializeGLEW "

Initialise GLEW if it is supported.  
";

%feature("docstring") getAmbientColour "

Accessor on the ambient colour of the material of a given polygon mesh.  

Returns
-------
the ambient colour  
";

%feature("docstring") getLocalTransformationMatrix "

Accessor on the local transformation matrix of a given node in the scenegraph.
If the node does not exist, the identity matrix is returned.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  

Returns
-------
the corresponding local transformation matrix  
";

%feature("docstring") getImageRow "

Accessor on the j-th row of pixels of a given image.  

Parameters
----------
* `anImage` :  
    the image  
* `j` :  
    the row number  

Returns
-------
the j-th row of pixels  
";

%feature("docstring") getMassAttenuationFromMixture "

Accessor on the mixture's mass attenuation.  

Parameters
----------
* `aMixture` :  
    the details about the mixture. It is given as a sequence of element symbol &
    relative weight, e.g. Ti90Al6V4.  
* `anEnergy` :  
    the photon energy of interest  
* `aUnitOfEnergy` :  
    the unit of energy corresponding to anEnergy. Acceptable values are:
    \"electronvolt\", \"eV\", \"kiloelectronvolt\", \"keV\",
    \"megaelectronvolt\", \"MeV\"  

Returns
-------
the mass attenuation of the corresponding mixture (in cm2/g)  
";

%feature("docstring") setWindowSize "

Create an OpenGL context and display it in a window.  

Parameters
----------
* `aWidth` :  
    the number of pixels along the X-axis  
* `aHeight` :  
    the number of pixels along the Y-axis  
* `aWindowID` :  
    the numerical ID of the corresponding context (default value: -1, means that
    the active context will be used)  
";

%feature("docstring") getMajorVersionOfSimpleGVXR "

Accessor on the major version of SimpleGVXR. This number is changed when
incompatible API changes have been made.  

Returns
-------
the major version number  
";

%feature("docstring") rotateNode "

Rotate a polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh to transform  
* `anAngle` :  
    the rotation angle in degrees  
* `x` :  
    the component of the rotation vector along the X-axis  
* `y` :  
    the component of the rotation vector along the Y-axis  
* `z` :  
    the component of the rotation vector along the Z-axis  
";

%feature("docstring") loadImage3D "
";

%feature("docstring") enableArtefactFilteringOnGPU "

Enable artefact filtering on GPU (it is fast but can be inaccurate).  
";

%feature("docstring") enableArtefactFilteringOnCPU "

Enable artefact filtering on CPU (can be extremely slow as it makes use of ray-
tracing).  
";

%feature("docstring") getNumberOfChildren "

Get the number of children of a node in the scenegraph.  

Parameters
----------
* `aLabel` :  
    the label in the scenegraph  

Returns
-------
the number of chidren  
";

%feature("docstring") saveSTLfile "

Save the polygon mesh of a given node.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `aFileName` :  
    the name of the output file (default: means that the filename will be
    automatically generated and the file saved in the current path)  
";

%feature("docstring") getDensityFromHU "

Accessor on the HU's density.  

Parameters
----------
* `HU` :  
    the HU  

Returns
-------
the density of the corresponding HU (in g / cm3)  
";

%feature("docstring") setDensity "

Set the density corresponding to the material properties of a polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `aDensity` :  
    the density  
* `aUnit` :  
    the unit corresponding to aDensity. Acceptable values are: \"g/cm3\" and
    \"g.cm-3\"  
";

%feature("docstring") resetSceneTransformation "

Replace the 3-D transformation matrix of the overall 3-D scene by an identity
matrix.  
";

%feature("docstring") saveLastProjectionSet "

Save the last projection set that has been computed.  

Parameters
----------
* `aFileName` :  
    the name of the output file (default: 0 means that the filename will be
    automatically generated and the file saved in the current path)  
* `useCompression` :  
    use data compression is possible (default: true)  
";

%feature("docstring") shearNode "

Use a shear matrix to transform the node.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh to transform  
* `yx` :  
    the yx component of the shear matrix  
* `zx` :  
    the zx component of the shear matrix  
* `xy` :  
    the xy component of the shear matrix  
* `zy` :  
    the zy component of the shear matrix  
* `xz` :  
    the xz component of the shear matrix  
* `yz` :  
    the yz component of the shear matrix  
";

%feature("docstring") getMaterialLabel "

Accessor on the material label of a polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  

Returns
-------
the label of the material  
";

%feature("docstring") computeZNCC "
";

%feature("docstring") usingArcBallRotation "

Check if arc-ball rotation is currently in use.  

Returns
-------
true if arc-ball rotation is currently in use, false otherwise  
";

%feature("docstring") getMassAttenuationFromElement "

Accessor on the chemical element's mass attenuation.  

Parameters
----------
* `anElement` :  
    the name or symbol of the element  
* `anEnergy` :  
    the photon energy of interest  
* `aUnitOfEnergy` :  
    the unit of energy corresponding to anEnergy. Acceptable values are:
    \"electronvolt\", \"eV\", \"kiloelectronvolt\", \"keV\",
    \"megaelectronvolt\", \"MeV\"  

Returns
-------
the mass attenuation of the corresponding element (in cm2/g)  
";

%feature("docstring") getMassAttenuationFromElement "

Accessor on the chemical element's mass attenuation.  

Parameters
----------
* `Z` :  
    the Z number of the element  
* `anEnergy` :  
    the photon energy of interest  
* `aUnitOfEnergy` :  
    the unit of energy corresponding to anEnergy. Acceptable values are:
    \"electronvolt\", \"eV\", \"kiloelectronvolt\", \"keV\",
    \"megaelectronvolt\", \"MeV\"  

Returns
-------
the mass attenuation of the corresponding element (in cm2/g)  
";

%feature("docstring") translateNode "

Translate a polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh to transform  
* `x` :  
    the component of the translation vector along the X-axis  
* `y` :  
    the component of the translation vector along the Y-axis  
* `z` :  
    the component of the translation vector along the Z-axis  
* `aUnitOfLength` :  
    the unit of length corresponding to the x, y and z parameters. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") computeLBuffer "

Compute the L-buffer of a polygon mesh corresponding to the environment that has
previously been set, i.e.  

*   Detector position  
*   Detector orientation  
*   Detector size and resolution  
*   Source position  
*   Source shape  
*   Scanned object geometry  

    Parameters:  
    * `aLabel` :  
        the label of the polygon mesh  

    Returns:
    the corresponding L-buffer  
";

%feature("docstring") cursorPositionCallback "

Call this function when the user moves the mouse.  

Parameters
----------
* `x` :  
    the cursor x-axis position in the viewport coordinate system  
* `y` :  
    the cursor y-axis position in the viewport coordinate system  
* `aViewportWidth` :  
    the size of the viewport along the x-axis  
* `aViewportHeight` :  
    the size of the viewport along the y-axis  
";

%feature("docstring") enableArtifactFilteringOnCPU "

Enable artefact filtering on CPU (can be extremely slow as it makes use of ray-
tracing).  
";

%feature("docstring") setCompound "

Set the compound corresponding to the material properties of a polygon mesh.
Don't forget to set the density of the material.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `aCompound` :  
    the details about the compound. It is given as a sequence of element symbol
    & number of atoms, e.g. H2O for water and SiC for silicon carbide.  
";

%feature("docstring") getVersionOfSimpleGVXR "

Accessor on the full string version of SimpleGVXR.  

Returns
-------
the full version number  
";

%feature("docstring") computeRMSE "
";

%feature("docstring") getImageWidth "

Accessor on the width of a given image.  

Parameters
----------
* `anImage` :  
    the image  

Returns
-------
the width of the image (in number of pixels)  
";

%feature("docstring") removePolygonMeshesFromSceneGraph "

Empty the scenegraph. Note that it also empties the X-ray renderer from all its
meshes.  
";

%feature("docstring") setElement "

Set the chemical element (or element) corresponding to the material properties
of a polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `Z` :  
    the atomic number (or Z number) corresponding to the element  
";

%feature("docstring") setElement "

Set the chemical element (or element) corresponding to the material properties
of a polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `aName` :  
    the symbol or name corresponding to the element  
";

%feature("docstring") getUnitOfLength "

Accessor on the numerical value corresponding to a unit of length.  

Parameters
----------
* `aUnitOfLength` :  
    the unit of length. Acceptable values are: \"um\", \"micrometre\",
    \"micrometer\", \"mm\", \"millimetre\", \"millimeter\", \"cm\",
    \"centimetre\", \"centimeter\", \"dm\", \"decimetre\", \"decimeter\", \"m\",
    \"metre\", \"meter\", \"dam\", \"decametre\", \"decameter\", \"hm\",
    \"hectometre\", \"hectometer\", \"km\", \"kilometre\", \"kilometer\"  

Returns
-------
the corresponding numerical value  
";

%feature("docstring") getLinearAttenuationCoefficient "

Accessor on the linear attenuation coefficient at a given energy corresponding
to the material properties of a polygon mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `anEnergy` :  
    the photon energy of interest  
* `aUnitOfEnergy` :  
    the unit of energy corresponding to anEnergy. Acceptable values are:
    \"electronvolt\", \"eV\", \"kiloelectronvolt\", \"keV\",
    \"megaelectronvolt\", \"MeV\"  

Returns
-------
the linear attenuation coefficient of the polygon mesh's material in cm-1  
";

%feature("docstring") getEnergyBins "

Accessor on the energy bins of the beam spectrum.  

Parameters
----------
* `aUnitOfEnergy` :  
    the unit of energy corresponding to the returned vector. Acceptable values
    are: \"electronvolt\", \"eV\", \"kiloelectronvolt\", \"keV\",
    \"megaelectronvolt\", \"MeV\"  

Returns
-------
the list of energy bins in aUnitOfEnergy  
";

%feature("docstring") translateScene "

Translate the 3-D scene (all the polygon meshes will be affected but not
modified).  

Parameters
----------
* `x` :  
    the component of the translation vector along the X-axis  
* `y` :  
    the component of the translation vector along the Y-axis  
* `z` :  
    the component of the translation vector along the Z-axis  
* `aUnitOfLength` :  
    the unit of length corresponding to the x, y and z parameters. Acceptable
    values are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  
";

%feature("docstring") startArcBallRotation "

Call this function when the user starts an arc-ball rotation (e.g. left-mouse
button click in the 3-D visualisation window).  

Parameters
----------
* `x` :  
    the cursor x-axis position in the viewport coordinate system  
* `y` :  
    the cursor y-axis position in the viewport coordinate system  
";

%feature("docstring") loadImage2D "

void setShiftFilter(double aValue); void setScaleFilter(double aValue); double
getShiftFilter(); double getScaleFilter();  
";

%feature("docstring") renderLoop "

3-D visualisation of the 3-D scene (source, detector, and scanned objects). Note
that there is interactive loop running. Keys are:  

*   Q/Escape: to quit the event loop (does not close the window)  
*   B: display/hide the X-ray beam  
*   W: display the polygon meshes in solid or wireframe  
*   N: display the X-ray image in negative or positive  
*   L: switch lighting on/off  
*   D: display/hide the X-ray detector  
*   V: display/hide normal vectors  

    Parameters:  
    * `aWindowID` :  
        the numerical ID of the corresponding context (default value: -1, means
        that the active context will be used)  
";

%feature("docstring") getDetectorSize "

Accessor on the size of the X-ray detector.  

Parameters
----------
* `aUnitOfLength` :  
    the unit of length corresponding to the returned value. Acceptable values
    are: \"um\", \"micrometre\", \"micrometer\", \"mm\", \"millimetre\",
    \"millimeter\", \"cm\", \"centimetre\", \"centimeter\", \"dm\",
    \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\", \"dam\",
    \"decametre\", \"decameter\", \"hm\", \"hectometre\", \"hectometer\",
    \"km\", \"kilometre\", \"kilometer\"  

Returns
-------
the size in a given unit of length  
";

%feature("docstring") removePolygonMeshesFromXRayRenderer "

Empty the X-ray renderer from all its meshes. Note that the meshes are kept in
the scenegraph.  
";

%feature("docstring") setHounsfieldUnit "

Set the Hounsfield value corresponding to the material properties of a polygon
mesh.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh  
* `HU` :  
    the Hounsfield value  
";

%feature("docstring") getSpecularColour "

Accessor on the specular colour of the material of a given polygon mesh.  

Returns
-------
the specular colour  
";

%feature("docstring") destroyWindow "

Close and destroy a given window or OpenGL context.  
";

%feature("docstring") setRootTransformationMatrix "

Set the transformation matrix of the scengraph's root node.  

Parameters
----------
* `aMatrix` :  
    the transformation matrix as a 4x4 array  
";

%feature("docstring") resetBeamSpectrum "

Empty the beam spectrum.  
";

%feature("docstring") getRootTransformationMatrix "

Accessor on the transformation matrix of the scengraph's root node.  

Returns
-------
the scengraph's root node transformation matrix as a 4x4 array  
";

%feature("docstring") computeSinogram "

Compute the sinogram corresponding to the environment that has previously been
set, i.e.  

*   Detector position  
*   Detector orientation  
*   Detector size and resolution  
*   Source position  
*   Source shape  
*   Beam spectrum  
*   Scanned object geometries  
*   Scanned object material properties Note that the rotation vector is the same
    as the up-vector of the detector.  

    Parameters:  
    * `x` :  
        the position of the rotation centre along the X-axis  
    * `y` :  
        the position of the rotation centre along the Y-axis  
    * `z` :  
        the position of the rotation centre along the Z-axis  
    * `aUnitOfLength` :  
        the unit of length corresponding to the x, y and z parameters.
        Acceptable values are: \"um\", \"micrometre\", \"micrometer\", \"mm\",
        \"millimetre\", \"millimeter\", \"cm\", \"centimetre\", \"centimeter\",
        \"dm\", \"decimetre\", \"decimeter\", \"m\", \"metre\", \"meter\",
        \"dam\", \"decametre\", \"decameter\", \"hm\", \"hectometre\",
        \"hectometer\", \"km\", \"kilometre\", \"kilometer\"  
    * `aNumberOfAngle` :  
        the total number of projections in the sinogram  
    * `anAngleOffset` :  
        the angle in degrees between two successive projections  

    Returns:
    the corresponding sinogram  
";

%feature("docstring") getElementAtomicNumber "

Accessor on the chemical element's Z number.  

Parameters
----------
* `anElement` :  
    the name or symbol of the element  

Returns
-------
the Z number of the corresponding element  
";

%feature("docstring") addPolygonMeshAsInnerSurface "

Add a polygon mesh, given its label, to the X-ray renderer as an inner surface.  

Parameters
----------
* `aLabel` :  
    the label of the polygon mesh in the scenegraph  
";

%feature("docstring") showWindow "

Make an OpenGL context visible and display a window.  

Parameters
----------
* `aWindowID` :  
    the numerical ID of the corresponding context (default value: -1, means that
    the active context will be used)  
";

%feature("docstring") getDetectorNumberOfPixels "

Accessor on the number of pixels of the X-ray detector.  

Returns
-------
the size of the detector in number of pixels  
";

