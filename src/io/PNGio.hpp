#ifndef PNGIO_H
#define PNGIO_H

#include "../lib/lodepng/lodepng.h"
#include "../lib/glm/vec3.hpp"
#include <string>


class PNGio
{
	public :
		glm::dvec3* readPNG(const char* filename);
		void writePNG( std::string filename, glm::dvec3* image, unsigned width, unsigned height );	
};


#endif // PNGREADER_H
