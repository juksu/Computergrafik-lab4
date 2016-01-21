#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "IntersectionResult.hpp"
#include "./light/LightContainer.hpp"
#include "./surface/Surface.hpp"
#include "../lib/glm/glm.hpp"
#include <vector>

//~ #include "../lib/glm/vec3.hpp"
//~ #include "../lib/glm/mat4x4.hpp"
//~ using namespace glm;

class Raytracer
{
	private:
		// array where the color information is stored
		glm::dvec3* image;
		
		glm::dvec3 backgroundColor;
		
		// all the camera stuff
		glm::dvec3 camera;
		glm::dvec3 center;
		glm::dvec3 up;
		double fov;
		int horizontal;
		int vertical;
		int maxBounces;
		
		// objectlist
		std::vector<Surface*> surfaceArray;
		
		// lightlist
		//~ std::vector<Light*> lightArray;
		LightContainer lightContainer;
		
		// private functions
		glm::dvec3 shade( const IntersectionResult* const intersection, const Surface* const objectNumber, 
				const glm::dvec3* const eyeVector ) const;
		glm::dvec3 trace( glm::dvec3* point, glm::dvec3* ray, const glm::dvec3* const eyeVector, 
				int step, double refractionIndex ) const;
		glm::dvec3 ambientLight( double phongKA, glm::dvec3 lightColor ) const;
		glm::dvec3 diffuseLight( double phongKD, glm::dvec3 lightColor, 
				glm::dvec3 lightVector, glm::dvec3 normalVector ) const;
		glm::dvec3 specularLight( double phongKS, double phongExponent, glm::dvec3 lightColor, 
				glm::dvec3 lightVector, glm::dvec3 normalVector, glm::dvec3 eyeVector ) const;
		
		
	public:
		~Raytracer()
		{
			delete[] image;
		}
		
		void setBackgroundColor( glm::dvec3 backgroundColor ){ this->backgroundColor = backgroundColor; }
		// camera
		void setCameraPosition( glm::dvec3 camera ){ this->camera = camera; }
		void setLookAt( glm::dvec3 center ){ this->center = center; }
		void setCameraUp( glm::dvec3 up ){ this->up = up; }
		void setHorizontalFOV( double fov ){ this->fov = fov; }
		void setResolution( int horizontal, int vertical ) {
			this->horizontal = horizontal; this->vertical = vertical; }
		void setMaxBounces( int maxBounces ){ this->maxBounces = maxBounces; }
		
		// lights
		//~ void setLightArray( std::vector<Light*> lightArray ){ this->lightArray = lightArray; }
		void setLightContainer( LightContainer lightContainer ){ this->lightContainer = lightContainer; }
		
		// surfaces
		void setSurfaceArray( std::vector<Surface*> surfaceArray ){ this->surfaceArray = surfaceArray; }
		
		void render();
		
		glm::dvec3* getImage() const { return image; }
		
		//~ void setupViewPlane();
		
		
};

#endif // RAYTRACER_H
