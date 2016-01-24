#include "XMLReader.hpp"
#include "WavefrontOBJReader.hpp"
#include "PNGio.hpp"
#include "../graphics/light/AmbientLight.hpp"
#include "../graphics/light/PointLight.hpp"
#include "../graphics/light/ParallelLight.hpp"
#include "../graphics/light/SpotLight.hpp"
#include "../graphics/surface/Sphere.hpp"
#include "../graphics/surface/Mesh.hpp"
#include "../lib/glm/gtx/string_cast.hpp"


#include <iostream>
#include <string>

using namespace tinyxml2;

bool XMLReader::loadFile( std::string filename )
{
	std::cout << "reading xml " << filename << std::endl;
	
	//~ XMLDocument xmlDocument;
	this->filename = filename;
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
		//~ std::string fn = xmlElement->Attribute("output_file");
		//~ 
		//~ int fileExt = fn.rfind(".");
	//~ 
		//~ return fn.substr(0, fileExt).append(".ppm");
		return xmlElement->Attribute("output_file");
	}
	else
	{
		std::cout << "no output name found, out.png used instead" << std::endl;
	}
	// if no filename can be found use standard name
	return "out.png";
}

glm::dvec3 XMLReader::getBackgroundColor()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("background_color");
		
	glm::dvec3 backgroundColor = glm::dvec3(
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
glm::dvec3 XMLReader::getCameraPosition()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("camera")->FirstChildElement("position");
	
	glm::dvec3 cameraPosition = glm::dvec3(
			xmlElement->DoubleAttribute("x"),
			xmlElement->DoubleAttribute("y"),
			xmlElement->DoubleAttribute("z") );
			
	return cameraPosition;
}

glm::dvec3 XMLReader::getLookAt()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("camera")->FirstChildElement("lookat");
	
	glm::dvec3 lookAt = glm::dvec3(
			xmlElement->DoubleAttribute("x"),
			xmlElement->DoubleAttribute("y"),
			xmlElement->DoubleAttribute("z") );
	
	return lookAt;
}

glm::dvec3 XMLReader::getCameraUp()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("camera")->FirstChildElement("up");
	
	glm::dvec3 up = glm::dvec3(
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

size_t XMLReader::getHorizontalResolution()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("camera")->FirstChildElement("resolution");
			
	return xmlElement->UnsignedAttribute("horizontal");		
}

size_t XMLReader::getVerticalResolution()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("camera")->FirstChildElement("resolution");
			
	return xmlElement->UnsignedAttribute("vertical");
}

int XMLReader::getMaxBounces()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("camera")->FirstChildElement("max_bounces");
			
	return xmlElement->IntAttribute("n");
}

int XMLReader::getSuperSampling()
{
	XMLElement* xmlElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("camera")->FirstChildElement("supersampling");
	
	if( xmlElement == nullptr )
		return -1;
	else
		return xmlElement->IntAttribute("samplerate");
}
/*
 * camera END
 */
 
 /*
  * lights
  */
