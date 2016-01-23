#include "PPMWriter.hpp"
#include <cstdlib>
#include <cstdio>
#include <iostream>

void PPMWriter::writePPM( glm::dvec3* image )
{
	std::cout << "writing image " << filename << std::endl;
	FILE *fp = fopen(filename.c_str(), "w");
	fprintf(fp, "P3\n%lu %lu\n255\n", horizontal, vertical);	// header of ppm ASCII
	for( size_t i = 0; i < vertical; i++ )
	{
		for( size_t j = 0; j < horizontal; j++ )
		{

			//~ fprintf( fp, "%3d %3d %3d \t", 		// use this line for better format but larger files
			fprintf( fp, "%d %d %d ", 
					(int)(image[i*horizontal+j][0]*255), 
					(int)(image[i*horizontal+j][1]*255),
					(int)(image[i*horizontal+j][2]*255) );
			
		}
		fprintf( fp, "\n" );
	}
	fclose(fp);
}

void PPMWriter::setFilename( std::string filename )
{
	int fileExt = filename.rfind(".");
	this->filename =  filename.substr(0, fileExt).append(".ppm");
}
