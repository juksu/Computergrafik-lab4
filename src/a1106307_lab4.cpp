#include <string>
#include <cstddef>
#include <iostream>
#include "./lib/tinyxml2/tinyxml2.h"

using namespace std;

//~ bool checkXMLResult( tinyxml2::XMLError xmlResult )
//~ {
	//~ if( xmlResult != tinyxml2::XML_SUCCESS )
	//~ {
		//~ std::cerr << "Error reading xml: " << xmlResult << std::endl;
		//~ 
		//~ return false;
	//~ }
	//~ else
		//~ std::cout << "success" << std::endl;
	//~ return true;
//~ }

bool loadXML(std::string filename)
{
	tinyxml2::XMLDocument xmlDocument;
	tinyxml2::XMLError xmlResult = xmlDocument.LoadFile( filename.c_str() );
	//~ tinyxml2::XMLError xmlResult = xmlDocument.LoadFile( "example1.xml" );
	if( xmlResult != tinyxml2::XML_SUCCESS )
	{
		std::cerr << "Error reading xml: " << xmlResult << std::endl;
		return false;
	}
	
	xmlDocument.PrintError();	

	// pointer to root node
	tinyxml2::XMLNode* xmlRoot = xmlDocument.FirstChild();
	// it may be that we get a nullpointer
	if( xmlRoot == nullptr )
	{
		cout << "no xmlRoot" << endl;
		return false;		
	}
	
	// get output filename
	tinyxml2::XMLElement* xmlElement = xmlDocument.FirstChildElement("scene");
	cout << "output filename " << xmlElement->Attribute("output_file") << endl;
	
	// background color
	xmlElement = xmlDocument.FirstChildElement("scene")->FirstChildElement("background_color");
	cout << "background color: r=" 
			<< xmlElement->DoubleAttribute("r") << " g="
			<< xmlElement->DoubleAttribute("g") << " b="
			<< xmlElement->DoubleAttribute("b") << endl;

	// camera
	tinyxml2::XMLElement* xmlCameraElement = xmlDocument.FirstChildElement("scene")->FirstChildElement("camera");
	xmlElement = xmlCameraElement->FirstChildElement("position");
	cout << "camera position: x=" 
			<< xmlElement->DoubleAttribute("x") << " y="
			<< xmlElement->DoubleAttribute("y") << " z="
			<< xmlElement->DoubleAttribute("z") << endl;
	
	xmlElement = xmlCameraElement->FirstChildElement("lookat");
	cout << "look at: x=" 
			<< xmlElement->DoubleAttribute("x") << " y="
			<< xmlElement->DoubleAttribute("y") << " z="
			<< xmlElement->DoubleAttribute("z") << endl;
	
	xmlElement = xmlCameraElement->FirstChildElement("up");
	cout << "up: x=" 
			<< xmlElement->DoubleAttribute("x") << " y="
			<< xmlElement->DoubleAttribute("y") << " z="
			<< xmlElement->DoubleAttribute("z") << endl;
	
	xmlElement = xmlCameraElement->FirstChildElement("horizontal_fov");
	cout << "horizontal_fov: " << xmlElement->DoubleAttribute("angle") << endl;

	xmlElement = xmlCameraElement->FirstChildElement("resolution");
	cout << "resolution: horizontal=" 
			<< xmlElement->IntAttribute("horizontal") << " vertical="
			<< xmlElement->IntAttribute("vertical") << endl;

	xmlElement = xmlCameraElement->FirstChildElement("max_bounces");
	cout << "max_bounces: n=" << xmlElement->IntAttribute("n") << endl;
	
	
	//~ xmlDocument.PrintError();	
	return true;
}

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
	
	loadXML( xmlFilename );
	
	
	//~ int option;
	//~ 
	//~ do
	//~ {
		//~ std::cout << "What you want to do? \n (1) Load xml \n (0) exit \n ";
		//~ std::cin >> option;
		//~ 
		//~ switch( option )
		//~ {
			//~ case 1: std::cout << "filename" << std::endl;
					//~ std::cin >> xmlFilename;
					//~ if( !loadXML(xmlFilename) )
						//~ std::cout << "xml not read successfully" << std::endl;
					//~ break;		
		//~ }
	//~ 
	//~ } while( option != 0 );
	
	
}
