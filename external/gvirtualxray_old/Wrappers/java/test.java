import static java.lang.Math.toIntExact;

class gvxrJavaExample
{
    static{ System.loadLibrary("gvxrJava"); }
    public static void main(String[] args)
    {
        // Print the libraries' version
        System.out.println(gvxrJava.getVersionOfCoreGVXR());
        System.out.println(gvxrJava.getVersionOfSimpleGVXR());

        // Create an OpenGL context
        System.out.println("Create an OpenGL context");
        gvxrJava.createWindow();
        gvxrJava.setWindowSize(512, 512);


        // Set up the beam
        System.out.println("Set up the beam");
        gvxrJava.setSourcePosition(-40.0,  0.0, 0.0, "cm");
        gvxrJava.usePointSource();
        //gvxrJava.useParallelBeam();
        gvxrJava.setMonoChromatic(0.08, "MeV", 1000);

        // Set up the detector
        System.out.println("Set up the detector");
        gvxrJava.setDetectorPosition(10.0, 0.0, 0.0, "cm");
        gvxrJava.setDetectorUpVector(0, 1, 0);
        gvxrJava.setDetectorNumberOfPixels(640, 320);
        gvxrJava.setDetectorPixelSize(0.5, 0.5, "mm");

        // Load the data
        System.out.println("Load the data");
        gvxrJava.loadSceneGraph("../data/welsh-dragon-small.stl", "mm");

        // Process every node
		for (int i = 0; i < gvxrJava.getNumberOfChildren("root"); ++i)
		{
		    // Get the label
    		String label = gvxrJava.getChildLabel("root", i);

            System.out.println("Move " + label + " to the centre");
		    gvxrJava.moveToCentre(label);

            //System.out.println("Move the mesh to the center");
            //gvxrJava.moveToCenter(label);

            System.out.println("Set " + label + "'s Hounsfield unit");
		    gvxrJava.setHU(label, (short) 1000);
        }

        // Compute an X-ray image
        System.out.println("Compute an X-ray image");
        gvxrJava.disableArtefactFiltering();
        // Not working anymore gvxrJava.enableArtefactFilteringOnGPU();
        // Not working anymore gvxrJava.enableArtefactFilteringOnCPU();
        Vectorff temp = gvxrJava.computeXRayImage();
        int h = toIntExact(temp.size());
        int w = toIntExact(temp.get(0).size());
        System.out.println(w +"x" + h);

        float x_ray_image[][] = new float[h][w];
        for (int j = 0; j < h; ++j)
        {
            for (int i = 0; i < w; ++i)
            {
                x_ray_image[j][i] = temp.get(j).get(i);
            }
        }

        // Save the last image into a file
        System.out.println("Save the last image into a file");
        gvxrJava.saveLastXRayImage();
        gvxrJava.saveLastLBuffer();

        // Display the 3D scene (no event loop)
        gvxrJava.displayScene();

        // Display the 3D scene (no event loop)
        // Run an interactive loop
        // (can rotate the 3D scene and zoom-in)
        // Keys are:
        // Q/Escape: to quit the event loop (does not close the window)
        // B: display/hide the X-ray beam
        // W: display the polygon meshes in solid or wireframe
        // N: display the X-ray image in negative or positive
        // H: display/hide the X-ray detector
        gvxrJava.renderLoop();
    }
}