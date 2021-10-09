import importlib

class R(object):
    def __init__(self):
        self.gvxr = importlib.import_module('gvxrPython3')

        # Print the libraries' version
        print (self.gvxr.getVersionOfSimpleGVXR())
        print (self.gvxr.getVersionOfCoreGVXR())

        # Create an OpenGL context
        print("Create an OpenGL context")
        self.gvxr.createWindow()
        self.gvxr.setWindowSize(512, 512)

        self.gvxr.setSourcePosition(0.0, 0.5*1040, 0.0, "mm")
        self.gvxr.setDetectorPosition(0.0, -0.5*1040, 0.0, "mm")
        self.gvxr.usePointSource()
        self.gvxr.setMonoChromatic(int(150), "keV", 1000)
        self.gvxr.setDetectorUpVector(0, 0, -1)
        self.gvxr.setDetectorNumberOfPixels(1526, 1496)
        self.gvxr.setDetectorPixelSize(0.25, 0.25, "mm")
        self.gvxr.disableArtefactFiltering()

        # Load the data
        print("Load the data")
        self.gvxr.loadSceneGraph("/data/pipes_A/00001/mesh.stl", "mm")

        # Process every node
        for i in range(self.gvxr.getNumberOfChildren('root')):
            # Get the label
            label = self.gvxr.getChildLabel('root', i)

            print("Move ", label, " to the centre")
            self.gvxr.moveToCentre(label)

            #print("Move the mesh to the center")
            #self.gvxr.moveToCenter(label)

            print("Set ", label, "'s Hounsfield unit")
            self.gvxr.setHU(label, 1000)


        # Compute an X-ray image
        print("Compute an X-ray image")
        self.gvxr.disableArtefactFiltering()
        # Not working anymore self.gvxr.enableArtefactFilteringOnGPU()
        # Not working anymore self.gvxr.enableArtefactFilteringOnCPU()
        x_ray_image = self.gvxr.computeXRayImage()

if __name__ == "__main__":
    R()