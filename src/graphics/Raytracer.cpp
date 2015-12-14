#include "Raytracer.h"
#include <cmath>

#include <iostream>
using namespace glm;

void Raytracer::setBackgroundColor( vec3 backgroundColor )
{
	this->backgroundColor = backgroundColor;
}

void Raytracer::setCameraPosition( vec3 camera )
{
	this->camera = camera;
}

void Raytracer::setLookAt( vec3 lookAt )
{
	this->lookAt = lookAt;
}

void Raytracer::setCameraUp( vec3 up )
{
	this->up = up;
}
void Raytracer::setHorizontalFOV( double fov )
{
	this->fov = fov;
}
void Raytracer::setResolution( int horizontal, int vertical )
{
	this->horizontal = horizontal;
	this->vertical = vertical;
}

void Raytracer::setMaxBounces( int maxBounces )
{
	this->maxBounces = maxBounces;
}


//~ void Raytracer::setupViewPlane()
//~ {
	//~ /// TODO
//~ }

vec3* Raytracer::getImage()
{
	return image;
}

void Raytracer::render()
{
	/// TODO all the graphics processing
	image = new vec3[horizontal*vertical];		
	
	// calculate the tan values for x and y using radian
	double tanX = tan( fov * M_PI / 180 );
	double tanY = tan( (vertical / horizontal) * (fov * M_PI / 180) );
	
	std::cout << "tanx = " << tanX << ", tanY = " << tanY  << std::endl;
	
	// traverse through every pixel
	for( int u = 0; u < horizontal; u++  )
		for( int v = 0; v < vertical; v++ )
		{		
			// calculate ray;
			double x = (2*u - (horizontal-1))*tanX;
			double y = (2*v - (vertical-1))*tanY;
			// this assumes the camera is in the center with y-axis being up
			/// TODO need to transform ray? Or maybe World?
			
			// for debuging
			vec3 color = vec3(abs(x), 0, abs(y));
			color = normalize(color);
			color = vec3(color[0]*256, color[1]*256, color[2]*256);
			
			image[u * horizontal + v] = color;
			
		}
}
