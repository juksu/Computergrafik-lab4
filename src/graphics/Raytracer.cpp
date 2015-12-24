#include "Raytracer.hpp"

#include "../lib/glm/gtc/matrix_transform.hpp"
//~ #include "../lib/glm/gtc/matrix_inverse.hpp"
#include "../lib/glm/gtx/string_cast.hpp"

#include <iostream>
#include <limits>
#include <cmath>

using namespace glm;

/// maybe a own function specifically for Phong


dvec3 Raytracer::shade( IntersectionResult* intersectionResult, Surface* surface, dvec3* eyeVector )
{
	IntersectionResult* shadowRayIntersection = new IntersectionResult();
	
	// color of the material
	dvec3 color = surface->getMaterial()->getColor();
	
	dvec3 intensity = dvec3(0, 0, 0);
	
	//~ dvec3 color = backgroundColor;
	
	// Phong ilumination model
	// Ambient Light
	std::vector<AmbientLight*> ambientLights = lightContainer.getAmbientLights();
	for( size_t i = 0; i < ambientLights.size(); i++ )
	{
		intensity = intensity + surface->getMaterial()->getPhongKA() * ambientLights.at(i)->getColor();
	}

	// parallel light
	std::vector<ParallelLight*> parallelLights = lightContainer.getParallelLights();
	shadowRayIntersection->setIntersection( false );
	for( size_t i = 0; i < parallelLights.size(); i++ )
	{
		for( size_t j = 0; j < surfaceArray.size(); j++ )
		{
			
			delete shadowRayIntersection;
			shadowRayIntersection = surfaceArray.at(j)->intersect(
					intersectionResult->getIntersectionPoint(), 
					normalize( -(parallelLights.at(i)->getDirection()) ) );
			
			if( shadowRayIntersection->isIntersection() )
				break;
		}
		if( !shadowRayIntersection->isIntersection() )
		{
			// diffuse reflection
			double angle = dot( normalize( -parallelLights.at(i)->getDirection() ), 
					intersectionResult->getNormal() );
			
			if( angle < 0 )
				angle = 0;
			
			intensity = intensity + surface->getMaterial()->getPhongKD() * angle 
					* parallelLights.at(i)->getColor();
			
			// specular reflection
			// calculate reflection ray from light source
			dvec3 lightReflection = normalize( 2 * angle * intersectionResult->getNormal() 
					- normalize( -parallelLights.at(i)->getDirection() ) );
			
			angle = dot( lightReflection, *eyeVector );
			if( angle < 0 )
				angle = 0;
			
			intensity = intensity + surface->getMaterial()->getPhongKS()
					* parallelLights.at(i)->getColor() 
					* pow( angle, surface->getMaterial()->getPhongExponent() );
		}
	}
	delete shadowRayIntersection;
	
	color[0] = color[0] * intensity[0];
	color[1] = color[1] * intensity[1];
	color[2] = color[2] * intensity[2];
	
	return color;
}


dvec3 Raytracer::trace( dvec3* point, dvec3* ray, dvec3* eyeVector, int step )
{
	//~ std::cout << "step: " << step << ", maxBounces: " << maxBounces << std::endl;
	
	if( step > maxBounces )
		return backgroundColor;

	dvec3 color = backgroundColor;
	
	IntersectionResult* closestIntersection = new IntersectionResult();
	closestIntersection->setIntersection( false );
	closestIntersection->setAlpha( std::numeric_limits<double>::max() );
	size_t closestObject;
	//~ double alphaMin = std::numeric_limits<double>::max();
	
	for( size_t i = 0; i < surfaceArray.size(); i++ )
	{
		IntersectionResult* intersect = surfaceArray.at(i)->intersect( *point, *ray );
		
		/// TODO here is probably room for optimization
		if( intersect->isIntersection() )
		{
			//~ std::cout << "surface intersection" << std::endl;
			if( closestIntersection->getAlpha() > intersect->getAlpha() )
			{
				delete closestIntersection;
				closestIntersection = intersect;
				closestObject = i;
			}
			else
				delete intersect;
		}
	}
	
	if( closestIntersection->isIntersection() )
	{	
		surfaceArray.at(closestObject)->getIntersectionInformation( 
				*point, *ray, closestIntersection );
		
		color = shade( closestIntersection, surfaceArray.at(closestObject), eyeVector );
		
		//~ delete point;
		*point = dvec3( closestIntersection->getIntersectionPoint() );
		//~ delete ray;
		
		// the ray points towards the surface, the normal away
		// therefore use -ray so that both point away from the surface
		dvec3 reflectedRay = normalize( 2 
				* dot( closestIntersection->getNormal(), -(*ray) ) 
				* closestIntersection->getNormal() + (*ray) );
		*ray = dvec3( reflectedRay );
		
		delete closestIntersection;
				
		/// for debugging
		//~ color = surfaceArray.at(closestObject)->getMaterial()->getColor();
		//~ color = closestIntersection->getIntersectionPoint();
		//~ color = closestIntersection->getNormal();
		//~ color = reflectedRay;
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
			
			dvec3 initialRay = normalize(dvec3(x,y,z));
			
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
					
			dvec3* point = new dvec3( camera );
			dvec3* ray = new dvec3( initialRay );
			dvec3* cameraVector = new dvec3( -initialRay );
			
			//~ dvec3 color = trace( camera, ray, -ray, 0 );
			//~ dvec3 color = trace( point, ray, cameraVector, 0 );
			
			// loop through bounces
			// (recursive call really kills memory)
			dvec3 color = dvec3( 0, 0, 0 );
			for( int i = 0; i <= maxBounces; i++ )
			{
				color = color + trace( point, ray, cameraVector, i );
			}
			
			delete point;
			delete ray;
			delete cameraVector;
			
			
			// need to swap picture, otherwise it will be upside down
			image[ (vertical-1-v)*horizontal + u ] = color;
			
		}
}
