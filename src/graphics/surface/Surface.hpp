#ifndef SURFACE_H
#define SURFACE_H

#include "../../lib/glm/glm.hpp"
#include "../material/Material.hpp"
#include "../IntersectionResult.hpp"
#include <string>

class Surface
{
	private:
		Material* material;	
		//~ glm::mat4 transformations = glm::mat4(1.0);	// initializes with identity matrix
		glm::dmat4 transformations;	// initializes with identity matrix
		
	public:
		Surface(){ transformations = glm::dmat4(1.0); }
		
		void setMaterial( Material* material ){ this->material = material; }
		
		void translate( glm::dvec3 translate ); 
		void scale( glm::dvec3 scale );
		void rotateX( double rotateX );
		void rotateY( double rotateY );
		void rotateZ( double rotateZ );
		
		Material* getMaterial(){ return material; }
		glm::dmat4 getTransformationMatrix(){ return transformations; }
		
		std::string transformationMatrixString();
		
		virtual IntersectionResult* intersect( glm::dvec3 point, glm::dvec3 ray ) = 0;		
};		

#endif // SURFACE_H
