#include "Mesh.hpp"
#include "../../lib/glm/gtc/matrix_inverse.hpp"
#include "../../lib/glm/gtx/string_cast.hpp"
#include "../../lib/glm/gtc/matrix_access.hpp"
#include <iostream>

using namespace glm;

dvec2 solve2x2( double* matrix, double* result )
{
	// swap rows to largest pivot
	if( abs( matrix[2] > matrix[0] ) )
	{
		double temp = matrix[0];
		matrix[0] = matrix[2];
		matrix[2] = temp;
		temp = matrix[1];
		matrix[1] = matrix[3];
		matrix[3] = temp;
		
		temp = result[0];
		result[0] = result[1];
		result[1] = temp;
	}
	
	//~ std::cout << "matrix in solve2x2 after swap " << std::endl;
	//~ for( int i = 0; i < 4; i++ )
	//~ {
		//~ std::cout << matrix[i] << " ";
		//~ if( i % 2 == 1 )
			//~ std::cout << std::endl;
	//~ }
	
	//~ std::cout << "result in solveUV after swap " << std::endl;
	//~ for( int i = 0; i < 2; i++ )
	//~ {
		//~ std::cout << result[i] << " ";
	//~ }
	//~ std::cout << "'" << std::endl;
	
	// bring matrix into triangular form
	double factor = matrix[2]/matrix[0];
	//~ matrix[2] = matrix[2] - matrix[0] * factor;		// i know that this will be 0 so no need to calculate
	matrix[2] = 0;		// i know that this will be 0 so no need to calculate
	matrix[3] = matrix[3] - matrix[1] * factor;
	result[1] = result[1] - result[0] * factor;
	
	// bring matrix into diagonal form
	factor = matrix[1] / matrix[3];
	//~ matrix[1] = matrix[1] - matrix[3] * factor;		// i know that this will be 0 so no need to calculate
	matrix[1] = 0;		// i know that this will be 0 so no need to calculate
	result[0] = result[0] - result[1] * factor;
	
	// bring matrix into identity form
	result[1] = result[1] / matrix[3];
	matrix[3] = 1;
	result[0] = result[0] / matrix[0];
	matrix[0] = 1;
	
	return dvec2(result[0], result[1]);
}

