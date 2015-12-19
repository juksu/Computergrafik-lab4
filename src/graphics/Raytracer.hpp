#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "../lib/glm/glm.hpp"
#include "./surface/Surface.hpp"
#include "./light/Light.hpp"
#include "vector"
//~ #include "../lib/glm/vec3.hpp"
//~ #include "../lib/glm/mat4x4.hpp"
//~ using namespace glm;

class Raytracer
{
	private:
		// array where the color information is stored
		glm::vec3* image;
		
		glm::vec3 backgroundColor;
		
		// all the camera stuff
		glm::vec3 camera;
		glm::vec3 center;
		glm::vec3 up;
		double fov;
		int horizontal;
		int vertical;
		int maxBounces;
		
		// objectlist
		std::vector<Surface*> surfaceArray;
		
		// lightlist
		std::vector<Light*> lightArray;
		
		// private functions
		glm::vec3 trace( glm::vec3 point, glm::vec3 ray, int step );
		
	public:
		
		void setBackgroundColor( glm::vec3 backgroundColor ){ this->backgroundColor = backgroundColor; }
		// camera
		void setCameraPosition( glm::vec3 camera ){ this->camera = camera; }
		void setLookAt( glm::vec3 center ){ this->center = center; }
		void setCameraUp( glm::vec3 up ){ this->up = up; }
		void setHorizontalFOV( double fov ){ this->fov = fov; }
		void setResolution( int horizontal, int vertical );
		void setMaxBounces( int maxBounces ){ this->maxBounces = maxBounces; }
		
		// lights
		void setLightArray( std::vector<Light*> lightArray ){ this->lightArray = lightArray; }
		
		// surfaces
		void setSurfaceArray( std::vector<Surface*> surfaceArray ){ this->surfaceArray = surfaceArray; }
		
		void render();
		
		glm::vec3* getImage(){ return image; }
		
		//~ void setupViewPlane();
		
		
};

inline void Raytracer::setResolution( int horizontal, int vertical )
{ 
	this->horizontal = horizontal; this->vertical = vertical; 
}

#endif // RAYTRACER_H
