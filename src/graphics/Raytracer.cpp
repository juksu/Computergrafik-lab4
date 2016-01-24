#include "Raytracer.hpp"
#include <iostream>
#include <limits>
#include <cmath>

using namespace glm;

dvec3 Raytracer::ambientLight( double phongKA, dvec3 lightColor ) const
{
	return phongKA * lightColor;
}

dvec3 Raytracer::diffuseLight( double phongKD, dvec3 lightColor, 
		dvec3 lightVector, dvec3 normalVector ) const
{
	double lightNormDot = dot( lightVector, normalVector );
	
	// having negative values screws with the color information		
	return phongKD * max(lightNormDot, 0.0) * lightColor;
}

dvec3 Raytracer::specularLight( double phongKS, double phongExponent, dvec3 lightColor, 
		dvec3 lightVector, dvec3 normalVector, dvec3 eyeVector ) const
{
	// calculate reflection ray from light source
	dvec3 lightReflection = normalize( 2 * dot( lightVector, normalVector ) 
			* normalVector - lightVector );

	double lightEyeDot = dot( lightReflection, eyeVector );
	
	return phongKS * lightColor * pow( max( lightEyeDot, 0.0 ), phongExponent );
}

dvec3 Raytracer::shade( const IntersectionResult* const intersectionResult, 
		const Surface* const surface, const dvec3* const eyeVector ) const
{
	// color of the material
	dvec3 color = intersectionResult->getSurfaceColor();
	//~ std::cout << to_string(intersectionResult->getSurfaceColor()) << std::endl;
	
	//~ dvec3 color = dvec3(0,0,0);
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
			if( shadowRayIntersection->isIntersection() )
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
			// get the dot product between the spotLight direction and the vector from spotLightPosition to IntersectionPoint
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
	//~ return intensity;
}

