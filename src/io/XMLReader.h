#ifndef XMLREADER_H
#define XMLREADER_H

#include <string>
#include "../lib/tinyxml2/tinyxml2.h"
#include "../lib/glm/glm.hpp"

class XMLReader
{
	private:
		std::string filename;
		tinyxml2::XMLDocument xmlDocument;
		
	public:
		XMLReader(){ tinyxml2::XMLDocument xmlDocument; };
		bool loadFile( std::string filename );
		
		// basic
		std::string getOutputFilename();
		glm::vec3 getBackgroundColor();
		
		// camera
		glm::vec3 getCameraPosition();
		glm::vec3 getLookAt();
		glm::vec3 getCameraUp();
		double getHorizontalFOV();
		int getHorizontalResolution();
		int getVerticalResolution();
		int getMaxBounces();
		
		// light
		
		
		// Surface/Geometry
		
		// Material
		
		// Transformation
		
		
		void printxml();
};

#endif //XMLREADER_H
