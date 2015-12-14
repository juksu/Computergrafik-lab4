#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "../lib/glm/glm.hpp"
//~ using namespace glm;

class Raytracer
{
	private:
		glm::vec3* image;
		
		glm::vec3 backgroundColor;
		
		glm::vec3 camera;
		glm::vec3 lookAt;
		glm::vec3 up;
		double fov;
		int horizontal;
		int vertical;
		int maxBounces;
		
		
		
		
		
	public:
		
		void setBackgroundColor( glm::vec3 backgroundColor );
		// camera
		void setCameraPosition( glm::vec3 camera );
		void setLookAt( glm::vec3 lookAt );
		void setCameraUp( glm::vec3 up );
		void setHorizontalFOV( double fov );
		void setResolution( int horizontal, int vertical );
		void setMaxBounces( int maxBounces );
		
		glm::vec3* getImage();
		
		//~ void setupViewPlane();
		
		void render();
		
		
			
	
};

#endif // RAYTRACER_H
