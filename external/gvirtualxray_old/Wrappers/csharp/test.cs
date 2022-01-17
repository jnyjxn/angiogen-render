using System;

public class RunMe
{
    static public void Main ()
    {
        // Print the libraries' version
        Console.WriteLine(gvxrCSharp.getVersionOfCoreGVXR());
        Console.WriteLine(gvxrCSharp.getVersionOfSimpleGVXR());

        // Create an OpenGL context
        Console.WriteLine("Create an OpenGL context");
		gvxrCSharp.createWindow();
		gvxrCSharp.setWindowSize(512, 512);

		// Set up the beam
		Console.WriteLine("Set up the beam");
		gvxrCSharp.setSourcePosition(-40.0,  0.0, 0.0, "cm");
		gvxrCSharp.usePointSource();
		//gvxrCSharp.useParallelBeam();
		gvxrCSharp.setMonoChromatic(0.08, "MeV", 1000);

		// Set up the detector
		Console.WriteLine("Set up the detector");
		gvxrCSharp.setDetectorPosition(10.0, 0.0, 0.0, "cm");
		gvxrCSharp.setDetectorUpVector(0, 1, 0);
		gvxrCSharp.setDetectorNumberOfPixels(640, 320);
		gvxrCSharp.setDetectorPixelSize(0.5, 0.5, "mm");

		// Load the data
		Console.WriteLine("Load the data");
		gvxrCSharp.loadSceneGraph("../data/welsh-dragon-small.stl", "mm");

		// Process every node
		for (uint i = 0; i < gvxrCSharp.getNumberOfChildren("root"); ++i)
		{
		    // Get the label
		    string label = gvxrCSharp.getChildLabel("root", i);

		    Console.WriteLine("Move ", label, " to the centre");
		    gvxrCSharp.moveToCentre(label);
		    //gvxrCSharp.moveToCenter(label);

		    Console.WriteLine("Set ", label, "'s Hounsfield unit");
		    gvxrCSharp.setHU(label, 1000);
		}

		// Compute an X-ray image
		Console.WriteLine("Compute an X-ray image");
		gvxrCSharp.disableArtefactFiltering();
		// Not working anymore gvxrCSharp.enableArtefactFilteringOnGPU();
		// Not working anymore gvxrCSharp.enableArtefactFilteringOnCPU();
		gvxrCSharp.computeXRayImage();

		// Save the last image into a file
		Console.WriteLine("Save the last image into a file");
		gvxrCSharp.saveLastXRayImage();
		gvxrCSharp.saveLastLBuffer();

		// Display the 3D scene (no event loop)
		gvxrCSharp.displayScene();

		// Display the 3D scene (no event loop)
		// Run an interactive loop
		// (can rotate the 3D scene and zoom-in)
		// Keys are:
		// Q/Escape: to quit the event loop (does not close the window)
		// B: display/hide the X-ray beam
		// W: display the polygon meshes in solid or wireframe
		// N: display the X-ray image in negative or positive
		// H: display/hide the X-ray detector
		gvxrCSharp.renderLoop();
	}
}