//~ std::vector<Light*> XMLReader::getLights()
LightContainer XMLReader::getLights()
{
	//~ std::vector<Light*> lightArray;
	
	LightContainer lights;
	
	XMLElement* xmlLightElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("lights")->FirstChildElement();
	
	XMLElement* xmlElement;
	
	while( xmlLightElement != nullptr )
	{
		std::string type = xmlLightElement->Value();
		
		if( type.compare("ambient_light") == 0 )
		{
			AmbientLight* light = new AmbientLight();
			
			xmlElement = xmlLightElement->FirstChildElement("color");
			glm::dvec3 color = glm::dvec3( xmlElement->DoubleAttribute("r"), 
					xmlElement->DoubleAttribute("g"), xmlElement->DoubleAttribute("b") );
			light->setColor( color );
			
			lights.addAmbientLight( light );
			//~ lightArray.push_back(light);
		}
		
		if( type.compare("point_light") == 0 )
		{
			PointLight* light = new PointLight();
			
			xmlElement = xmlLightElement->FirstChildElement("color");
			glm::dvec3 color = glm::dvec3( xmlElement->DoubleAttribute("r"), 
					xmlElement->DoubleAttribute("g"), xmlElement->DoubleAttribute("b") );
			light->setColor( color );
			
			xmlElement = xmlLightElement->FirstChildElement("position");
			glm::dvec3 position = glm::dvec3( xmlElement->DoubleAttribute("x"), 
					xmlElement->DoubleAttribute("y"), xmlElement->DoubleAttribute("z") );
			light->setPosition( position );
			
			//~ lightArray.push_back(light);
			lights.addPointLight( light );
		}
		
		if( type.compare("parallel_light") == 0 )
		{
			ParallelLight* light = new ParallelLight();
			
			xmlElement = xmlLightElement->FirstChildElement("color");
			glm::dvec3 color = glm::dvec3( xmlElement->DoubleAttribute("r"), 
					xmlElement->DoubleAttribute("g"), xmlElement->DoubleAttribute("b") );
			light->setColor( color );
			
			xmlElement = xmlLightElement->FirstChildElement("direction");
			glm::dvec3 direction = glm::dvec3( xmlElement->DoubleAttribute("x"), 
					xmlElement->DoubleAttribute("y"), xmlElement->DoubleAttribute("z") );
			light->setDirection( direction );
			
			//~ lightArray.push_back(light);
			lights.addParallelLight( light );
		}
		
		if( type.compare("spot_light") == 0 )
		{
			SpotLight* light = new SpotLight();
			
			xmlElement = xmlLightElement->FirstChildElement("color");
			glm::dvec3 color = glm::dvec3( xmlElement->DoubleAttribute("r"), 
					xmlElement->DoubleAttribute("g"), xmlElement->DoubleAttribute("b") );
			light->setColor( color );
			
			xmlElement = xmlLightElement->FirstChildElement("position");
			glm::dvec3 position = glm::dvec3( xmlElement->DoubleAttribute("x"), 
					xmlElement->DoubleAttribute("y"), xmlElement->DoubleAttribute("z") );
			light->setPosition( position );
			
			xmlElement = xmlLightElement->FirstChildElement("direction");
			glm::dvec3 direction = glm::dvec3( xmlElement->DoubleAttribute("x"), 
					xmlElement->DoubleAttribute("y"), xmlElement->DoubleAttribute("z") );
			light->setDirection( direction );
			
			xmlElement = xmlLightElement->FirstChildElement("falloff");
			light->setFalloffAlpha1( xmlElement->DoubleAttribute("alpha1") ); 
			light->setFalloffAlpha2( xmlElement->DoubleAttribute("alpha2") ); 
			
			//~ lightArray.push_back(light);
			lights.addSpotlLight( light );
		}
		
		//~ // set color
		//~ xmlElement = xmlLightElement->FirstChildElement("color");
		//~ glm::dvec3 color = glm::dvec3( xmlElement->DoubleAttribute("r"), 
				//~ xmlElement->DoubleAttribute("g"), xmlElement->DoubleAttribute("b") );
		//~ lightArray.back()->setColor( color );
		
		xmlLightElement = xmlLightElement->NextSiblingElement();
	}
	return lights;
}
/*
 * lights END
 */

/*
 * surfaces
 */
std::vector<Surface*> XMLReader::getSurfaces()
{
	std::vector<Surface*> surfaceArray;
	
	XMLElement* xmlSurfaceElement = xmlDocument.FirstChildElement("scene")
			->FirstChildElement("surfaces")->FirstChildElement();
	
	XMLElement* xmlElement;
	
	while( xmlSurfaceElement != nullptr )
	{
		std::string type = xmlSurfaceElement->Value();
		
		if( type.compare("sphere") == 0 )
		{
			//~ Sphere* sphere = new Sphere();
			Sphere* sphere = new Sphere();
			sphere->setRadius( xmlSurfaceElement->DoubleAttribute("radius") );
			//~ std::cout << "radius " << sphere->getRadius() << std::endl;
			
			xmlElement = xmlSurfaceElement->FirstChildElement("position");
			
			glm::dvec3 position = glm::dvec3( xmlElement->DoubleAttribute("x"), 
					xmlElement->DoubleAttribute("y"), xmlElement->DoubleAttribute("z") );
			sphere->setPosition( position );
			
			surfaceArray.push_back( sphere );			
		}
		
		if( type.compare("mesh") == 0 )
		{
			Mesh* mesh = new Mesh();
			WavefrontOBJReader objReader;
			
			// if the file is executed from a different path where the xml is saved (which is usually the case)
			// we need to add the path information to the filename 
			// assuming the .obj file is at the same location as the .xml file
			std::string fn = filename;
			size_t i = fn.rfind( "/" );
			fn.erase( i+1, fn.length() );
			fn.append( xmlSurfaceElement->Attribute("name") );
			
			objReader.readOBJ( fn, mesh );
			//~ objReader.readOBJ( "./xml/plane_small.obj", mesh );
			std::cout << "read obj name " << fn << std::endl;
			
			surfaceArray.push_back( mesh );
			
			//~ mesh->printMesh();
		}
		
		xmlElement = xmlSurfaceElement->FirstChildElement("material_solid");
		if( xmlElement != nullptr )
			surfaceArray.back()->setMaterial( getMaterialSolid( xmlElement ) );
		
		xmlElement = xmlSurfaceElement->FirstChildElement("material_textured");
		if( xmlElement != nullptr )
			surfaceArray.back()->setMaterial( getMaterialTextured( xmlElement ) );
		
		xmlElement = xmlSurfaceElement->FirstChildElement("transform");
		if( xmlElement != nullptr )	// in case no transformations exist
			getTransformations( xmlElement, surfaceArray.back() );
		
		//~ std::cout << xmlSurfaceElement->Value() << std::endl;
		xmlSurfaceElement = xmlSurfaceElement->NextSiblingElement();
	}
	return surfaceArray;
}

