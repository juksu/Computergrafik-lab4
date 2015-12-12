#include <string>
#include <cstddef>
#include <iostream>
#include "./lib/tinyxml2/tinyxml2.h"
#include "./io/PPMWriter.h"
#include "./io/XMLReader.h"

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
	
	// load xml
	XMLReader xmlReader;
	xmlReader.loadFile( xmlFilename );
	xmlReader.readxml();
	
	// write ppm
	PPMWriter ppmWriter;
	ppmWriter.setDimensions( 600, 600 );
	ppmWriter.setFilename( xmlReader.getOutputFilename() );
	ppmWriter.writePPM();
	
}
