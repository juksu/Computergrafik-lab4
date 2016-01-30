#include "Surface.hpp"
#include "../../lib/glm/gtc/matrix_transform.hpp"
#include "../../lib/glm/gtx/string_cast.hpp"
#include <cmath>

void Surface::translate( glm::dvec3 translate )
{
	transformations = glm::translate( transformations, translate );
	inverseTransformationsSet = false;
} 

void Surface::scale( glm::dvec3 scale )
{
	transformations = glm::scale( transformations, scale );
	inverseTransformationsSet = false;
}

// important: glm::rotate uses radians, NOT degrees
// WTF!!!??? seriously! the documentation clearly says it would use degrees but it actually does use radians
void Surface::rotateX( double rotateX )
{
	transformations = glm::rotate( transformations, rotateX * M_PI / 180, glm::dvec3(1,0,0) );
	inverseTransformationsSet = false;
}

void Surface::rotateY( double rotateY )
{
	transformations = glm::rotate( transformations, rotateY * M_PI / 180, glm::dvec3(0,1,0) );
	inverseTransformationsSet = false;
}

void Surface::rotateZ( double rotateZ )
{
	transformations = glm::rotate( transformations, rotateZ * M_PI / 180, glm::dvec3(0,0,1) );
	inverseTransformationsSet = false;
}

std::string Surface::transformationMatrixString() const
{
	return glm::to_string( transformations );
}
