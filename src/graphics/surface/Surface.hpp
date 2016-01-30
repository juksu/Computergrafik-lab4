#ifndef SURFACE_H
#define SURFACE_H

#include "../../lib/glm/glm.hpp"
#include "../material/Material.hpp"
#include "../IntersectionResult.hpp"
#include <string>

class Surface
{
	protected:
		Material* material;	
		glm::dmat4 transformations;
		glm::dmat4 inverseTransformations;
		bool inverseTransformationsSet;
		
	public:
		//~ Surface(){ transformations = glm::dmat4(1.0); inverseTransformationsSet = false; inverseTransformations = glm::dmat4(1.0); }
		Surface(){ transformations = glm::dmat4(1.0); inverseTransformationsSet = false; }
		
		void setMaterial( Material* material ){ this->material = material; }
		Material* getMaterial() const { return material; }
				
		void translate( glm::dvec3 translate ); 
		void scale( glm::dvec3 scale );
		void rotateX( double rotateX );
		void rotateY( double rotateY );
		void rotateZ( double rotateZ );

		std::string transformationMatrixString() const;
		
		virtual IntersectionResult* intersect( glm::dvec3 point, glm::dvec3 ray ) = 0;
};		

#endif // SURFACE_H
