#include "Sphere.hpp"
#include "../../lib/glm/gtc/matrix_inverse.hpp"
//~ #include "../../lib/glm/gtx/string_cast.hpp"
#include <iostream>

using namespace glm;

bool Sphere::intersect( vec3 point, vec3 ray )
{
	// transform point and ray into object coordinates with inverse transformation matrix
	// all transformations are affine, so affineInvserse should work fine.
	/// TODO, for optimization i may calculate this only once (when setting transformation maybe?)
	//~ std::cout << to_string( getTransformationMatrix() ) << std::endl;
	mat4 inverseTransformations = affineInverse( getTransformationMatrix() );
	//~ std::cout << to_string( inverseTransformations ) << std::endl;
	
	vec3 pointTransformed = vec3( inverseTransformations * vec4( point, 1 ) );
	vec3 rayTransformed = vec3( inverseTransformations * vec4( ray, 0 ) );
	
	//intersection test
	
	double a = dot( rayTransformed, rayTransformed );
	double b = 2 * dot( pointTransformed, rayTransformed ) - 2 * dot( position, rayTransformed );
	double c = dot( pointTransformed - position, pointTransformed - position ) - radius * radius;
	
	
	
	
	double t = b*b - 4*a*c;
	
	if( t < 0 )
		return false;
	else
		return true;
		
	/// TODO == 0 ?
	
	

	
	
	
}
