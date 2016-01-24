#include "PNGio.hpp"
#include <iostream>

glm::dvec3* PNGio::readPNG(std::string filename)
{
	std::cout << "reading texture " << filename << std::endl;
	
	std::vector<unsigned char> png;
	std::vector<unsigned char> image; //the raw pixels
	unsigned width;
	unsigned height;
	
	//load and decode
	unsigned error = lodepng::load_file(png, filename);
	if( !error )
		error = lodepng::decode(image, width, height, png);
	
	//if there's an error, display it
	if( error )
	{
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		this->width = 0;
		this->height = 0;
		return nullptr;
	}
	else
	{
		this->width = width;
		this->height = height;

		// i prefer to have image information in vec3
		// image is in column major - bring it in row major
		glm::dvec3* vec3image = new glm::dvec3[ width * height ];
		for( unsigned i = 0; i < width; i++ )
			for( unsigned j = 0; j < height; j++ )
			{
				vec3image[i*height + j][0] = ((double)image[ (j* width + i)*4]) / 255;
				vec3image[i*height + j][1] = ((double)image[ (j* width + i)*4 + 1]) / 255;
				vec3image[i*height + j][2] = ((double)image[ (j* width + i)*4 + 2]) / 255;
			}
		
		return vec3image;
	}
}

void PNGio::writePNG( std::string filename, const glm::dvec3* const image, unsigned width, unsigned height ) const
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
	
	// if there's an error, display it
	if( error ) 
		std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}
