#include "Sphere.hpp"
#include "../../lib/glm/gtc/matrix_inverse.hpp"
//~ #include "../../lib/glm/gtx/string_cast.hpp"
#include <iostream>
#include <limits>

using namespace glm;

IntersectionResult* Sphere::intersect( dvec3 point, dvec3 ray )
{
	IntersectionResult* intersectionResult = new IntersectionResult();
	intersectionResult->setIntersection( false );
	//~ double lambda = std::numeric_limits<double>::max();
	
	// transform point and ray into object coordinates with inverse transformation matrix
	// all transformations are affine, so affineInverse should work fine.
	/// TODO, for optimization i may calculate this only once (when setting transformation maybe?)
	dmat4 inverseTransformations = affineInverse( getTransformationMatrix() );
	
	dvec3 pointTransformed = dvec3( inverseTransformations * dvec4( point, 1 ) );
	dvec3 rayTransformed = dvec3( inverseTransformations * dvec4( ray, 0 ) );
	
	//intersection test
	double a = dot( rayTransformed, rayTransformed );
	double b = 2 * dot( pointTransformed, rayTransformed ) - 2 * dot( position, rayTransformed );
	double c = dot( pointTransformed - position, pointTransformed - position ) - radius * radius;
	double t = b*b - 4*a*c;
	double epsilon = 1e-12;
	
	//~ if( t < -std::numeric_limits<double>::epsilon() )
	if( t < -epsilon )
		intersectionResult->setIntersection( false );
		
	else
	{	
		double lambda = -1;
		// lambda = ~ 0
		//~ if( abs(t) < std::numeric_limits<double>::epsilon() )
		if( abs(t) < epsilon )
		{
			// we just touche the sqhere -> we do not go through the object and therefore there is no transmittance
			intersectionResult->setTransmittance( false );
			lambda = (-b)/(2*a);
		}
		else
		{
			intersectionResult->setTransmittance( true );
			double sqrtT = std::sqrt(t);
			double lambda1 = (-b+sqrtT)/(2*a);
			double lambda2 = (-b-sqrtT)/(2*a);
			
			
			// if lambda < 0 than intersection is in the other direction, if it is 0 than intersection with the own wall
			if( lambda1 < lambda2 )
			{
				//~ if( lambda1 > std::numeric_limits<double>::epsilon()*lambda1 )
				//~ if( lambda1 > std::numeric_limits<double>::epsilon()*10000 )
				if( lambda1 > epsilon )
					lambda = lambda1;
				else
					lambda = lambda2;
			} else 
			{
				if( lambda2 < lambda1 )
				{
					//~ if( lambda2 > std::numeric_limits<double>::epsilon()*lambda2 )
					//~ if( lambda2 > std::numeric_limits<double>::epsilon()*10000 )
					if( lambda2 > epsilon )
						lambda = lambda2;
					else
						lambda = lambda1;
				}
			}
		}
		
		intersectionResult->setLambda( lambda );
		//~ std::cout << lambda << std::endl;
		//~ if( lambda > std::numeric_limits<double>::epsilon()*10000 )
		if( lambda > epsilon )
		{
		//~ if( lambda > 1.0e-17 )
			intersectionResult->setIntersection( true );
			
			// get intersection Point and normal
			dvec3 intersectionPoint = pointTransformed + rayTransformed * lambda;
			intersectionResult->setIntersectionPoint( 
					dvec3(getTransformationMatrix()* dvec4( intersectionPoint, 1) ) );
	
			// the normal is the intersection point - center of the sphere
			dvec3 intersectionNormal = normalize( intersectionPoint - position );
			intersectionResult->setNormal( 
					normalize( dvec3( getTransformationMatrix() * dvec4( intersectionNormal, 0 ) ) ) );
			
		}
		else
			intersectionResult->setIntersection( false );	
			
	}
	
	return intersectionResult;
}


// this function is probably now obsolete
void Sphere::getIntersectionInformation( dvec3 point, dvec3 ray, 
		IntersectionResult* intersectionResult )
{
	dmat4 inverseTransformations = affineInverse( getTransformationMatrix() );
	
	dvec3 pointTransformed = dvec3( inverseTransformations * dvec4( point, 1 ) );
	dvec3 rayTransformed = dvec3( inverseTransformations * dvec4( ray, 0 ) );	
	
	// need to transform back into world space
	// the intersection point is ray origin + lambda * ray direction
	dvec3 intersectionPoint = pointTransformed + ( rayTransformed*intersectionResult->getLambda() );
	intersectionResult->setIntersectionPoint( 
			dvec3(getTransformationMatrix()* dvec4( intersectionPoint, 1) ) );
	
	// the normal is the intersection point - center of the sphere
	dvec3 intersectionNormal = normalize( intersectionPoint - position );
	intersectionResult->setNormal( 
			normalize( dvec3( getTransformationMatrix() * dvec4( intersectionNormal, 0 ) ) ) );
}
