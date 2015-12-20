#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "IntersectionResult.hpp"
#include "./light/Light.hpp"
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
		std::vector<Light*> lightArray;
		
		// private functions
		glm::dvec3 shade( IntersectionResult intersection, Surface* objectNumber );
		glm::dvec3 trace( glm::dvec3 point, glm::dvec3 ray, int step );
		
	public:
		
		void setBackgroundColor( glm::dvec3 backgroundColor ){ this->backgroundColor = backgroundColor; }
		// camera
		void setCameraPosition( glm::dvec3 camera ){ this->camera = camera; }
		void setLookAt( glm::dvec3 center ){ this->center = center; }
		void setCameraUp( glm::dvec3 up ){ this->up = up; }
		void setHorizontalFOV( double fov ){ this->fov = fov; }
		void setResolution( int horizontal, int vertical );
		void setMaxBounces( int maxBounces ){ this->maxBounces = maxBounces; }
		
		// lights
		void setLightArray( std::vector<Light*> lightArray ){ this->lightArray = lightArray; }
		
		// surfaces
		void setSurfaceArray( std::vector<Surface*> surfaceArray ){ this->surfaceArray = surfaceArray; }
		
		void render();
		
		glm::dvec3* getImage(){ return image; }
		
		//~ void setupViewPlane();
		
		
};

inline void Raytracer::setResolution( int horizontal, int vertical )
{ 
	this->horizontal = horizontal; this->vertical = vertical; 
}

#endif // RAYTRACER_H
