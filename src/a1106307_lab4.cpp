#include <string>
#include <cstddef>
#include <iostream>
#include "./lib/tinyxml2/tinyxml2.h"

bool checkXMLResult( tinyxml2::XMLError xmlResult )
{
	if( xmlResult != tinyxml2::XML_SUCCESS )
	{
		std::cerr << "Error reading xml: " << xmlResult << std::endl;
		return true;
	}
	return false;
}

bool loadXML(std::string filename)
{
	tinyxml2::XMLDocument xmlDocument;
	tinyxml2::XMLError xmlResult = xmlDocument.LoadFile( filename.c_str() );
	checkXMLResult( xmlResult );
	
	// pointer to root node
	tinyxml2::XMLNode* xmlRoot = xmlDocument.FirstChild();
	// it may be that we get a nullpointer
	if( !xmlRoot )
		return false;
	
	tinyxml2::XMLElement* xmlElement = xmlRoot->FirstChildElement("IntValue");
	// now Element IntValue provided
	//~ if ( !pElement )
		//~ return false;
	//~ 
	//~ int iOutInt;
	//~ xmlResult = xmlElement->QueryIntText(&iOutInt);
	//~ XMLCheckResult(xmlResult);
	
	
		
	return true;
}

int main()
{
	loadXML("example.xml");
}
