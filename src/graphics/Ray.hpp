#ifndef RAY_H
#define RAY_H
#include "../lib/glm/glm.hpp"

class Ray
{
	private:
		glm::dvec3 point;
		glm::dvec3 vector;
		glm::dvec3 normal;
	public:
		void setPoint( glm::dvec3 point ){ this->point = point; }
		glm::dvec3 getPoint(){ return point; }
		void setVector( glm::dvec3 vector ){ this->vector = vector; }
		glm::dvec3 getVector(){ return vector; }
		void setNormal( glm::dvec3 normal ){ this->normal = normal; }
		glm::dvec3 getNormal(){ return normal; }
};

#endif // RAY_H
