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
		void setFilename( std::string filename );
		void writePPM( glm::vec3* image );
};

#endif //PPMWRITER_H
