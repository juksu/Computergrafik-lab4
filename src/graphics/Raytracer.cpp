#include "Raytracer.hpp"

#include "../lib/glm/gtc/matrix_transform.hpp"
//~ #include "../lib/glm/gtc/matrix_inverse.hpp"
#include "../lib/glm/gtx/string_cast.hpp"

#include <iostream>
#include <limits>
//~ #include <cmath>

using namespace glm;

dvec3 Raytracer::shade( IntersectionResult intersectionResult, Surface* surface )
{
	IntersectionResult shadowRayIntersection;
	
	
	//~ dvec3 color = surface->getMaterial()->getColor();
	dvec3 color = backgroundColor;
	
	//~ std::cout << "shade" << std::endl;
	
	std::vector<ParallelLight*> parallelLights = lightContainer.getParallelLights();
	
	
	/// TODO some error
	shadowRayIntersection.setIntersection( false );
	for( size_t i = 0; i < parallelLights.size(); i++ )
	{
		for( size_t j = 0; j < surfaceArray.size(); j++ )
		//~ for( size_t j = surfaceArray.size(); j > 0; j-- )
		{
			//~ std::cout << "testing shadowray intersection" << std::endl;
			shadowRayIntersection = surfaceArray.at(j)->intersect( 
					intersectionResult.getIntersectionPoint(), 
					normalize( -(parallelLights.at(i)->getDirection()) ) );
					//~ normalize( (parallelLights.at(i)->getDirection()) ) );
					//~ normalize(dvec3(1,0,0)) );
			//~ 
			if( shadowRayIntersection.isIntersection() )
			{
				//~ std::cout << "intersection" << std::endl;
				break;
			}
		}
		if( !shadowRayIntersection.isIntersection() )
		{
			//~ 
			//~ std::cout << "no intersection" << std::endl;
			//~ /// TODO something very wrong:
			//~ /// example1.xml -> it thinks left is completly in shadow, the other completely in light
			color = surface->getMaterial()->getColor();
			//~ color = dvec3(0.7,0.7,0.7);
		}
	}
	
	//~ parallelLights.at(0)->getDirection();
	
	return color;
}


dvec3 Raytracer::trace( dvec3 point, dvec3 ray, int step )
{
	if( step > maxBounces )
		return backgroundColor;

	dvec3 color = backgroundColor;
	
	IntersectionResult closestIntersection;
	closestIntersection.setIntersection( false );
	closestIntersection.setAlpha( std::numeric_limits<double>::max() );
	size_t closestObject;
	//~ double alphaMin = std::numeric_limits<double>::max();
	
	for( size_t i = 0; i < surfaceArray.size(); i++ )
	{
		IntersectionResult intersect = surfaceArray.at(i)->intersect( point, ray );
		
		/// TODO here is probably room for optimization
		if( intersect.isIntersection() )
		{
			//~ std::cout << "surface intersection" << std::endl;
			if( closestIntersection.getAlpha() > intersect.getAlpha() )
			{
				closestIntersection = intersect;
				closestObject = i;
			}
		}
	}
	
	if( closestIntersection.isIntersection() )
	{	
		surfaceArray.at(closestObject)->getIntersectionInformation( 
				point, ray, &closestIntersection );
		
		//~ if( closestObject == 0 )
			//~ std::cout << to_string(closestIntersection.getIntersectionPoint() ) << std::endl;
		
		color = shade( closestIntersection, surfaceArray.at(closestObject) );
		
		
		/// for debugging
		//~ color = surfaceArray.at(closestObject)->getMaterial()->getColor();
		//~ color = closestIntersection.getIntersectionPoint();
		//~ color = closestIntersection.getNormal();
		//~ color = normalize(color);
		//~ if( color[0] < 0 )
			//~ color[0] = 0;
		//~ if( color[1] < 0 )
			//~ color[1] = 0;
		//~ if( color[2] < 0 )
			//~ color[2] = 0;
		
		//~ color[2] = color[2]/3;
		
		//~ color = abs(color);
		
		
	}
	
	return color;
}

void Raytracer::render()
{
	/// TODO all the graphics processing
	image = new dvec3[horizontal*vertical];		
	
	// calculate the tan values for x and y using radian
	double tanX = tan( fov * M_PI / 180 );
	double tanY = tan( ((double)vertical / horizontal) * (fov * M_PI / 180) );
	//~ std::cout << "vertical = " << vertical << " horizontal = " << horizontal << std::endl;
	//~ std::cout << (double)vertical/ horizontal << std::endl;
	//~ std::cout << "tanx = " << tanX << ", tanY = " << tanY  << std::endl;
	
	// traverse through every pixel
	for( int u = 0; u < horizontal; u++  )
		for( int v = 0; v < vertical; v++ )
		{		
			// calculate the view plane point, assuming that the view plane is at z = -1;
			/// TODO: the question is really does fov give the angle to the edge of the outermost pixel or to the middle of it
			/// if edge: (horizontal-1)/horizontal, else (horizontal-1)/(horizontal-1)
			/// the same for vertical
			double x = (double)(2*u - (horizontal-1))/(horizontal-1)*tanX;
			double y = (double)(2*v - (vertical-1))/(vertical-1)*tanY;
			//~ double z = camera[2] - 1;
			double z = -1;
			
			dvec3 ray = normalize(dvec3(x,y,z));
			
			/// TODO: do something about lookat, up and so on

			// this assumes the camera is in the center with y-axis being up and viewplane at z = 1;
			// to get the ray calculate viewplane - cameraposition
			
			//~ vec3 ray = vec3(x,y,z) - camera;
			//~ std::cout << "xyz " << x << ", " << y << ", " << z << ", " << std::endl;
			
			//~ std::cout << "ray " << ray[0] << ", " << ray[1] << ", " << ray[2] << std::endl;
			// normalize
			//~ ray = normalize(ray);
			//~ std::cout << "ray " << ray[0] << ", " << ray[1] << ", " << ray[2] << std::endl;
			
			/// TODO, no that does not work, inverse or not
			// construct lookAtTransformation takin care about up and center and stuff
			//~ mat4 lookAtTransformation = lookAt( camera, center, up );
			//~ vec4 magdalena = affineInverse(lookAtTransformation) * vec4(ray[0],ray[1],ray[2],0);		// i told you
			//~ std::cout << "magdalena " << magdalena[0] << ", " << magdalena[1] << ", " 
					//~ << magdalena[2] << std::endl;	
					
			dvec3 color = trace( camera, ray, 0 );
			
			// need to swap picture, otherwise it will be upside down
			image[ (vertical-1-v)*horizontal + u ] = color;
			
		}
}
