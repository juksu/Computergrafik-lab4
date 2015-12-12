#include "XMLReader.h"
#include <iostream>

using namespace tinyxml2;

bool XMLReader::loadFile( std::string filename )
{
	//~ XMLDocument xmlDocument;
	XMLError xmlResult = xmlDocument.LoadFile( filename.c_str() );
	//~ tinyxml2::XMLError xmlResult = xmlDocument.LoadFile( "example1.xml" );
	if( xmlResult != XML_SUCCESS )
	{
		std::cerr << "Error reading xml: " << xmlResult << std::endl;
		xmlDocument.PrintError();
		return false;
	}
	
	return true;
}

std::string XMLReader::getOutputFilename()
{
	// get output filename
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene");
	
	if( xmlElement != nullptr )
	{
		std::string fn = xmlElement->Attribute("output_file");
		
		int fileExt = fn.rfind(".");
	
		return fn.substr(0, fileExt).append(".ppm");
	}
	else
	{
		std::cout << "no output name found, out.ppm used instead" << std::endl;
	}
	// if no filename can be found use standard name
	return "out.ppm";
}

void XMLReader::readxml()
{

	// pointer to root node
	XMLNode* xmlRoot = xmlDocument.FirstChild();
	// it may be that we get a nullpointer
	if( xmlRoot == nullptr )
	{
		std::cerr << "no xmlRoot" << std::endl;
	}
	
	// get output filename
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene");
	std::cout << "output filename " << xmlElement->Attribute("output_file") << std::endl;
	
	// background color
	xmlElement = xmlDocument.FirstChildElement("scene")->FirstChildElement("background_color");
	std::cout << "background color: r=" 
			<< xmlElement->DoubleAttribute("r") << " g="
			<< xmlElement->DoubleAttribute("g") << " b="
			<< xmlElement->DoubleAttribute("b") << std::endl;

	// camera
	XMLElement* xmlCameraElement = xmlDocument.FirstChildElement("scene")->FirstChildElement("camera");
	xmlElement = xmlCameraElement->FirstChildElement("position");
	std::cout << "camera position: x=" 
			<< xmlElement->DoubleAttribute("x") << " y="
			<< xmlElement->DoubleAttribute("y") << " z="
			<< xmlElement->DoubleAttribute("z") << std::endl;
	
	xmlElement = xmlCameraElement->FirstChildElement("lookat");
	std::cout << "look at: x=" 
			<< xmlElement->DoubleAttribute("x") << " y="
			<< xmlElement->DoubleAttribute("y") << " z="
			<< xmlElement->DoubleAttribute("z") << std::endl;
	
	xmlElement = xmlCameraElement->FirstChildElement("up");
	std::cout << "up: x=" 
			<< xmlElement->DoubleAttribute("x") << " y="
			<< xmlElement->DoubleAttribute("y") << " z="
			<< xmlElement->DoubleAttribute("z") << std::endl;
	
	xmlElement = xmlCameraElement->FirstChildElement("horizontal_fov");
	std::cout << "horizontal_fov: " << xmlElement->DoubleAttribute("angle") << std::endl;

	xmlElement = xmlCameraElement->FirstChildElement("resolution");
	std::cout << "resolution: horizontal=" 
			<< xmlElement->IntAttribute("horizontal") << " vertical="
			<< xmlElement->IntAttribute("vertical") << std::endl;

	xmlElement = xmlCameraElement->FirstChildElement("max_bounces");
	std::cout << "max_bounces: n=" << xmlElement->IntAttribute("n") << std::endl;
	
	xmlDocument.PrintError();	
}
