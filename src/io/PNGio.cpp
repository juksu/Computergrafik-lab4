#include "PNGio.hpp"
#include <iostream>

glm::dvec3* PNGio::readPNG(const char* filename)
{
	std::vector<unsigned char> png;
	std::vector<unsigned char> image; //the raw pixels
	size_t width;
	size_t height;
	
	//load and decode
	unsigned error = lodepng::load_file(png, filename);
	if( !error )
		error = lodepng::decode(image, width, height, png);
	
	//if there's an error, display it
	if( error ) 
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	
	// i prefer to have image information in vec3
	glm::dvec3* vec3image = new glm::dvec3[ width * height ];
	for( unsigned i = 0; i < width * height; i++ )
	{
		vec3image[i][0] = ((double)image[ i*4]) / 256;
		vec3image[i][1] = ((double)image[ i*4 + 1]) / 256;
		vec3image[i][2] = ((double)image[ i*4 + 2]) / 256;
	}
	
	writePNG( "test.png", vec3image, width, height );
	
	return vec3image;
}

void PNGio::writePNG( std::string filename, glm::dvec3* image, unsigned width, unsigned height )
{
	std::cout << "writing image " << filename << std::endl; 
	
	std::vector<unsigned char> png;
	std::vector<unsigned char> imagePNG;
	
	for( size_t i = 0; i < width*height; i++ )
	{
		imagePNG.push_back( image[i][0] * 255 );
		imagePNG.push_back( image[i][1] * 255 );
		imagePNG.push_back( image[i][2] * 255 );
		imagePNG.push_back( 255 );
	}
	
	unsigned error = lodepng::encode(png, imagePNG, width, height);
	if( !error )
		lodepng::save_file(png, filename);
	
	//if there's an error, display it
	if( error ) 
		std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}
