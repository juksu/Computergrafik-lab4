#include "PPMWriter.h"
#include <cstdlib>
#include <cstdio>

void PPMWriter::setResolution( int horizontal, int vertical )
{
	this->horizontal = horizontal;
	this->vertical = vertical;
}

void PPMWriter::setFilename( std::string filename )
{
	this->filename = filename;
}

void PPMWriter::writePPM()		/// TODO: array with color
{
	//~ const int dimx = 800, dimy = 800;
	//~ int i, j;
	//~ FILE *fp = fopen(filename.c_str(), "wb"); /* b - binary mode */
	FILE *fp = fopen(filename.c_str(), "w"); /* b - binary mode */
	//~ (void) fprintf(fp, "P6\n%d %d\n255\n", horizontal, vertical);
	//~ fprintf(fp, "P6\n%d %d\n255\n", horizontal, vertical);
	fprintf(fp, "P3\n%d %d\n255\n", horizontal, vertical);	// header of ppm ASCII
	for( int i = 0; i < vertical; ++i )
	{
		for( int j = 0; j < horizontal; ++j )
		{
			//~ static unsigned char color[3];
			int color[3];

			color[0] = i % 256;  /* red */
			color[1] = j % 256;  /* green */
			color[2] = (i * j) % 256;  /* blue */
			fprintf( fp, "%3d %3d %3d \t", color[0], color[1], color[2] );
			//~ fprintf( fp, "%d %d %d \t", color[0], color[1], color[2] );
		}
		fprintf( fp, "\n" );
	}
	fclose(fp);
}


void PPMWriter::writePPM( glm::vec3* image )		/// TODO: array with color
{
	FILE *fp = fopen(filename.c_str(), "w");
	fprintf(fp, "P3\n%d %d\n255\n", horizontal, vertical);	// header of ppm ASCII
	for( int i = 0; i < vertical; i++ )
	{
		for( int j = 0; j < horizontal; j++ )
		{

			fprintf( fp, "%3d %3d %3d \t", 
					(int)image[i*horizontal+j][0], (int)image[i*horizontal+j][1], (int)image[i*horizontal+j][2] );
			
		}
		fprintf( fp, "\n" );
	}
	fclose(fp);
}
