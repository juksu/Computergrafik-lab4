#ifndef SPHERE_H
#define SPHERE_H

#include "Surface.hpp"
#include "../../lib/glm/glm.hpp"

class Sphere : public Surface
{
	private:
		double radius;
		glm::vec3 position;
		
	public:
		void setRadius( double radius ){ this->radius = radius; }
		double getRadius(){ return radius; }
		void setPosition( glm::vec3 position ){ this->position = position; }
		glm::vec3 getPosition(){ return position; }
		virtual bool intersect( glm::vec3 point, glm::vec3 ray );
		//~ bool intersect( glm::vec3 point, glm::vec3 ray );

};

#endif // SPHERE_H
