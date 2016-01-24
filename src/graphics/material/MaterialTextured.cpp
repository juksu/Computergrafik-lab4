#include "MaterialTextured.hpp"
#include <cmath>
#include "../../lib/glm/gtx/string_cast.hpp"

glm::dvec3 MaterialTextured::getColor( double x, double y ) const
{
			//~ size_t xCoordinate = round(x*width);
			size_t xCoordinate = x*width;
			if( xCoordinate >= width )
				xCoordinate = width - 1;
			//~ size_t yCoordinate = round(y*height);
			size_t yCoordinate = y*height;
			if( yCoordinate >= height )
				yCoordinate = height - 1;
			//~ std::cout << "get TexCoo " << xCoordinate << " " << yCoordinate << " " << xCoordinate * height + yCoordinate << std::endl;
			//~ std::cout << "get TexCol " << to_string(texture[ xCoordinate * width + yCoordinate ]) << std::endl;
			//~ std::cout << "get TexCol " << to_string(texture[ yCoordinate * height + xCoordinate ]) << std::endl;
			return texture[ xCoordinate * height + yCoordinate ];
			//~ return texture[ yCoordinate * width + xCoordinate ];
			//~ return texture[ yCoordinate * height + xCoordinate ];
			//~ return glm::dvec3(0.3,0.4,0.5);
}
