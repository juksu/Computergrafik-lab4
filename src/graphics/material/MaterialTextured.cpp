#include "MaterialTextured.hpp"
#include <cmath>
#include "../../lib/glm/gtx/string_cast.hpp"

glm::dvec3 MaterialTextured::getColor( double x, double y ) const
{
			size_t xCoordinate = x*width;
			xCoordinate = xCoordinate % width;	/// TODO this may cause texture error where the last column of texture will look like the first one?
			// it may be that we get xCoordinate out of range 1*width = width -> out of range (zero based)
			//~ if( xCoordinate >= width )
				//~ xCoordinate = width - 1;
			size_t yCoordinate = y*height;
			yCoordinate = yCoordinate % height;
			//~ if( yCoordinate >= height )
				//~ yCoordinate = height - 1;
			return texture[ xCoordinate * height + yCoordinate ];
}
