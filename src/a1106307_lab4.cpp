#include <string>
#include <cstddef>
#include <iostream>
#include "./io/PPMWriter.hpp"
#include "./io/XMLReader.hpp"
#include "./io/PNGio.hpp"
#include "./graphics/Raytracer.hpp"


using namespace std;

int main( int argc, char* argv[] )
{
	std::string xmlFilename;
	std::cout << "argc " << argc << std::endl;

   for(int i = 0; i < argc; i++)
      std::cout << "argv[" << i << "] = " << argv[i] << std::endl;

	if( argc > 1 )
	{
		xmlFilename = argv[1];
		//~ std::cout << xmlFilename << std::endl;
	}
	else
	{
		std::cout << "provide xml file as command line argument" << std::endl;
		return 0;
	}
	
	// load xml
	XMLReader xmlReader;
	xmlReader.loadFile( xmlFilename );
	
	// set up raytracer
	Raytracer raytracer;
	// basic
	raytracer.setBackgroundColor( xmlReader.getBackgroundColor() );
	// camera
	raytracer.setCameraPosition( xmlReader.getCameraPosition() );
	raytracer.setLookAt( xmlReader.getLookAt() );
	raytracer.setCameraUp( xmlReader.getCameraUp() );
	raytracer.setHorizontalFOV( xmlReader.getHorizontalFOV() );
	raytracer.setResolution( xmlReader.getHorizontalResolution(), xmlReader.getVerticalResolution() );
	raytracer.setMaxBounces( xmlReader.getMaxBounces() );
	raytracer.setSupersampling( xmlReader.getSuperSampling() );
	
	raytracer.setSurfaceArray( xmlReader.getSurfaces() );
	
	// get all lights
	raytracer.setLightContainer( xmlReader.getLights() );
	
	// now make some pretty pictures
	raytracer.render();
	
	// write png
	PNGio pngIo;
	pngIo.writePNG( xmlReader.getOutputFilename(), raytracer.getImage(), 
			xmlReader.getHorizontalResolution(), xmlReader.getVerticalResolution() );
	
	// write ppm
	PPMWriter ppmWriter;
	ppmWriter.setResolution( xmlReader.getHorizontalResolution(), xmlReader.getVerticalResolution() );
	ppmWriter.setFilename( xmlReader.getOutputFilename() );
	ppmWriter.writePPM( raytracer.getImage() );
	
}