dvec2 solveUV( double* matrix, double* result )
{	
	//~ std::cout << "matrix in solveUV " << std::endl;
	//~ for( int i = 0; i < 9; i++ )
	//~ {
		//~ std::cout << matrix[i] << " ";
		//~ if( i % 3 == 2 )
			//~ std::cout << std::endl;
	//~ }
	//~ 
	//~ std::cout << "result in solveUV " << std::endl;
	//~ for( int i = 0; i < 3; i++ )
	//~ {
		//~ std::cout << result[i] << " ";
	//~ }
	//~ std::cout << "'" << std::endl;
	
	int ignoreRow = -1;
	int ignoreColumn = -1;
	
	for( int i = 0; i < 3; i++ )
		if( matrix[3*i] == 0 && matrix[3*i + 1] == 0 && matrix[3*i + 2] == 0 )
		{
			ignoreRow = i;
			break;
		}
	for( int i = 0; i < 3; i++ )
		if( matrix[0 + i] == 0 && matrix[3 + i] == 0 && matrix[6 + i] == 0 )
		{
			ignoreColumn = i;
			break;
		}
	// from ignoreRow and ignoreColumn we can now build a 2x2 matrix which will be linear independent
	
	// if there was not any column or row with only 0 it still may be that there is a linear dependence
	/// TODO i hope that is right now -> need sleep zzzz
	int m1 = 0;
	if( m1 == ignoreRow )
		m1++;
	int m2 = m1 + 1;
	if( m2 == ignoreRow )
		m2++;
	int n1 = 2;
	if( n1 == ignoreColumn )
		n1--;
	int n2 = n1 - 1;
	if( n2 == ignoreColumn )
		n2--;
	
	while( true )
	{
		double psi1 = matrix[ m1*3 + n1 ] / matrix[ m2*3 + n1 ];
		double psi2 = matrix[ m1*3 + n2 ] / matrix[ m2*3 + n2 ];
		if( psi1 != psi2 )
		{	
			//~ if( ignoreColumn == -1 )
				//~ ignoreColumn = 0;
			//~ if( ignoreRow == -1 )
				//~ ignoreRow == 2;
			
			ignoreColumn = 0;
			if( ignoreColumn == n2 )
				ignoreColumn++;
			if( ignoreColumn == n1 )
				ignoreColumn++;
			
			ignoreRow = 0;
			if( ignoreRow == m1 )
				ignoreRow++;
			if( ignoreRow == m2 )
				ignoreRow++;
			break;
			
		}
		else if( ignoreRow == -1 )	// search for different row
		{
			if( m2 < 2 )
				m2++;
			else if( m1 < 1 )
				m1++;
		}
		else if( ignoreColumn == -1 )	// search for different column
		{
			if( n2 > -1 )
				n2--;
			else if( n1 > 0 )
				n1--;
		}
	}
	
	double linIndMatrix[4];
	double linIndResult[2];
	
	//~ std::cout << "matrix in solveUV - 2 " << std::endl;
	//~ for( int i = 0; i < 9; i++ )
	//~ {
		//~ std::cout << matrix[i] << " ";
		//~ if( i % 3 == 2 )
			//~ std::cout << std::endl;
	//~ }
	//~ 
	//~ std::cout << "ignoreRow " << ignoreRow << std::endl;
	//~ std::cout << "ignoreColumn " << ignoreColumn << std::endl;
	
	
	int it = 0;
	for( int i = 0; i < 3; i++ )
		for( int j = 0; j < 3; j++ )
		{
			if( i != ignoreRow && j != ignoreColumn )
			{
				//~ std::cout << "it " << it << " i " << i << " j " << j << std::endl;
				linIndMatrix[it] = matrix[i*3 + j];
				it++;
			}
		}
	it = 0;
	for( int i = 0; i < 3; i++ )
		if( i != ignoreRow )
		{
			linIndResult[it] = result[i];
			it++;
		}
	
	//~ std::cout << "linIndMatrix in solveUV " << std::endl;
	//~ for( int i = 0; i < 4; i++ )
	//~ {
		//~ std::cout << linIndMatrix[i] << " ";
		//~ if( i % 2 == 1 )
			//~ std::cout << std::endl;
	//~ }
	//~ 
	//~ std::cout << "linIndResult in solveUV " << std::endl;
	//~ for( int i = 0; i < 2; i++ )
	//~ {
		//~ std::cout << linIndResult[i] << " " << std::endl;
	//~ }
	
	
	/// TODO debug
	linIndMatrix[0] = matrix[1];
	linIndMatrix[1] = matrix[2];
	linIndMatrix[2] = matrix[4];
	linIndMatrix[3] = matrix[5];
	linIndResult[0] = result[0];
	linIndResult[1] = result[1];
	
	dvec2 uv = solve2x2( linIndMatrix, linIndResult );
	
	//~ std::cout << "uv in solveUV" << std::endl;
	//~ std::cout << to_string(uv) << std::endl;

/// TODO restore after debug
	// we got 1 - u - v and v
	//~ if( ignoreColumn == 1 )
		//~ return dvec2( 1-uv[1], uv[1] );
	// we got 1 - u - v and u
	//~ if( ignoreColumn == 2 )
		//~ return dvec2( uv[1], 1-uv[1] );	
	// else we have uv directly
	return uv;
}

