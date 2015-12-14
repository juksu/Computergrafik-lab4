#include <string>
#include <cstddef>
#include <iostream>
#include "./lib/tinyxml2/tinyxml2.h"
#include "./lib/glm/glm.hpp"
#include "./io/PPMWriter.h"
#include "./io/XMLReader.h"
#include "./graphics/Raytracer.h"


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
		std::cout << xmlFilename << std::endl;
		
	}
	else
	{
		std::cout << "provide xml file" << std::endl;
		std::cin >> xmlFilename;
	}
	
	int horizontal;
	int vertical;
	
	// load xml
	XMLReader xmlReader;
	xmlReader.loadFile( xmlFilename );
	horizontal = xmlReader.getHorizontalResolution();
	vertical = xmlReader.getVerticalResolution();
	xmlReader.printxml();
	
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
	
	//~ raytracer.setupViewPlane();
	
	glm::vec3 camera = xmlReader.getCameraPosition();
	cout << "camera ";
	for( int i = 0; i < 3; i++ )
		cout << camera[i] << " ";
	cout << endl;
	
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
