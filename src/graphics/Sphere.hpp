#ifndef SPHERE_H
#define SPHERE_H

#include "Surfaces.hpp"
#include "../lib/glm/glm.hpp"

class Sphere : public Surfaces
{
	private:
		double radius;
		glm::vec3 position;
		
	public:
		void setRadius( double radius ){ this->radius = radius; };
		void setPosition( glm::vec3 position ){ this->position = position; };

};

#endif // SPHERE_H
