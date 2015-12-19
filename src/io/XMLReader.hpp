#ifndef XMLREADER_H
#define XMLREADER_H

#include <string>
#include <vector>
#include "../lib/tinyxml2/tinyxml2.h"
#include "../lib/glm/glm.hpp"

#include "../graphics/surface/Surface.hpp"
//~ #include "../graphics/Material.hpp"
#include "../graphics/material/MaterialSolid.hpp"
#include "../graphics/material/MaterialTextured.hpp"
#include "../graphics/light/Light.hpp"

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
		std::vector<Light*> getLights();
		
		// Surface/Geometry
		std::vector<Surface*> getSurfaces();
		
		void getPhong( tinyxml2::XMLElement* xmlElement, Material* material );
		
		// Material
		MaterialSolid* getMaterialSolid( tinyxml2::XMLElement* xmlMaterialElement );
		MaterialTextured* getMaterialTextured( tinyxml2::XMLElement* xmlMaterialElement );
		void getPhong( tinyxml2::XMLElement xmlMaterialElement, Material* material );
		void getTransformations( tinyxml2::XMLElement* xmlTransformElement, Surface* surface );

		// Transformation
		
		
		void printxml();
};

#endif //XMLREADER_H
