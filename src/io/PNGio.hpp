#ifndef PNGIO_H
#define PNGIO_H

#include "../lib/lodepng/lodepng.h"
#include "../lib/glm/vec3.hpp"
#include <string>


class PNGio
{
	private:
		size_t width;
		size_t height;
	
	public :
		glm::dvec3* readPNG(std::string filename);
		size_t getWidth(){ return width; }
		size_t getHeight(){ return height; }
		void writePNG( std::string filename, glm::dvec3* image, unsigned width, unsigned height );	
};


#endif // PNGREADER_H
