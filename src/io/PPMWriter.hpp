#ifndef PPMWRITER_H
#define PPMWRITER_H

#include <string>
#include "../lib/glm/vec3.hpp"

class PPMWriter
{
	private:
		int horizontal;
		int vertical;
		std::string filename;
		
	public:
		void setResolution( int horizontal, int vertical );
		void setFilename( std::string filename ){ this->filename = filename; }
		void writePPM( glm::dvec3* image );
};

inline void PPMWriter::setResolution( int horizontal, int vertical )
{
	this->horizontal = horizontal; this->vertical = vertical;
}


#endif //PPMWRITER_H
