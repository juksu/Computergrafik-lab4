#ifndef PPMWRITER_H
#define PPMWRITER_H

#include <string>

class PPMWriter
{
	private:
		int xdim;
		int ydim;
		std::string filename;
		
	public:
		//~ PPMWriter();
		void setDimensions( int xdim, int ydim );
		void setFilename( std::string filename );
		void writePPM();
};

#endif //PPMWRITER_H
