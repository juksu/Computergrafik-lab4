#include "Surface.hpp"
#include "../../lib/glm/gtc/matrix_transform.hpp"
#include "../../lib/glm/gtx/string_cast.hpp"

void Surface::translate( glm::dvec3 translate )
{
	transformations = glm::translate( transformations, translate );
	inverseTransformations = glm::translate( inverseTransformations, -translate );
} 

void Surface::scale( glm::dvec3 scale )
{
	transformations = glm::scale( transformations, scale );
	inverseTransformations = glm::scale( inverseTransformations, 1.0/scale );
}

// important: glm::rotate uses degrees
void Surface::rotateX( double rotateX )
{
	transformations = glm::rotate( transformations, rotateX, glm::dvec3(1,0,0) );
	inverseTransformations = glm::rotate( inverseTransformations, -rotateX, glm::dvec3(1,0,0) );
}

void Surface::rotateY( double rotateY )
{
	transformations = glm::rotate( transformations, rotateY, glm::dvec3(0,1,0) );
	inverseTransformations = glm::rotate( inverseTransformations, -rotateY, glm::dvec3(0,1,0) );
}

void Surface::rotateZ( double rotateZ )
{
	transformations = glm::rotate( transformations, rotateZ, glm::dvec3(0,0,1) );
	inverseTransformations = glm::rotate( inverseTransformations, -rotateZ, glm::dvec3(0,0,1) );
}

std::string Surface::transformationMatrixString() const
{
	return glm::to_string( transformations );
}
