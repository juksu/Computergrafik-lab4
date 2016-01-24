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
#include "../graphics/light/LightContainer.hpp"

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
		glm::dvec3 getBackgroundColor();
		
		// camera
		glm::dvec3 getCameraPosition();
		glm::dvec3 getLookAt();
		glm::dvec3 getCameraUp();
		double getHorizontalFOV();
		size_t getHorizontalResolution();
		size_t getVerticalResolution();
		int getMaxBounces();
		int getSuperSampling();
		
		// light
		//~ std::vector<Light*> getLights();
		LightContainer getLights();
		
		// Surface/Geometry
		std::vector<Surface*> getSurfaces();
		
		void getPhong( tinyxml2::XMLElement* xmlElement, Material* material );
		
		// Material
		MaterialSolid* getMaterialSolid( tinyxml2::XMLElement* xmlMaterialElement );
		MaterialTextured* getMaterialTextured( tinyxml2::XMLElement* xmlMaterialElement );
		void getPhong( tinyxml2::XMLElement xmlMaterialElement, Material* material );
		// Transformation
		void getTransformations( tinyxml2::XMLElement* xmlTransformElement, Surface* surface );
		
		void printxml();
};

#endif //XMLREADER_H
