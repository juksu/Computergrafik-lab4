#include "XMLReader.hpp"
#include <iostream>
#include <string>

#include "../graphics/Sphere.hpp"

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

/*
 * basic
 */
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

glm::vec3 XMLReader::getBackgroundColor()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("background_color");
		
	glm::vec3 backgroundColor = glm::vec3(
			xmlElement->DoubleAttribute("r"),
			xmlElement->DoubleAttribute("g"),
			xmlElement->DoubleAttribute("b") );
	
	return backgroundColor;
}
/*
 * basic END
 */

/*
 * camera
 */
glm::vec3 XMLReader::getCameraPosition()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("camera")->FirstChildElement("position");
	
	glm::vec3 cameraPosition = glm::vec3(
			xmlElement->DoubleAttribute("x"),
			xmlElement->DoubleAttribute("y"),
			xmlElement->DoubleAttribute("z") );
			
	return cameraPosition;
}

glm::vec3 XMLReader::getLookAt()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("camera")->FirstChildElement("lookat");
	
	glm::vec3 lookAt = glm::vec3(
			xmlElement->DoubleAttribute("x"),
			xmlElement->DoubleAttribute("y"),
			xmlElement->DoubleAttribute("z") );
	
	return lookAt;
}

glm::vec3 XMLReader::getCameraUp()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("camera")->FirstChildElement("up");
	
	glm::vec3 up = glm::vec3(
			xmlElement->DoubleAttribute("x"),
			xmlElement->DoubleAttribute("y"),
			xmlElement->DoubleAttribute("z") );
	
	return up;
}

double XMLReader::getHorizontalFOV()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("camera")->FirstChildElement("horizontal_fov");
	
	return xmlElement->DoubleAttribute( "angle" );
}

int XMLReader::getHorizontalResolution()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("camera")->FirstChildElement("resolution");
			
	return xmlElement->IntAttribute("horizontal");		
}

int XMLReader::getVerticalResolution()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("camera")->FirstChildElement("resolution");
			
	return xmlElement->IntAttribute("vertical");
}

int XMLReader::getMaxBounces()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("camera")->FirstChildElement("max_bounces");
			
	return xmlElement->IntAttribute("n");
}
/*
 * camera END
 */
 
 /*
  * Lights
  */

/*
 * Surfaces
 */
std::vector<Surfaces> XMLReader::getSurfaces()
{
	std::vector<Surfaces> surfaceArray;
	
	XMLElement* xmlSurfaceElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("surfaces")->FirstChildElement();
	
	XMLElement* xmlElement;
	//~ XMLElement* xmlMaterialElement;
	
	while( xmlSurfaceElement != nullptr )
	{
		std::string type = xmlSurfaceElement->Value();
		
		if( type.compare("sphere") == 0 )
		{
			Sphere sphere;
			sphere.setRadius( xmlSurfaceElement->DoubleAttribute("radius") );
			
			xmlElement = xmlSurfaceElement->FirstChildElement("position");
			
			glm::vec3 position = glm::vec3( xmlElement->DoubleAttribute("x"), 
					xmlElement->DoubleAttribute("y"), xmlElement->DoubleAttribute("z") );
			sphere.setPosition( position );
			
			/// TODO from here it is for all surfaces the same -> change code
			xmlElement = xmlSurfaceElement->FirstChildElement("material_solid");
			if( xmlElement != nullptr )
				sphere.setMaterial( getMaterialSolid( xmlElement ) );
			
			xmlElement = xmlSurfaceElement->FirstChildElement("material_textured");
			if( xmlElement != nullptr )
				sphere.setMaterial( getMaterialTextured( xmlElement ) );
			
			xmlElement = xmlSurfaceElement->FirstChildElement("transform");
			if( xmlElement != nullptr )	// in case no transformations exist
				getTransformations( xmlElement, &sphere );
			
			
			/// TODO continue
			
			//~ surfaceArray.push_back
			
		}
	
		std::cout << xmlSurfaceElement->Value() << std::endl;
		xmlSurfaceElement = xmlSurfaceElement->NextSiblingElement();
	}
	return surfaceArray;
}

MaterialSolid XMLReader::getMaterialSolid( XMLElement* xmlMaterialElement )
{
	MaterialSolid material;
	
	XMLElement* xmlElement = xmlMaterialElement->FirstChildElement("color");
	glm::vec3 color = glm::vec3( xmlElement->DoubleAttribute("x"), 
			xmlElement->DoubleAttribute("y"), xmlElement->DoubleAttribute("z") );
	material.setColor( color );
	
	getPhong( xmlMaterialElement, &material );
	
	xmlElement = xmlMaterialElement->FirstChildElement("reflectance");
	material.setReflactance( xmlElement->DoubleAttribute("r") );
	
	xmlElement = xmlMaterialElement->FirstChildElement("transmittance");
	material.setTransmittance( xmlElement->DoubleAttribute("t") );
	
	xmlElement = xmlMaterialElement->FirstChildElement("refraction");
	material.setTransmittance( xmlElement->DoubleAttribute("iof") );
	
	return material;
}

MaterialTextured XMLReader::getMaterialTextured( XMLElement* xmlMaterialElement )
{
	MaterialTextured material;
	/// TODO
	return material;
}

//~ void XMLReader::getPhong( XMLElement* xmlElement, Material* material )
void XMLReader::getPhong( XMLElement* xmlMaterialElement, Material* material )
{
	XMLElement* xmlElement = xmlMaterialElement->FirstChildElement("phong");
	material->setPhong( xmlElement->DoubleAttribute("ka"), xmlElement->DoubleAttribute("kd"),
			xmlElement->DoubleAttribute("ks"), xmlElement->IntAttribute("exponent") );
}

void XMLReader::getTransformations( XMLElement* xmlTransformElement, Surfaces* surface )
{
	XMLElement* xmlElement = xmlTransformElement->FirstChildElement();
	while( xmlElement != nullptr )
	{
		std::cout << "I make a transformation" << std::endl;
		std::string type = xmlElement->Value();
		if( type.compare("translate") == 0 )
		{
			glm::vec3 translate = glm::vec3( xmlElement->DoubleAttribute("x"), 
					xmlElement->DoubleAttribute("y"), xmlElement->DoubleAttribute("z") );
			surface->translate( translate );
		}
		else if( type.compare("scale") == 0 )
		{
			glm::vec3 scale = glm::vec3( xmlElement->DoubleAttribute("x"), 
					xmlElement->DoubleAttribute("y"), xmlElement->DoubleAttribute("z") );
			surface->scale( scale );
		}
		else if( type.compare("rotateX") == 0 )
		{
			surface->rotateX( xmlElement->DoubleAttribute("theta") );
		}
		else if( type.compare("rotateY") == 0 )
		{
			surface->rotateY( xmlElement->DoubleAttribute("theta") );
		}
		else if( type.compare("rotateZ") == 0 )
		{
			surface->rotateZ( xmlElement->DoubleAttribute("theta") );
		}
		/// TODO: Ask if there will be also shear.
		
		xmlElement->NextSiblingElement();
	}
	std::cout << surface->printTransformationMatrix() << std::endl;
}

/*
 * print xml (only part of it)
 */
void XMLReader::printxml()
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
