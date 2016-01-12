#include "Raytracer.hpp"

#include "../lib/glm/gtc/matrix_transform.hpp"
//~ #include "../lib/glm/gtc/matrix_inverse.hpp"
#include "../lib/glm/gtx/string_cast.hpp"

#include <iostream>
#include <limits>
#include <cmath>

using namespace glm;

/// maybe a own function specifically for Phong


dvec3 Raytracer::ambientLight( double phongKA, dvec3 lightColor )
{
	return phongKA * lightColor;
}

dvec3 Raytracer::diffuseLight( double phongKD, dvec3 lightColor, 
		dvec3 lightVector, dvec3 normalVector )
{
	//~ double angle = dot( normalize( -parallelLights.at(i)->getDirection() ), 
					//~ intersectionResult->getNormal() );
	double angle = dot( lightVector, normalVector );
			
	if( angle < 0 )
		angle = 0;
			
	return phongKD * angle * lightColor;
	//~ intensity = intensity + surface->getMaterial()->getPhongKD() * angle
			//~ * parallelLights.at(i)->getColor();	
}

dvec3 Raytracer::specularLight( double phongKS, double phongExponent, dvec3 lightColor, 
		dvec3 lightVector, dvec3 normalVector, dvec3 eyeVector )
{
	// calculate reflection ray from light source
	dvec3 lightReflection = normalize( 2 * dot( lightVector, normalVector ) 
			* normalVector - lightVector );
			
	double angle = dot( lightReflection, eyeVector );
	
	if( angle < 0 )
		angle = 0;
		
	return phongKS * lightColor * pow( angle, phongExponent );
	
	
				//~ dvec3 lightReflection = normalize( 2 * angle * intersectionResult->getNormal() 
					//~ - normalize( -parallelLights.at(i)->getDirection() ) );
			//~ 
			//~ angle = dot( lightReflection, *eyeVector );
			//~ if( angle < 0 )
				//~ angle = 0;
			//~ 
			//~ intensity = intensity + surface->getMaterial()->getPhongKS()
					//~ * parallelLights.at(i)->getColor() 
					//~ * pow( angle, surface->getMaterial()->getPhongExponent() );
}

