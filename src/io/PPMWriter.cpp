#include "PPMWriter.hpp"
#include <cstdlib>
#include <cstdio>

void PPMWriter::writePPM( glm::dvec3* image )
{
	FILE *fp = fopen(filename.c_str(), "w");
	fprintf(fp, "P3\n%d %d\n255\n", horizontal, vertical);	// header of ppm ASCII
	for( int i = 0; i < vertical; i++ )
	{
		for( int j = 0; j < horizontal; j++ )
		{

			fprintf( fp, "%3d %3d %3d \t", 
					(int)(image[i*horizontal+j][0]*255), 
					(int)(image[i*horizontal+j][1]*255),
					(int)(image[i*horizontal+j][2]*255) );
			
		}
		fprintf( fp, "\n" );
	}
	fclose(fp);
}
