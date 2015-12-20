#include <string>
#include <cstddef>
#include <iostream>
#include <vector>
//~ #include "./lib/tinyxml2/tinyxml2.h"
#include "./lib/glm/glm.hpp"
#include "./lib/glm/gtx/string_cast.hpp"
#include "./io/PPMWriter.hpp"
#include "./io/XMLReader.hpp"
#include "./graphics/Raytracer.hpp"

#include "./graphics/surface/Surface.hpp"
//~ #include "./graphics/Sphere.hpp"
//~ #include "./graphics/Material.hpp"


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
	//~ horizontal = xmlReader.getHorizontalResolution();
	//~ vertical = xmlReader.getVerticalResolution();
	//~ xmlReader.printxml();
	
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
	
	//~ vector<Surface*> surfaceArray;
	// get all surfaces
	//~ surfaceArray = xmlReader.getSurfaces();
	raytracer.setSurfaceArray( xmlReader.getSurfaces() );
	
	// get all lights
	raytracer.setLightArray( xmlReader.getLights() );
	
	
	glm::dvec3 test = glm::dvec3(1,2,3);
	test = test * 2.0;
	cout << glm::to_string( glm::dvec3(2,2,2) + test) << endl;
	
	//~ raytracer.setupViewPlane();
	
	//~ glm::vec3 camera = xmlReader.getCameraPosition();
	//~ cout << "camera ";
	//~ for( int i = 0; i < 3; i++ )
		//~ cout << camera[i] << " ";
	//~ cout << endl;
	
	//~ glm::vec3* image = new glm::vec3[horizontal*vertical];		
	//~ for( int i = 0; i < vertical; ++i )
		//~ for( int j = 0; j < horizontal; ++j )			
			//~ image[ i*horizontal + j ] = glm::vec3( i % 256, j % 256, (i * j) % 256 );
	
	raytracer.render();
	
	// write ppm
	PPMWriter ppmWriter;
	ppmWriter.setResolution( xmlReader.getHorizontalResolution(), xmlReader.getVerticalResolution() );
	ppmWriter.setFilename( xmlReader.getOutputFilename() );
	//~ ppmWriter.writePPM(  );
	ppmWriter.writePPM( raytracer.getImage() );
	
}
