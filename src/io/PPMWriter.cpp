#include "PPMWriter.h"
#include <cstdlib>
#include <cstdio>

void PPMWriter::setDimensions( int xdim, int ydim )
{
	this->xdim = xdim;
	this->ydim = ydim;
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
	//~ (void) fprintf(fp, "P6\n%d %d\n255\n", xdim, ydim);
	//~ fprintf(fp, "P6\n%d %d\n255\n", xdim, ydim);
	fprintf(fp, "P3\n%d %d\n255\n", xdim, ydim);	// header of ppm ASCII
	for( int j = 0; j < ydim; ++j )
	{
		for( int i = 0; i < xdim; ++i )
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
