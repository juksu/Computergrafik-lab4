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
		size_t getWidth() const { return width; }
		size_t getHeight() const { return height; }
		void writePNG( std::string filename, const glm::dvec3* const image, unsigned width, unsigned height ) const;	
};


#endif // PNGREADER_H