MaterialSolid* XMLReader::getMaterialSolid( XMLElement* xmlMaterialElement )
{
	MaterialSolid* material = new MaterialSolid();
	//~ std::cout << xmlMaterialElement->Value() << std::endl;
	
	XMLElement* xmlElement = xmlMaterialElement->FirstChildElement("color");
	glm::dvec3 color = glm::dvec3( xmlElement->DoubleAttribute("r"), 
			xmlElement->DoubleAttribute("g"), xmlElement->DoubleAttribute("b") );
	material->setColor( color );
	
	getPhong( xmlMaterialElement, material );
	
	xmlElement = xmlMaterialElement->FirstChildElement("reflectance");
	material->setReflectance( xmlElement->DoubleAttribute("r") );
	
	xmlElement = xmlMaterialElement->FirstChildElement("transmittance");
	material->setTransmittance( xmlElement->DoubleAttribute("t") );
	
	xmlElement = xmlMaterialElement->FirstChildElement("refraction");
	material->setRefractionIndex( xmlElement->DoubleAttribute("iof") );
	
	return material;
}

MaterialTextured* XMLReader::getMaterialTextured( XMLElement* xmlMaterialElement )
{
	MaterialTextured* material = new MaterialTextured();
	
	PNGio pngIo;
	XMLElement* xmlElement = xmlMaterialElement->FirstChildElement("texture");
	
	// if the file is executed from a different path where the xml is saved (which is usually the case)
	// we need to add the path information to the filename 
	// assuming the .texture file is at the same location as the .xml file
	std::string fn = filename;
	size_t i = fn.rfind( "/" );
	fn.erase( i+1, fn.length() );
	fn.append( xmlElement->Attribute("name") );
	
	//~ objReader.readOBJ( fn, mesh );

	material->setTexture( pngIo.readPNG( fn ) );
	material->setDimension( pngIo.getWidth(), pngIo.getHeight() );
	
	getPhong( xmlMaterialElement, material );
	
	/// TODO test if texture was read correctly
	pngIo.writePNG( "test.png", material->getTexture(), pngIo.getWidth(), pngIo.getHeight() );
	
	xmlElement = xmlMaterialElement->FirstChildElement("reflectance");
	material->setReflectance( xmlElement->DoubleAttribute("r") );
	
	xmlElement = xmlMaterialElement->FirstChildElement("transmittance");
	material->setTransmittance( xmlElement->DoubleAttribute("t") );
	
	xmlElement = xmlMaterialElement->FirstChildElement("refraction");
	material->setRefractionIndex( xmlElement->DoubleAttribute("iof") );

	return material;
}

//~ void XMLReader::getPhong( XMLElement* xmlElement, Material* material )
void XMLReader::getPhong( XMLElement* xmlMaterialElement, Material* material )
{
	XMLElement* xmlElement = xmlMaterialElement->FirstChildElement("phong");
	material->setPhong( xmlElement->DoubleAttribute("ka"), xmlElement->DoubleAttribute("kd"),
			xmlElement->DoubleAttribute("ks"), xmlElement->IntAttribute("exponent") );
}

void XMLReader::getTransformations( XMLElement* xmlTransformElement, Surface* surface )
{	
	XMLElement* xmlElement = xmlTransformElement->FirstChildElement();
	while( xmlElement != nullptr )
	{
		std::string type = xmlElement->Value();
		//~ std::cout << "I make a transformation of " << type << std::endl;
		//~ std::string type = xmlElement->Value();
		if( type.compare("translate") == 0 )
		{
			glm::dvec3 translate = glm::dvec3( xmlElement->DoubleAttribute("x"), 
					xmlElement->DoubleAttribute("y"), xmlElement->DoubleAttribute("z") );
			surface->translate( translate );
		}
		else if( type.compare("scale") == 0 )
		{
			glm::dvec3 scale = glm::dvec3( xmlElement->DoubleAttribute("x"), 
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
		
		xmlElement = xmlElement->NextSiblingElement();
		//~ xmlSurfaceElement = xmlSurfaceElement->NextSiblingElement();
	}
	//~ std::cout << surface->transformationMatrixString() << std::endl;
}
/*
 * surfaces END
 */

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
