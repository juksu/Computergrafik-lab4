#include "MaterialTextured.hpp"
#include <cmath>
#include "../../lib/glm/gtx/string_cast.hpp"

glm::dvec3 MaterialTextured::getColor( double x, double y ) const
{
			size_t xCoordinate = x*width;
			if( xCoordinate >= width )
				xCoordinate = width - 1;
			size_t yCoordinate = y*height;
			if( yCoordinate >= height )
				yCoordinate = height - 1;
			return texture[ xCoordinate * height + yCoordinate ];
}