IntersectionResult* Mesh::intersect( dvec3 point, dvec3 ray )
{
	IntersectionResult* intersectionResult = new IntersectionResult();
	intersectionResult->setIntersection( false );
	
	// transform point and ray into object coordinates with inverse transformation matrix
	// all transformations are affine, so affineInverse should work fine.
	/// TODO, for optimization i may calculate this only once (when setting transformation maybe?)
	dmat4 inverseTransformations = affineInverse( getTransformationMatrix() );
	
	dvec3 pointTransformed = dvec3( inverseTransformations * dvec4( point, 1 ) );
	dvec3 rayTransformed = dvec3( inverseTransformations * dvec4( ray, 0 ) );
	
	// test for all faces
	for( size_t i = 0; i < verticesPerFace.size()-1; i++ )
	{
		// polygons could be one with more than three vertices (for example quadrilaterals)
		// however this intersection test does (only?) work for triangles
		
		if( verticesPerFace.at( i + 1 ) - verticesPerFace.at( i ) == 3 )
		{
			double epsilon = 1e-12;
			
			/// TODO, we know that the ray can only enter an object through faces which normals face toward the point
			/// and only exit through faces which normals face away from the point
			/// could use that information to further accellerate calculation AND OR it may be usefull for transmittance?
			dvec3 v1 = vertices.at( faceVertices.at( verticesPerFace.at( i ) ) );
			dvec3 v2 = vertices.at( faceVertices.at( verticesPerFace.at( i ) + 1 ) );
			dvec3 v3 = vertices.at( faceVertices.at( verticesPerFace.at( i ) + 2 ) );
			//~ std::cout << "v1 " << to_string(v1) << std::endl;
			//~ std::cout << "v2 " << to_string(v2) << std::endl;
			//~ std::cout << "v3 " << to_string(v3) << std::endl;
			
			//~ dvec3 n = cross( v3 - v1, v2 - v1 ) / abs( length( cross( v3 - v1, v2 - v1 ) ) );
			dvec3 n = normalize(cross( v3 - v1, v2 - v1 ));
			//~ std::cout << "n " << to_string( n ) << std::endl;
			
			double nd = dot( n, rayTransformed );
			//~ std::cout << "nd " << nd << std::endl;
			
			
			// if n.d = 0 than either parallel to plane or inside plane therefore no intersection
			// if n.d != 0 than one intersection with the plane
			//~ if( abs(nd) > epsilon )
			//~ if( abs(nd) > abs(epsilon) )
			if( nd != 0 )
			{
				/// TODO lambda is 0 -> maybe some error in calculation?
				double lambda = ( dot( n, v1 ) - dot( n, pointTransformed ) ) / nd;
				intersectionResult->setLambda( lambda );
				//~ std::cout << "lambda " << lambda << std::endl;
				/// TODO
				//~ lambda = 1;
				dvec3 result = pointTransformed + lambda * rayTransformed;
				
				//~ std::cout << "pointTransformed " << to_string( pointTransformed ) << std::endl;
				//~ std::cout << "rayTransformed " << to_string( rayTransformed ) << std::endl;
				
				
				//~ dmat2x2 matrix;
				
				//~ dmat3x3 matrix;
				//~ matrix = column( matrix, 0, dvec3(0,1,3) );
				//~ matrix = column( matrix, 1, dvec3(3,2,3) );
				//~ matrix = column( matrix, 2, dvec3(6,4,2) );				
				//~ matrix = column( matrix, 0, v1 );
				//~ matrix = column( matrix, 1, v2 );
				//~ matrix = column( matrix, 2, v3 );
				//~ 
				//~ std::cout << to_string( matrix ) << std::endl << std::endl;
				
				
				
				double matrix[9];
				for( int i = 0; i < 3; i++ )
				{
					matrix[i*3] = v1[i];
					matrix[i*3+1] = v2[i];
					matrix[i*3+2] = v3[i];
				}
				
				double res[3];
				for( int i = 0; i < 3; i++ )
					res[i] = result[i];
				
				// solve the linear system to find out if intersection is inside the triangle
				/// TODO: write a different solver because this one probably makes the problems
				dvec2 uv = solveUV( matrix, res );
				//~ std::cout << "uv " << to_string(uv) << std::endl;
				
				//~ if( uv[0] >= 0 && uv[0] <= 1 )
				//~ if( abs(uv[0]) <= 1 )
					//~ if( uv[1] >= 0 && uv[1] <= 1 )
					//~ if( uv[1] <= 1 )		
					//~ if( uv[1] >= 0 )		
					//~ if( uv[0] <= 1 )		/// TODO hier ein fehler? -> vorzeichen verkehrt?
						if( uv[0] >= 0 )
						//~ if( abs(uv[0] + uv[1]) <= 1 )		// it seems to work only when using abs?
						//~ if( uv[0] + uv[1] <= 1 )
						//~ if( uv[0] + uv[1] > 0 )
						{
							intersectionResult->setIntersection( true );
							
							// get intersection Point and normal
							dvec3 intersectionPoint = pointTransformed + rayTransformed * lambda;
							intersectionResult->setIntersectionPoint( 
									dvec3(getTransformationMatrix()* dvec4( intersectionPoint, 1) ) );
					
							//~ dvec3 intersectionNormal = n;
							//~ intersectionResult->setNormal( 
									//~ normalize( dvec3( getTransformationMatrix() * dvec4( n, 0 ) ) ) );
							intersectionResult->setNormal( -normals.at( faceNormals.at( normalsPerFace.at( i ) ) ) );
						}
				
				
				//~ std::cout << "solve " << to_string( solve ) << std::endl;
			}			
		}
	}
	
	return intersectionResult;
}

/// TODO obsolete???
void Mesh::getIntersectionInformation( dvec3 point, dvec3 ray, 
		IntersectionResult* intersectionResult )
{
	
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
