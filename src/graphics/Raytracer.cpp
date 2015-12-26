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
	std::cout << "vertical = " << vertical << " horizontal = " << horizontal << std::endl;
	//~ std::cout << (double)vertical/ horizontal << std::endl;
	std::cout << "tanx = " << tanX << ", tanY = " << tanY  << std::endl;
	
	
	//~ // other technique
	dvec3 n = normalize( camera - center );
	dvec3 u = normalize( cross( up, n ) );
	dvec3 v = cross( u, n );
	double viewPlaneHalfHorizontal = tan( fov * M_PI / 180 );	// fov is already half
	double aspectRatio = ((double)vertical) / horizontal;
	double viewPlaneHalfVertical = aspectRatio*viewPlaneHalfHorizontal;
	
	dvec3 viewPlaneBottomLeft = center - v * viewPlaneHalfVertical - u * viewPlaneHalfHorizontal;
	
	double bla = 2*viewPlaneHalfHorizontal/horizontal;
	dvec3 xIncrement = u*bla;
	bla = 2*viewPlaneHalfVertical/vertical;
	dvec3 yIncrement = v*bla;

	// traverse through every pixel
	for( int i = 0; i < horizontal; i++  )
		for( int j = 0; j < vertical; j++ )
		{		
			/// Old approach			
			// calculate the view plane point, assuming that the view plane is at z = -1;
			double x = (double)(2*i - (horizontal-1))/(horizontal-1)*tanX;
			double y = (double)(2*j - (vertical-1))/(vertical-1)*tanY;
			double z = -1;
			
			dvec3 initialRay = normalize(dvec3(x,y,z));
			//~ std::cout << "ray untransformed " << to_string( initialRay ) << std::endl;

			/// New approach: this draws image as if the fewplane would be there were the center is -> with examples a zoomed in view -> correct?
			/// same result only if center is a unit away from the camera
			dvec3 viewPlanePoint = viewPlaneBottomLeft + ((double)i)*xIncrement + ((double)j)*yIncrement;
			initialRay = normalize(viewPlanePoint - camera);
					
			dvec3* point = new dvec3( camera );
			dvec3* ray = new dvec3( initialRay );
			dvec3* cameraVector = new dvec3( -initialRay );
			
			// loop through bounces
			// (recursive call really kills memory)
			dvec3 color = dvec3( 0, 0, 0 );
			for( int k = 0; k <= maxBounces; k++ )
			{
				color = color + trace( point, ray, cameraVector, k );
			}
			
			delete point;
			delete ray;
			delete cameraVector;
			
			
			// need to swap picture, otherwise it will be upside down
			//~ image[ (vertical-1-j)*horizontal + i ] = color;
			image[ j*horizontal + i ] = color;
			
		}
}