/// TODO: maybe good and simple idea to give also the refractionIndex of the material the ray is comming from as an argument 
dvec3 Raytracer::trace( dvec3* point, dvec3* ray, const dvec3* const eyeVector, int step, double refractionIndex, bool internalRay ) const
{
	//~ std::cout << "step: " << step << ", maxBounces: " << maxBounces << std::endl;
	
	if( step > maxBounces )
		return backgroundColor;

	dvec3 color = backgroundColor;
	
	IntersectionResult* closestIntersection = new IntersectionResult();
	closestIntersection->setIntersection( false );
	closestIntersection->setLambda( std::numeric_limits<double>::max() );
	size_t closestObject;
	
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
		
		// apply shading model only once when we hit or enter the object
		if( !internalRay )
		{
			color = closestIntersection->getSurfaceColor();
			color = shade( closestIntersection, surfaceArray.at(closestObject), eyeVector );
			color = color * ( 1 - surfaceArray.at(closestObject)->getMaterial()->getReflectance()
				- surfaceArray.at(closestObject)->getMaterial()->getTransmittance() );
				
			//~ color = color * shade( closestIntersection, surfaceArray.at(closestObject), eyeVector );
			//~ color = color * shade( closestIntersection, surfaceArray.at(closestObject), eyeVector );
		}
		
		// reflection
		// no need for reflacted ray if reflectance is 0
		if( surfaceArray.at(closestObject)->getMaterial()->getReflectance() != 0 )
		{			
			*point = dvec3( closestIntersection->getIntersectionPoint() );
			
			dvec3* reflectedRay = new dvec3(
					normalize( 2 * dot( closestIntersection->getNormal(), -(*ray) ) 
							* closestIntersection->getNormal() + (*ray) ) );
							
			color = color + surfaceArray.at(closestObject)->getMaterial()->getReflectance() 
					* trace( point, reflectedRay, eyeVector, step + 1, refractionIndex, false );
			
			delete reflectedRay;
			
		}
		
		// refraction
		if( surfaceArray.at(closestObject)->getMaterial()->getTransmittance() != 0 )
		{
			*point = dvec3( closestIntersection->getIntersectionPoint() );
			
			double n1n2;
			if( !internalRay )
				n1n2 = refractionIndex / surfaceArray.at(closestObject)->getMaterial()->getRefractionIndex();
			else
				n1n2 = refractionIndex / 1;
				
			double cosIncidence = dot( -(*ray), closestIntersection->getNormal() );	// normal and ray are normalized
			//~ double cosIncidence = dot( *ray, closestIntersection->getNormal() );	// normal and ray are normalized
			
			double bla = 1 - (n1n2 * n1n2 * (1 - cosIncidence * cosIncidence));
			if( bla >= 0 )
			{
				double cosRefraction = sqrt(bla);
				
				dvec3* refractedRay = new dvec3( ( n1n2 * (*ray) )
								+ ( ( n1n2 * cosIncidence - cosRefraction ) * closestIntersection->getNormal() ) );
				
				// refraction is transmittion from one medium to another
				// we leave the object therefore use standard refraction index 1 (air)
				if( internalRay )
					color = color + surfaceArray.at(closestObject)->getMaterial()->getTransmittance()
							* trace( point, refractedRay, eyeVector, step + 1, 1, false );	
				else
					color = color + surfaceArray.at(closestObject)->getMaterial()->getTransmittance()
							* trace( point, refractedRay, eyeVector, step + 1, 
									surfaceArray.at(closestObject)->getMaterial()->getRefractionIndex(), true );
				
				delete refractedRay;
				
			}
			else 	// total reflection
			{				
				dvec3* reflectedRay = new dvec3(
						normalize( 2 * dot( closestIntersection->getNormal(), -(*ray) ) 
								* closestIntersection->getNormal() + (*ray) ) );
									
				color = color + surfaceArray.at(closestObject)->getMaterial()->getTransmittance()
						* trace( point, reflectedRay, eyeVector, step + 1, refractionIndex, internalRay );
				
				delete reflectedRay;		
			}
		}
		
		//~ if( !internalRay )
			//~ color = color * shade( closestIntersection, surfaceArray.at(closestObject), eyeVector );
		
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
	std::cout << "rendering" << std::endl;
	
	image = new dvec3[horizontal*vertical];
	
	//~ supersampling = true;
	//~ samplerate = 4;	
	
	double zoom = 1.0;		// used for zooming in or out of the scene
	dvec3 n = normalize( camera - center );
	dvec3 u = normalize( cross( up, n ) );
	dvec3 v = cross( u, n );
	double viewPlaneHalfHorizontal = tan( fov * M_PI / 180 ) * distance( camera, center ) * zoom;	// fov is already half
	double aspectRatio = ((double)vertical) / horizontal;
	double viewPlaneHalfVertical = aspectRatio*viewPlaneHalfHorizontal;
	
	dvec3 viewPlaneBottomLeft = center - v * viewPlaneHalfVertical - u * viewPlaneHalfHorizontal;
	
	double magdalena = 2*viewPlaneHalfHorizontal/( horizontal * samplerate );
	dvec3 xIncrement = u*magdalena;
	magdalena = 2*viewPlaneHalfVertical/( vertical * samplerate );
	dvec3 yIncrement = v*magdalena;

	// traverse through every pixel
	
	//~ int pixelNum = horizontal * vertical;
	//~ std::cout << "rendering 0%" << std::endl;
	//~ int progress = 0;
	for( int i = 0; i < horizontal; i++  )
		for( int j = 0; j < vertical; j++ )
		{		
			//~ if( i*horizontal + j*vertical % (pixelNum/5) == 0 )
			//~ {
				//~ progress += 20;
				//~ std::cout << "rendering " << progress << "%" << std::endl;
				//~ std::cout << ".";
			//~ }
			dvec3 color = dvec3(0,0,0);
			
			if( supersampling )		// use Jitter algorithm super sampling and take the average as the final color
			{
				// divide the pixel area into samplerate*sameplerate grids and shot an ray randomly through that grid
				// the position refers to the middle of the grid, need also to 
				for( int si = 0; si < samplerate; si++ )
					for( int sj = 0; sj < samplerate; sj++ )
					{
						//~ dvec3 viewPlanePoint = viewPlaneBottomLeft + 
								//~ + ((double)si + (((double)rand())/RAND_MAX) - 0.5) * xIncrement 
								//~ + ((double)sj + (((double)rand())/RAND_MAX) - 0.5) * yIncrement;
						
						dvec3 viewPlanePoint = viewPlaneBottomLeft + 
								+ ((double) i * samplerate + si + (((double)rand() / RAND_MAX) - 0.5 ) ) * xIncrement 
								+ ((double) j * samplerate + sj + (((double)rand() / RAND_MAX) - 0.5 ) ) * yIncrement;
						
						dvec3* point = new dvec3( camera );
						dvec3* initialRay = new dvec3(normalize(viewPlanePoint - camera));
						dvec3* eyeVector = new dvec3( -(*initialRay) );
						
						
						
						color = color + trace( point, initialRay, eyeVector, 0, 1, false );
				
						delete initialRay;
						delete point;
						delete eyeVector;
					}
				
				color = color / (double)( samplerate * samplerate );
				
				//~ color = dvec3(1,1,1);
			}
			else 	// if no supersampling simply shot ray through center of the pixel
			{
				dvec3 viewPlanePoint = viewPlaneBottomLeft + ((double)i)*xIncrement + ((double)j)*yIncrement;
				
				dvec3* point = new dvec3( camera );
				dvec3* initialRay = new dvec3( normalize( viewPlanePoint - camera ) );
				dvec3* eyeVector = new dvec3( -(*initialRay) );
				
				// we asume the material the ray originates from is equal to 1 (air)
				color = trace( point, initialRay, eyeVector, 0, 1, false );
				
				delete point;
				delete initialRay;
				delete eyeVector;
			}

			// cap maximum rgb value to 1
			if( color[0] > 1)
				color[0] = 1;
			if( color[1] > 1)
				color[1] = 1;
			if( color[2] > 1)
				color[2] = 1;
			
			image[ j*horizontal + i ] = color;
			
		}
}
