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
	double angle = dot( lightVector, normalVector );
			
	if( angle < 0 )
		angle = 0;
			
	return phongKD * angle * lightColor;
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
			if( shadowRayIntersection->isIntersection() )		/// this was commented out -> reason???
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

/// TODO: maybe good and simple idea to give also the refractionIndex of the material the ray is comming from as an argument 
dvec3 Raytracer::trace( dvec3* point, dvec3* ray, dvec3* eyeVector, int step, double refractionIndex )
{
	//~ std::cout << "step: " << step << ", maxBounces: " << maxBounces << std::endl;
	
	if( step > maxBounces )
		return backgroundColor;

	dvec3 color = backgroundColor;
	
	IntersectionResult* closestIntersection = new IntersectionResult();
	closestIntersection->setIntersection( false );
	closestIntersection->setLambda( std::numeric_limits<double>::max() );
	size_t closestObject;
	//~ double lambdaMin = std::numeric_limits<double>::max();
	
	for( size_t i = 0; i < surfaceArray.size(); i++ )
	{
		IntersectionResult* intersect = surfaceArray.at(i)->intersect( *point, *ray );
		
		/// TODO here is probably room for optimization
		if( intersect->isIntersection() )
		{
			//~ std::cout << "surface intersection" << std::endl;
			if( closestIntersection->getLambda() > intersect->getLambda() )
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
		//~ surfaceArray.at(closestObject)->getIntersectionInformation( 
				//~ *point, *ray, closestIntersection );
		
		color = shade( closestIntersection, surfaceArray.at(closestObject), eyeVector );
		
		// the color calculated in shade is only ( 1 - reflectance - transmittance ) part of final color
		color = color * ( 1 - surfaceArray.at(closestObject)->getMaterial()->getReflectance()
				- surfaceArray.at(closestObject)->getMaterial()->getTransmittance() );
				
		//~ delete point;
		
		/// TODO be aware if point changes in reflectedRay I may not use it anymore in refraction
		//~ *point = dvec3( closestIntersection->getIntersectionPoint() );
		//~ delete ray;
		
		// the ray points towards the surface, the normal away
		// therefore use -ray so that both point away from the surface
		
		delete closestIntersection;
		
		/// TODO if reflectance != 0 trace reflected Ray
		// reflection
		// no need for reflacted ray if reflectance is 0
		if( surfaceArray.at(closestObject)->getMaterial()->getReflectance() != 0 )
		{
			//~ std::cout << surfaceArray.at(closestObject)->getMaterial()->getReflectance() << std::endl;
			
			*point = dvec3( closestIntersection->getIntersectionPoint() );
			
			dvec3* reflectedRay = new dvec3(
					normalize( 2 * dot( closestIntersection->getNormal(), -(*ray) ) 
							* closestIntersection->getNormal() + (*ray) ) );
			//~ reflectedRay = normalize( 2 
					//~ * dot( closestIntersection->getNormal(), -(*ray) ) 
					//~ * closestIntersection->getNormal() + (*ray) );
			color = color + surfaceArray.at(closestObject)->getMaterial()->getReflectance() 
					* trace( point, reflectedRay, eyeVector, step + 1, refractionIndex );
			
			delete reflectedRay;
			
		}
		
		// refraction
		if( surfaceArray.at(closestObject)->getMaterial()->getTransmittance() != 0 )
		{
			*point = dvec3( closestIntersection->getIntersectionPoint() );
			
			bool totalInternalReflection = false;
			double n2n1 = surfaceArray.at(closestObject)->getMaterial()->getRefractionIndex() / refractionIndex;
			
			//~ double cosIncident = dot( -(*ray), closestIntersection->getNormal() );
			double cosIncident = dot( *ray, closestIntersection->getNormal() );		/// TODO: it seems this was the error but is it correct now?
			
			// if the fraction n2/n1 > 1 than arcsin is not defined and total internal reflection does not occur
			if( abs(n2n1) < 1 )
			{
				double criticalAngle = asin( n2n1 );
				double incidentAngle = acos( cosIncident );
				
				// if angle of incidence > critical angle than total internal reflection
				if( incidentAngle > criticalAngle )
					totalInternalReflection = true;
			}
			
			if( totalInternalReflection )
			{
				dvec3* internalReflectedRay = new dvec3(
						normalize( 2 * dot( -closestIntersection->getNormal(), -(*ray) ) 
								* closestIntersection->getNormal() + (*ray) ) );
									
				color = color + surfaceArray.at(closestObject)->getMaterial()->getReflectance()		/// TODO: with internal reflection do we use reflectance or transmittance index?
						* trace( point, internalReflectedRay, eyeVector, step + 1, refractionIndex );
				
				delete internalReflectedRay;
				
				std::cout << "total internal reflection" << step << std::endl;
				std::cout << "angle in " << acos( cosIncident ) << std::endl;
				std::cout << "angle crit " << asin( n2n1 ) << std::endl;
			}
			else 	// refraction
			{
				double n1n2 = refractionIndex / surfaceArray.at(closestObject)->getMaterial()->getRefractionIndex();
				// since we checked already for incidentAngle < criticalAngle this should be save
				double bla = 1 - n1n2 * n1n2 * (1 - cosIncident) * (1 - cosIncident);
				if( bla < 0 )
				{
					std::cout << "bla shouldn't happen " << bla << std::endl;
				}
				double cosTransmittance = sqrt( 1 - n1n2 * n1n2 * (1 - cosIncident) * (1 - cosIncident) );
				
				/// TODO: some error, many vectors with nan values -> error probably in bla -> still some errors
				dvec3* refractedRay = new dvec3( 
					n1n2 * (*ray) + ( n1n2 * cosIncident - cosTransmittance ) * closestIntersection->getNormal() );
				
				color = color + surfaceArray.at(closestObject)->getMaterial()->getTransmittance()
						* trace( point, refractedRay, eyeVector, step + 1, 
								surfaceArray.at(closestObject)->getMaterial()->getRefractionIndex() );
					
				delete refractedRay;
				
				std::cout << "refraction " << step << std::endl;
				std::cout << "inVec " << to_string(*ray) << std::endl;
				std::cout << "rfVec " << to_string(*refractedRay) << std::endl;
			}
			
			
					//~ refractionIndex / surfaceArray.at(closestObject)->getMaterial()->getRefractionIndex
					//~ * (*ray) );
			
			
			
			// if this condition is not fullfilled we have total internal reflection
			// condition is sin^2 phi < 1 -> 1 - cos^2 phi < 1
			//~ if( 1 - cosPhi * cosPhi < 1 )
			//~ {
				//~ 
			//~ }
			//~ else 	// total internal reflection
			//~ {
					//~ 
			//~ }
			
			
		}
		/// TODO if refractance != 0 trace refracted Ray
		
		//~ color = color + trace( point, ray, eyeVector, ++step );
		
		
				
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
			//~ color[2] = -color[2];
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
	
	double zoom = 1.0;		// used for zooming in or out of the scene
	dvec3 n = normalize( camera - center );
	dvec3 u = normalize( cross( up, n ) );
	dvec3 v = cross( u, n );
	double viewPlaneHalfHorizontal = tan( fov * M_PI / 180 ) * distance( camera, center ) * zoom;	// fov is already half
	double aspectRatio = ((double)vertical) / horizontal;
	double viewPlaneHalfVertical = aspectRatio*viewPlaneHalfHorizontal;
	
	dvec3 viewPlaneBottomLeft = center - v * viewPlaneHalfVertical - u * viewPlaneHalfHorizontal;
	
	double magdalena = 2*viewPlaneHalfHorizontal/horizontal;
	dvec3 xIncrement = u*magdalena;
	magdalena = 2*viewPlaneHalfVertical/vertical;
	dvec3 yIncrement = v*magdalena;

	// traverse through every pixel
	for( int i = 0; i < horizontal; i++  )
		for( int j = 0; j < vertical; j++ )
		{		
			dvec3 viewPlanePoint = viewPlaneBottomLeft + ((double)i)*xIncrement + ((double)j)*yIncrement;
			dvec3 initialRay = normalize(viewPlanePoint - camera);
					
			dvec3* point = new dvec3( camera );
			dvec3* ray = new dvec3( initialRay );
			dvec3* cameraVector = new dvec3( -initialRay );
			
			// loop through bounces
			// (recursive call really kills memory)
			/// after some additional tests and optimizations this seems no more the case (in fact even better than in loop?)
			//~ dvec3 color = dvec3( 0, 0, 0 );
			//~ for( int k = 0; k <= maxBounces; k++ )
			//~ {
				//~ color = color + trace( point, ray, cameraVector, k );
			//~ }
			
			// we asume the material the ray originates from is equal to 1 (air)
			dvec3 color = trace( point, ray, cameraVector, 0, 1 );
			
			delete point;
			delete ray;
			delete cameraVector;
			
			
			// need to swap picture, otherwise it will be upside down
			//~ image[ (vertical-1-j)*horizontal + i ] = color;
			image[ j*horizontal + i ] = color;
			
		}
}
