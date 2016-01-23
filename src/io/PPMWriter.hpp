#ifndef PPMWRITER_H
#define PPMWRITER_H

#include <string>
#include "../lib/glm/vec3.hpp"

class PPMWriter
{
	private:
		size_t horizontal;
		size_t vertical;
		std::string filename;
		
	public:
		void setResolution( size_t horizontal, size_t vertical ){
				this->horizontal = horizontal; this->vertical = vertical; }
		void setFilename( std::string filename );
		void writePPM( glm::dvec3* image );
};

#endif //PPMWRITER_H
