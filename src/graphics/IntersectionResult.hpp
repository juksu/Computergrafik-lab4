#ifndef INTERSECTIONRESULT_H
#define INTERSECTIONRESULT_H
#include "../lib/glm/glm.hpp"

class IntersectionResult
{
	private:
		bool intersection;
		double alpha;
		glm::dvec3 intersectionPoint;
		glm::dvec3 normal;
		//~ glm::dvec3 reflectionVector;
		bool transmittance;
		//~ glm::dvec3 refractionVector;		/// wait, do we have refraction???
												/// only tranmittance meaning no change of direction
		glm::dvec3 surfaceColor;
		
	public:
		void setIntersection( bool intersection ){ this->intersection = intersection; }
		bool isIntersection(){ return intersection; }
		void setAlpha( double alpha ){ this->alpha = alpha; }
		double getAlpha(){ return alpha; }
		void setIntersectionPoint( glm::dvec3 intersectionPoint ){
			this->intersectionPoint = intersectionPoint; }
		glm::dvec3 getIntersectionPoint(){ return intersectionPoint; }
		
		void setNormal( glm::dvec3 normal ){ this->normal = normal; }
		glm::dvec3 getNormal(){ return normal; }
		
		//~ void setReflectionVector( glm::dvec3 reflectionVector ){ 
			//~ this->reflectionVector = reflectionVector; }
		//~ glm::dvec3 getReflectionVector(){ return reflectionVector; }
		
		void setTransmittance( bool transmittance ){ this->transmittance = transmittance; }
		bool isTransmittance(){ return transmittance; }
		//~ void setRefractionVector( glm::dvec3 refractionVector ){ 
			//~ this->refractionVector = refractionVector; }
		//~ glm::dvec3 getRefractionVector(){ return refractionVector; }
		void setSurfaceColor( glm::dvec3 surfaceColor ){ this->surfaceColor = surfaceColor; }
		glm::dvec3 getSurfaceColor(){ return surfaceColor; }
		
};
#endif // INTERSECTIONRESULT_H
