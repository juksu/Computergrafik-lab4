#include "Raytracer.hpp"
#include <cmath>
#include "../lib/glm/gtc/matrix_transform.hpp"
//~ #include "../lib/glm/gtc/matrix_inverse.hpp"
#include "../lib/glm/gtx/string_cast.hpp"

#include <iostream>
using namespace glm;

vec3 Raytracer::trace( vec3 point, vec3 ray, int step )
{
	if( step > maxBounces )
		return backgroundColor;
	//~ 
	//~ vec4 test;
	//~ 
	vec3 color = backgroundColor;
	for( size_t i = 0; i < surfaceArray.size(); i++ )
	{
		if( surfaceArray.at(i)->intersect( point, ray ) )
		{
			color = surfaceArray.at(i)->getMaterial()->getColor();
			//~ std::cout << "trace color " << to_string(surfaceArray.at(i)->getMaterial()->getColor()) << std::endl;
			//~ /// TODO no depth test yet
			break;
		}
	}
	
	/// iterate through objects
	/// call for every object the intersect function
	/// in intersect transform the ray and point with the inverse of transformation matrix
	/// and not the object itself
	
	
	
	//~ vec3 color = vec3(ray[0], ray[1], ray[2]);
			//~ if( !( color[0] == 0 && color[1] == 0 && color[2] == 0 ) )
				//~ color = normalize(color);
	//~ color = vec3(abs(ray[0]*255), abs(ray[1]*255), abs(ray[2]*255));
	//~ vec3 color = ray;
	
	/// TODO: iterate through objects and find possible interaction
	/// if no interaction return backgroundcolor
	
	//~ color = backgroundColor;
	
	
	return color;
}

//~ void Raytracer::setBackgroundColor( vec3 backgroundColor )
//~ {
	//~ this->backgroundColor = backgroundColor;
//~ }
//~ 
//~ void Raytracer::setCameraPosition( vec3 camera )
//~ {
	//~ this->camera = camera;
//~ }
//~ 
//~ void Raytracer::setLookAt( vec3 center )
//~ {
	//~ this->center = center;
//~ }
//~ 
//~ void Raytracer::setCameraUp( vec3 up )
//~ {
	//~ this->up = up;
//~ }
//~ void Raytracer::setHorizontalFOV( double fov )
//~ {
	//~ this->fov = fov;
//~ }
//~ void Raytracer::setResolution( int horizontal, int vertical )
//~ {
	//~ this->horizontal = horizontal;
	//~ this->vertical = vertical;
//~ }
//~ 
//~ void Raytracer::setMaxBounces( int maxBounces )
//~ {
	//~ this->maxBounces = maxBounces;
//~ }


//~ void Raytracer::setupViewPlane()
//~ {
	//~ /// TODO
//~ }

//~ vec3* Raytracer::getImage()
//~ {
	//~ return image;
//~ }

void Raytracer::render()
{
	/// TODO all the graphics processing
	image = new vec3[horizontal*vertical];		
	
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
			
			vec3 ray = normalize(vec3(x,y,z));
			
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
					
			vec3 color = trace( camera, ray, 0 );


			// for debuging
			//~ double r = 0;
			//~ double g = 0;
			//~ double b = 0;
			
			//~ if( u > horizontal/2 - 1 )
				//~ r = x;
				//~ r = bla[0];
			//~ if( v > vertical/2 - 1 )
				//~ g = y;
				//~ g = bla[1];
			
			//~ r = x;
			//~ g = y;
			//~ if( u > horizontal/2 - 1 && v > vertical/2 - 1 )
				//~ b = x+y;
			//~ b = z;
			//~ b = bla[2];
			
			//~ vec3 color = vec3(abs(r), abs(g), abs(b));
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
