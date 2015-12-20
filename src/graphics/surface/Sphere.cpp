#include "Sphere.hpp"
#include "../../lib/glm/gtc/matrix_inverse.hpp"
//~ #include "../../lib/glm/gtx/string_cast.hpp"
#include <iostream>
#include <limits>

using namespace glm;

IntersectionResult Sphere::intersect( dvec3 point, dvec3 ray )
{
	IntersectionResult intersectionResult;
	//~ double alpha = std::numeric_limits<double>::max();
	
	// transform point and ray into object coordinates with inverse transformation matrix
	// all transformations are affine, so affineInvserse should work fine.
	/// TODO, for optimization i may calculate this only once (when setting transformation maybe?)
	//~ std::cout << to_string( getTransformationMatrix() ) << std::endl;
	dmat4 inverseTransformations = affineInverse( getTransformationMatrix() );
	//~ std::cout << to_string( inverseTransformations ) << std::endl;
	
	dvec3 pointTransformed = dvec3( inverseTransformations * dvec4( point, 1 ) );
	dvec3 rayTransformed = dvec3( inverseTransformations * dvec4( ray, 0 ) );
	
	//intersection test
	double a = dot( rayTransformed, rayTransformed );
	double b = 2 * dot( pointTransformed, rayTransformed ) - 2 * dot( position, rayTransformed );
	double c = dot( pointTransformed - position, pointTransformed - position ) - radius * radius;
	
	double t = b*b - 4*a*c;
	
	if( t < -std::numeric_limits<double>::epsilon() )
		intersectionResult.setIntersection( false );
		
	else
	{
		intersectionResult.setIntersection( true );
		
		/// TODO: that works fine for solid material but with textured i probably need to point
		//~ intersectionResult.setSurfaceColor( getMaterial()->getColor() );
		
		double alpha;
		// alpha = ~ 0
		if( t < std::numeric_limits<double>::epsilon() && t > -std::numeric_limits<double>::epsilon() )
		{
			// we just touche the sqhere -> we do not go through the object and therefore there is no transmittance
			intersectionResult.setTransmittance( false );
			alpha = (-b)/(2*a);
		}
		else
		{
			intersectionResult.setTransmittance( true );
			double sqrtT = std::sqrt(t);
			double alpha1 = (-b+sqrtT)/(2*a);
			double alpha2 = (-b-sqrtT)/(2*a);
			if( alpha1 < alpha2 )
				alpha = alpha1;
			else
				alpha = alpha2;
		}
		
		intersectionResult.setAlpha( alpha );
		
		
		
		
		//~ /// TODO: get point, normal, reflection, refraction
		//~ // need to transform back into world space
	//~ 
		//~ // the intersection point is ray origin + alpha * ray direction
		//~ dvec3 point = point + (ray*alpha);
		//~ intersectionResult.setIntersectionPoint( 
				//~ dvec3(getTransformationMatrix()* dvec4( point, 1) ) );
	//~ 
		//~ // the normal is the intersection point - center of the sphere
		//~ dvec3 normal = normalize( point - position );
		//~ normal = normalize( normal );
		//~ intersectionResult.setNormal( 
				//~ normalize( dvec3( getTransformationMatrix() * dvec4( normal, 0 ) ) ) );
				//~ 
		//~ // angle of incident = angle of reflection therefore 
		//~ // it is ray direction - 2( normal . ray direction ) * normal
		//~ dvec3 reflection = normalize( ray - 2* ( dot( normal, ray ) ) * normal );
		//~ intersectionResult.setReflectionVector( 
				//~ dvec3( getTransformationMatrix() * dvec4( reflection, 0 ) ) );
		
	}
	
	return intersectionResult;
}


void Sphere::getIntersectionInformation( dvec3 point, dvec3 ray, 
		IntersectionResult* intersectionResult )
{
	//~ IntersectionResult intersectionResult;
	//~ intersection.setIntersection( true );
	//~ intersection.setAlpha( alpha );
	
	dmat4 inverseTransformations = affineInverse( getTransformationMatrix() );
	
	dvec3 pointTransformed = dvec3( inverseTransformations * dvec4( point, 1 ) );
	dvec3 rayTransformed = dvec3( inverseTransformations * dvec4( ray, 0 ) );
	
	
	// need to transform back into world space
	// the intersection point is ray origin + alpha * ray direction
	dvec3 intersectionPoint = pointTransformed + (rayTransformed*intersectionResult->getAlpha());
	intersectionResult->setIntersectionPoint( 
			dvec3(getTransformationMatrix()* dvec4( intersectionPoint, 1) ) );
	
	// the normal is the intersection point - center of the sphere
	dvec3 intersectionNormal = normalize( intersectionPoint - position );
	intersectionResult->setNormal( 
			normalize( dvec3( getTransformationMatrix() * dvec4( intersectionNormal, 0 ) ) ) );
			
	// angle of incident = angle of reflection therefore 
	// it is ray direction - 2( normal . ray direction ) * normal
	dvec3 intersectionReflection = normalize( rayTransformed - 
			2 * ( dot( intersectionNormal, rayTransformed ) ) * intersectionNormal );
	intersectionResult->setReflectionVector( 
			dvec3( getTransformationMatrix() * dvec4( intersectionReflection, 0 ) ) );
}
