#ifndef INTERSECTIONRESULT_H
#define INTERSECTIONRESULT_H
#include "../lib/glm/glm.hpp"

class IntersectionResult
{
	private:
		bool intersection;
		double lambda;
		glm::dvec3 intersectionPoint;
		glm::dvec3 normal;
		glm::dvec3 surfaceColor;
		
	public:
		void setIntersection( bool intersection ){ this->intersection = intersection; }
		bool isIntersection() const { return intersection; }
		
		void setLambda( double lambda ){ this->lambda = lambda; }
		double getLambda() const { return lambda; }
		
		void setIntersectionPoint( glm::dvec3 intersectionPoint ){
			this->intersectionPoint = intersectionPoint; }
		glm::dvec3 getIntersectionPoint() const { return intersectionPoint; }
		
		void setNormal( glm::dvec3 normal ){ this->normal = normal; }
		glm::dvec3 getNormal() const { return normal; }

		void setSurfaceColor( glm::dvec3 surfaceColor ){ this->surfaceColor = surfaceColor; }
		glm::dvec3 getSurfaceColor() const { return surfaceColor; }
		
};
#endif // INTERSECTIONRESULT_H
