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
		
		/// TODO wird vielleicht gar nicht benötigt?
		//~ glm::vec3 translate;		/// TODO mesh has a second translate? or error on website?
		//~ glm::vec3 scale;			/// TODO scale has a second translate? or error on website?
									//~ /// Or is it possible to have same kind of transformations multiple times?
		//~ double rotateX;
		//~ double rotateY;
		//~ double rotateZ;
		
	public:
		Surface(){ transformations = glm::dmat4(1.0); }
		
		void setMaterial( Material* material ){ this->material = material; }
		
		//~ 
		//~ void setTranlate( glm::vec3 translate );
		//~ void setScale( glm::vec3 scale );
		//~ void setRotateX( double rotateX );
		//~ void setRotateY( double rotateY );
		//~ void setRotateZ( double rotateZ );
		//~ 
				//~ void setTransformations( glm::vec3 translate, glm::vec3 scale, 
				//~ double rotateX, double rotateY, double rotateZ, 
				//~ glm::vec3 translate, glm::vec3 scale );
		//~ 
		//~ void performTransformation();
		
		/// TODO, wait, wasn't it better to transform the ray!?!?
		void translate( glm::dvec3 translate ); 
		void scale( glm::dvec3 scale );
		void rotateX( double rotateX );
		void rotateY( double rotateY );
		void rotateZ( double rotateZ );
		
		Material* getMaterial(){ return material; }
		glm::dmat4 getTransformationMatrix(){ return transformations; }
		
		
		
		/// TODO don't make the transformations virtual but something else ?
		/// virtual void doSomethint() = 0;
		
		std::string transformationMatrixString();
		
		virtual IntersectionResult intersect( glm::dvec3 point, glm::dvec3 ray ) = 0;
		virtual void getIntersectionInformation( glm::dvec3 point, glm::dvec3 ray,
				IntersectionResult* intersectionResult ) = 0;
		
};		


#endif // SURFACE_H
