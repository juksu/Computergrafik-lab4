#include "Raytracer.h"
#include <cmath>
#include "../lib/glm/gtc/matrix_transform.hpp"

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

void Raytracer::setLookAt( vec3 center )
{
	this->center = center;
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
	double tanY = tan( ((double)vertical / horizontal) * (fov * M_PI / 180) );
	std::cout << "vertical = " << vertical << " horizontal = " << horizontal << std::endl;
	std::cout << (double)vertical/ horizontal << std::endl;
	std::cout << "tanx = " << tanX << ", tanY = " << tanY  << std::endl;
	
	// traverse through every pixel
	for( int u = 0; u < horizontal; u++  )
		for( int v = 0; v < vertical; v++ )
		{		
			// calculate ray;
			/// TODO: irgendwie komische vektoren -> doppelter winkel? tanX passt aber vielleicht hier?
			double x = (2*u - (horizontal-1))*tanX;
			double y = (2*v - (vertical-1))*tanY;
			double z = 0;
			// this assumes the camera is in the center with y-axis being up and viewplane at z = -1;
			
			
			std::cout << "blaNOTrans " << x << ", " << y << ", " << z << ", " << 0 << std::endl;
			/// TODO need to transform ray? Or maybe World?
			mat4 lookAtTransformation = lookAt( camera, center, up );

			vec4 bla = lookAtTransformation * vec4(x,y,z,1);
			
			std::cout << "blaTrans " << bla[0] << ", " << bla[1] << ", " << bla[2] << ", " << bla[3] << std::endl;
			
			

			// for debuging
			double r = 0;
			double g = 0;
			double b = 0;
			
			if( u > horizontal/2 - 1 )
				//~ r = x;
				r = bla[0];
			if( v > vertical/2 - 1 )
				//~ g = y;
				g = bla[1];
			
			//~ r = x;
			//~ g = y;
			//~ if( u > horizontal/2 - 1 && v > vertical/2 - 1 )
				//~ b = x+y;
			//~ b = z;
			b = bla[2];
			
			vec3 color = vec3(abs(r), abs(g), abs(b));
			//~ if( !( color[0] == 0 && color[1] == 0 && color[2] == 0 ) )
				//~ color = normalize(color);
			//~ color = vec3(color[0]*255, color[1]*255, color[2]*255);
			
			// need to swap picture, otherwise it will be upside down
			image[ (vertical-1-v)*horizontal + u ] = color;
			
		}
		
		
		// for debuging?
		//~ double max = 0;
		//~ for( int i = 0; i < vertical; i++ )
			//~ for( int j = 0; j < horizontal; j++ )
				//~ for( int k = 0; k < 3; k++ )
					//~ if( image[ i*horizontal + j ][k] > max )
						//~ max = image[ i*horizontal + j ][k];
//~ 
		//~ 
		//~ for( int i = 0; i < vertical; i++ )
			//~ for( int j = 0; j < horizontal; j++ )
				//~ for( int k = 0; k < 3; k++ )
				//~ {
					//~ image[ i*horizontal + j ][k] /= max;
					//~ image[ i*horizontal + j ][k] *= 255;
				//~ }
		
}