dvec3 Raytracer::shade( IntersectionResult* intersectionResult, Surface* surface, dvec3* eyeVector )
{
	// color of the material
	dvec3 color = surface->getMaterial()->getColor();
	
	dvec3 intensity = dvec3(0, 0, 0);
	
	// Phong ilumination model
	// Ambient Light
	std::vector<AmbientLight*> ambientLights = lightContainer.getAmbientLights();
	for( size_t i = 0; i < ambientLights.size(); i++ )
	{
		intensity = intensity + ambientLight(
				surface->getMaterial()->getPhongKA(), ambientLights.at(i)->getColor() );
	}

	// parallel light
	std::vector<ParallelLight*> parallelLights = lightContainer.getParallelLights();
	IntersectionResult* shadowRayIntersection = new IntersectionResult();
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
			intensity = intensity 
					+ diffuseLight( surface->getMaterial()->getPhongKD(), parallelLights.at(i)->getColor(),
							normalize( -parallelLights.at(i)->getDirection() ), intersectionResult->getNormal() );

			// specular reflection
			intensity = intensity
					+ specularLight( surface->getMaterial()->getPhongKS(), surface->getMaterial()->getPhongExponent(),
							parallelLights.at(i)->getColor(), normalize( -parallelLights.at(i)->getDirection() ),
							intersectionResult->getNormal(), *eyeVector );
		}
	}
	
	// point light
	std::vector<PointLight*> pointLights = lightContainer.getPointLights();
	shadowRayIntersection->setIntersection( false );
	for( size_t i = 0; i < pointLights.size(); i++ )
	{
		for( size_t j = 0; j < surfaceArray.size(); j++ )
		{
			
			delete shadowRayIntersection;
			shadowRayIntersection = surfaceArray.at(j)->intersect(
					intersectionResult->getIntersectionPoint(), 
					normalize( pointLights.at(i)->getPosition() 
							- intersectionResult->getIntersectionPoint() ) );
			
			// it may be that we have an intersection but the intersection is after the point light source
			// therefore, if only break if length to intersection point < length to light source
			// else, we tread it as no intersection
			if( shadowRayIntersection->isIntersection() )
			{
				if( length( shadowRayIntersection->getIntersectionPoint() 
								- intersectionResult->getIntersectionPoint() ) 
						< length( pointLights.at(i)->getPosition() 
								- intersectionResult->getIntersectionPoint() ) )
					break;
				else
					shadowRayIntersection->setIntersection( false );
			}
		}
		if( !shadowRayIntersection->isIntersection() )
		{
			// diffuse reflection
			intensity = intensity 
					+ diffuseLight( surface->getMaterial()->getPhongKD(), pointLights.at(i)->getColor(),
							normalize( pointLights.at(i)->getPosition() - intersectionResult->getIntersectionPoint() ),
							intersectionResult->getNormal() );

			// specular reflection
			intensity = intensity + 
					specularLight( surface->getMaterial()->getPhongKS(), surface->getMaterial()->getPhongExponent(),
							pointLights.at(i)->getColor(), 
							normalize( pointLights.at(i)->getPosition() - intersectionResult->getIntersectionPoint() ),
							intersectionResult->getNormal(), *eyeVector );
		}
	}
	
	// spot light
	std::vector<SpotLight*> spotLights = lightContainer.getSpotLights();
	shadowRayIntersection->setIntersection( false );
	for( size_t i = 0; i < spotLights.size(); i++ )
	{
		for( size_t j = 0; j < surfaceArray.size(); j++ )
		{
			
			delete shadowRayIntersection;
			shadowRayIntersection = surfaceArray.at(j)->intersect(
					intersectionResult->getIntersectionPoint(), 
					normalize( spotLights.at(i)->getPosition() 
							- intersectionResult->getIntersectionPoint() ) );
			
			// it may be that we have an intersection but the intersection is after the spot light source
			// therefore, if only break if length to intersection point < length to light source
			// else, we tread it as no intersection
			//~ if( shadowRayIntersection->isIntersection() )
			{
				if( length( shadowRayIntersection->getIntersectionPoint() 
								- intersectionResult->getIntersectionPoint() ) 
						< length( spotLights.at(i)->getPosition() 
								- intersectionResult->getIntersectionPoint() ) )
					break;
				else
					shadowRayIntersection->setIntersection( false );
			}
		}
		if( !shadowRayIntersection->isIntersection() )
		{			
			// what is the angle between the spotLight direction and the vector from spotLightPosition to IntersectionPoint?
			double angle = dot( normalize( spotLights.at(i)->getDirection() ),
					normalize( intersectionResult->getIntersectionPoint()
					- spotLights.at(i)->getPosition() ) );

			
			angle = acos(angle);
					
			dvec3 spotIntensity = dvec3( 0, 0, 0 );
			
			if( angle > spotLights.at(i)->getFalloffAlpha2() ) ;
				//~ spotIntensity = dvev3( 0, 0, 0 );
			else
			{
				spotIntensity = spotIntensity
						+ diffuseLight( surface->getMaterial()->getPhongKD(), spotLights.at(i)->getColor(),
								normalize( spotLights.at(i)->getPosition() - intersectionResult->getIntersectionPoint() ),
								intersectionResult->getNormal() );
				
				spotIntensity = spotIntensity + 
						specularLight( surface->getMaterial()->getPhongKS(), surface->getMaterial()->getPhongExponent(),
								spotLights.at(i)->getColor(), 
								normalize( spotLights.at(i)->getPosition() - intersectionResult->getIntersectionPoint() ),
								intersectionResult->getNormal(), *eyeVector );
								
				if( angle > spotLights.at(i)->getFalloffAlpha1() )
				{
					// what intensity between alpha1 (100% intensity) and alpha2 (0% intensity) do we have
					double falloff = ( angle - spotLights.at(i)->getFalloffAlpha1() ) 
							/ ( spotLights.at(i)->getFalloffAlpha2() - spotLights.at(i)->getFalloffAlpha1() );
					
					// linear falloff
					falloff = 1 - falloff;				
					// quadratic falloff (prettier result but spot looks smaller as it actually is)
					falloff = falloff * falloff;
											
					spotIntensity = spotIntensity * falloff;
				}
			}
			
			intensity = intensity + spotIntensity;
		}
	}
	delete shadowRayIntersection;
	
	color = color * intensity;
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
	//~ dvec3 viewPlaneBottomLeft = -n - v * viewPlaneHalfVertical - u * viewPlaneHalfHorizontal;
	
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
