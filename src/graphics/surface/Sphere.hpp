#ifndef SPHERE_H
#define SPHERE_H

#include "Surface.hpp"
#include "../../lib/glm/glm.hpp"

class Sphere : public Surface
{
	private:
		double radius;
		glm::dvec3 position;
		
	public:
		void setRadius( double radius ){ this->radius = radius; }
		double getRadius(){ return radius; }
		void setPosition( glm::dvec3 position ){ this->position = position; }
		glm::dvec3 getPosition(){ return position; }
		virtual IntersectionResult intersect( glm::dvec3 point, glm::dvec3 ray );
		virtual void getIntersectionInformation( glm::dvec3 point, glm::dvec3 ray, 
				IntersectionResult* intersectionResult );
		//~ bool intersect( glm::vec3 point, glm::vec3 ray );

};

#endif // SPHERE_H
