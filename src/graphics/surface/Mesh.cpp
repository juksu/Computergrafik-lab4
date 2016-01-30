#include "Mesh.hpp"
#include "../../lib/glm/gtc/matrix_inverse.hpp"
#include "../../lib/glm/gtx/string_cast.hpp"
#include "../../lib/glm/gtc/matrix_access.hpp"
#include "../../lib/glm/gtc/matrix_transform.hpp"
#include <iostream>

using namespace glm;

IntersectionResult* Mesh::intersect( dvec3 point, dvec3 ray )
{
	IntersectionResult* intersectionResult = new IntersectionResult();
	intersectionResult->setIntersection( false );
	// ray could interact with multiple faces of the object -> need to test which one is the closest
	intersectionResult->setLambda( std::numeric_limits<double>::max() );
	
	// transform point and ray into object coordinates with inverse transformation matrix
	if( !inverseTransformationsSet )
		inverseTransformations = inverse( transformations );
	dvec3 pointTransformed = dvec3( inverseTransformations * dvec4( point, 1 ) );
	dvec3 rayTransformed = dvec3( inverseTransformations * dvec4( ray, 0 ) );
	
	// test for all faces
	for( size_t i = 0; i < verticesPerFace.size()-1; i++ )
	{
		// polygons could be one with more than three vertices (for example quadrilaterals)
		// however for now have intersection test only for triangles	
		if( verticesPerFace.at( i + 1 ) - verticesPerFace.at( i ) == 3 )
		{
			double epsilon = 1e-12;
			
			dvec3 v1 = vertices.at( faceVertices.at( verticesPerFace.at( i ) ) );
			dvec3 v2 = vertices.at( faceVertices.at( verticesPerFace.at( i ) + 1 ) );
			dvec3 v3 = vertices.at( faceVertices.at( verticesPerFace.at( i ) + 2 ) );
			
			dvec3 planeNormal = normalize(cross( v2 - v1, v3 - v1 ));
			
			double dotPlaneNRay = dot( planeNormal, rayTransformed );
			
			// we want the planeNormal to face towards the point where the ray is comming from
			// check for orientation, normal and rayTransformed should have opposite orientation
			// important for some later calculations
			if( dotPlaneNRay > 0 )
			{
				planeNormal = -planeNormal;	// let planeNormal face opposite direction
				dotPlaneNRay = -dotPlaneNRay;	// dot product of opposite orrientation = -dot product of same orrientation
			}
			
			if( abs(dotPlaneNRay) > epsilon )
			{
				// calculate the distance from ray origin to plane
				double lambda = - ( dot( planeNormal, pointTransformed - v1 ) ) / dotPlaneNRay;
				
				// if that face is closer to other faces tested before set this as the new closest intersection
				// if lambda is very close to zero or is even in opposite direction we do not have an interaction
				//~ if( lambda > epsilon )
				if( lambda < intersectionResult->getLambda() && lambda > epsilon )
				{
					// test for ray triangle intersection with Barycentric Coordinates Computation
					// V(s,t) = V_1 + s(V_2 - V_1) + t(V_3 - V_1) = V_1 + su + tv
					
					dvec3 intersectionPoint = pointTransformed + rayTransformed * lambda;
					dvec3 w = intersectionPoint - v1;
					dvec3 u = v2 - v1;
					dvec3 v = v3 - v1;
					
					// formular from http://geomalgorithms.com/a06-_intersect-2.html
					double dotUV = dot( u, v );
					double dotWV = dot( w, v );
					double dotWU = dot( w, u );
					double dotUU = dot( u, u );
					double dotVV = dot( v, v );
					double denom = dotUV * dotUV - dotUU * dotVV;

					double s = ( dotUV * dotWV - dotVV * dotWU ) / denom;
					double t = ( dotUV * dotWU - dotUU * dotWV ) / denom;
					
					if( s >= 0 )
						if( t >= 0 )
							if( s + t <= 1 )
							{
								intersectionResult->setIntersection( true );
								intersectionResult->setLambda( lambda );
								
								// get intersection Point and normal
								intersectionResult->setIntersectionPoint( 
										dvec3( transformations* dvec4( intersectionPoint, 1) ) );
										
								intersectionResult->setNormal( 
										normalize( dvec3( transformations * dvec4( planeNormal, 0 ) ) ) );
								
								// texture coordinates
								
								dvec2 vt1 = textureCoordinates.at( faceTextureCoordinates.at( textureCoordinatesPerFace.at( i ) ) );
								dvec2 vt2 = textureCoordinates.at( faceTextureCoordinates.at( textureCoordinatesPerFace.at( i ) + 1 ) );
								dvec2 vt3 = textureCoordinates.at( faceTextureCoordinates.at( textureCoordinatesPerFace.at( i ) + 2 ) );
								
								dvec2 texCoordinate = vt1 + s*(vt2 - vt1) + t*(vt3 - vt1);
								
								intersectionResult->setSurfaceColor( material->getColor( texCoordinate[0], texCoordinate[1] ) );
							}
				}
			}			
		}
	}
	
	return intersectionResult;
}

void Mesh::printMesh()
{
	// debug
	std::cout << "verticesPerFace" << std::endl;
	for( size_t i = 0; i < verticesPerFace.size(); i++ )
		std::cout << verticesPerFace.at(i) << ", ";
	std::cout << std::endl << "normalsPerFace" << std::endl;
	for( size_t i = 0; i < normalsPerFace.size(); i++ )
		std::cout << normalsPerFace.at(i) << ", ";
	std::cout << std::endl << "texturesPerFace" << std::endl;
	for( size_t i = 0; i < textureCoordinatesPerFace.size(); i++ )
		std::cout << textureCoordinatesPerFace.at(i) << ", ";
	std::cout << std::endl << "faceVertices" << std::endl;
	for( size_t i = 0; i < faceVertices.size(); i++ )
		std::cout << faceVertices.at(i) << ", ";
	std::cout << std::endl << "faceNormals" << std::endl;
	for( size_t i = 0; i < faceNormals.size(); i++ )
		std::cout << faceNormals.at(i) << ", ";
	std::cout << std::endl << "faceTextureCoordinates" << std::endl;
	for( size_t i = 0; i < faceTextureCoordinates.size(); i++ )
		std::cout << faceTextureCoordinates.at(i) << ", ";
	std::cout << std::endl << "vertices" << std::endl;
	for( size_t i = 0; i < vertices.size(); i++ )
		std::cout << glm::to_string(vertices.at(i)) << ", ";
	std::cout << std::endl << "normals" << std::endl;
	for( size_t i = 0; i < normals.size(); i++ )
		std::cout << glm::to_string(normals.at(i)) << ", ";
	std::cout << std::endl << "textureCoordinates" << std::endl;
	for( size_t i = 0; i < textureCoordinates.size(); i++ )
		std::cout << glm::to_string(textureCoordinates.at(i)) << ", ";
	std::cout << std::endl;
}	
