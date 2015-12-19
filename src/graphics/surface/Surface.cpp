#include "Surface.hpp"
#include "../../lib/glm/gtc/matrix_transform.hpp"
#include "../../lib/glm/gtx/string_cast.hpp"
//~ #include <string>

void Surface::translate( glm::vec3 translate )
{
	transformations = glm::translate( transformations, translate );
} 

void Surface::scale( glm::vec3 scale )
{
	transformations = glm::scale( transformations, scale );
}

// important glm::rotate uses degrees and only accepts float
void Surface::rotateX( double rotateX )
{
	transformations = glm::rotate( transformations, (float)rotateX, glm::vec3(1,0,0) );
}

void Surface::rotateY( double rotateY )
{
	transformations = glm::rotate( transformations, (float)rotateY, glm::vec3(0,1,0) );
}

void Surface::rotateZ( double rotateZ )
{
	transformations = glm::rotate( transformations, (float)rotateZ, glm::vec3(0,0,1) );
}

std::string Surface::transformationMatrixString()
{
	return glm::to_string( transformations );
}
